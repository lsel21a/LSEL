#include "esp_log.h"

#include "drivers_emergencia.h"
#include "gps/drivers_gps.h"
#include "config.h"

static const char* TAG = "drivers_emergencia";

int SolicitudDatos (fsm_t *this)
{
    ESP_LOGD(TAG, "[SolicitudDatos] Se comprueba si se han solicitado datos del servidor.");

    fsm_emergencia_t *fp = (fsm_emergencia_t*) this;
    bool rxSolicitudDatos = false;

    if( *fp->solicitudDatosQueue != 0 ) 
    {
        // Receive a message on the solicitudDatos queue.
        if (xQueueReceive( *(fp->solicitudDatosQueue), &(rxSolicitudDatos), ( TickType_t ) 0) == pdTRUE) {
            ESP_LOGD(TAG, "[SolicitudDatos] Se ha recibido %d en la cola de solicitudDatos.", rxSolicitudDatos);
        } else {
            ESP_LOGD(TAG, "[SolicitudDatos] Cola de solicitudDatos vacia.");
        }
    } else {
        ESP_LOGE(TAG, "[SolicitudDatos] Error al abrir cola de solicitudDatos.");
        return 0;
    }

    return rxSolicitudDatos;
}

int SenalIncendio (fsm_t* this) 
{
    fsm_emergencia_t *fp = (fsm_emergencia_t*) this;
    bool rxIncendio = false;

    if( *fp->incendioQueue != 0 ) {
        // Receive a message on the incendio queue.
        if (xQueueReceive( *(fp->incendioQueue), &(rxIncendio), ( TickType_t ) 0) == pdTRUE) {
            ESP_LOGD(TAG, "[SenalIncendio] Se ha recibido %d en la cola de incendio.", rxIncendio);
        } else {
            ESP_LOGD(TAG, "[SenalIncendio] Cola de incendio vacia.");
        }
    } else {
        ESP_LOGE(TAG, "[SenalIncendio] Error en abrir cola de incendio.");
        return 0;
    }

    return (rxIncendio && !CONFIG_EMERGENCIA_MODO_SILENCIOSO);
}

int SenalNoIncendio (fsm_t* this) 
{
    fsm_emergencia_t *fp = (fsm_emergencia_t*) this;
    bool rxIncendio = true;

    if( *fp->incendioQueue != 0 ) {
        // Receive a message on the created queue. 
        if (xQueueReceive( *(fp->incendioQueue), &(rxIncendio), ( TickType_t ) 0) == pdTRUE) {
            ESP_LOGD(TAG, "[SenalNoIncendio] Se ha recibido %d en la cola de incendio.", rxIncendio);
        } else {
            ESP_LOGD(TAG, "[SenalNoIncendio] Cola de incendio vacia.");
        }
    }
    else {
       ESP_LOGE(TAG, "[SenalNoIncendio] Error en abrir cola de incendio.");
       return 0;
    }
    return !rxIncendio;
}

int SenalIncendioSil (fsm_t* this) 
{
    fsm_emergencia_t *fp = (fsm_emergencia_t*) this;
    bool rxIncendio = false;

    if( *fp->incendioQueue != 0 ) {
        // Receive a message on the incendio queue. 
        if (xQueueReceive( *(fp->incendioQueue), &(rxIncendio), ( TickType_t ) 0) == pdTRUE) {
            ESP_LOGD(TAG, "[SenalIncendioSil] Se ha recibido %d en la cola de incendio.", rxIncendio);
        } else {
            ESP_LOGD(TAG, "[SenalIncendioSil] Cola de incendio vacia.");
        }
    } else {
        ESP_LOGE(TAG, "[SenalIncendioSil] Error en abrir cola de incendio.");
        return 0;
    }

    return (rxIncendio && CONFIG_EMERGENCIA_MODO_SILENCIOSO);
}

void SendDatos (fsm_t* this)
{
    ESP_LOGD(TAG, "[SendDatos] Se envían los datos de la baliza al servidor.");

    fsm_emergencia_t *fp = (fsm_emergencia_t *)this;
    sensors_data_t rxDataSensor[CONFIG_SENSOR_NUM];

    // Se lee la cola de datos
    if ( xQueuePeek(*(fp->datosMQTTQueue), (void *) rxDataSensor, ( TickType_t ) 0) != pdTRUE ){
        ESP_LOGD(TAG, "[SendDatos] No hemos recibido ningún dato.");
        return;
    }

    char topic[512], to_send[512]; //size of the message

    for (int i = 0; i < CONFIG_SENSOR_NUM; i++)
    {
        // Temperature
        sprintf(to_send, "%f", rxDataSensor[i].temperature);
        ESP_LOGD(TAG, "[SendDatos] %s", to_send);

        sprintf(topic, "%s/%d/%s/%d", CONFIG_MQTT_TOPIC_PREFIX, CONFIG_BALIZA_ID, CONFIG_MQTT_TOPIC_TEMPERATURA, i);
        esp_mqtt_client_publish((*fp->client), topic, to_send, 0, CONFIG_MQTT_QOS, 0);

        // Humidity
        sprintf(to_send, "%f", rxDataSensor[i].humidity);
        ESP_LOGD(TAG, "[SendDatos] %s", to_send);

        sprintf(topic, "%s/%d/%s/%d", CONFIG_MQTT_TOPIC_PREFIX, CONFIG_BALIZA_ID, CONFIG_MQTT_TOPIC_HUMEDAD, i);
        esp_mqtt_client_publish((*fp->client), topic, to_send, 0, CONFIG_MQTT_QOS, 0);

        // Gas resistance
        sprintf(to_send, "%f", rxDataSensor[i].gas_resistance);
        ESP_LOGD(TAG, "[SendDatos] %s", to_send);

        sprintf(topic, "%s/%d/%s/%d", CONFIG_MQTT_TOPIC_PREFIX, CONFIG_BALIZA_ID, CONFIG_MQTT_TOPIC_GASES, i);
        esp_mqtt_client_publish((*fp->client), topic, to_send, 0, CONFIG_MQTT_QOS, 0);

        // Pressure
        sprintf(to_send, "%f", rxDataSensor[i].pressure);
        ESP_LOGD(TAG, "[SendDatos] %s", to_send);

        sprintf(topic, "%s/%d/%s/%d", CONFIG_MQTT_TOPIC_PREFIX, CONFIG_BALIZA_ID, CONFIG_MQTT_TOPIC_PRESION, i);
        esp_mqtt_client_publish((*fp->client), topic, to_send, 0, CONFIG_MQTT_QOS, 0);
    }

    // Lectura del GPS
    float longitud, latitud;
    read_GPS( &longitud, &latitud);

    sprintf(to_send, "%f;%f", longitud, latitud );
    ESP_LOGD(TAG, "[SendDatos] %s", to_send);

    sprintf(topic, "%s/%d/%s", CONFIG_MQTT_TOPIC_PREFIX, CONFIG_BALIZA_ID, CONFIG_MQTT_TOPIC_GPS);
    esp_mqtt_client_publish((*fp->client), topic, to_send, 0, CONFIG_MQTT_QOS, 0);

    return;
}

void EnvioSenalEmergencia (fsm_t* this)
{
    ESP_LOGD(TAG, "[EnvioSenalEmergencia] Se envía la señal de emergencia al servidor.");

    fsm_emergencia_t *fp = (fsm_emergencia_t*) this;

    char topic[512], to_send[512];
    sprintf(topic, "%s/%d/%s", CONFIG_MQTT_TOPIC_PREFIX, CONFIG_BALIZA_ID, CONFIG_MQTT_TOPIC_INCENDIO);
    esp_mqtt_client_publish((*fp->client), topic, "Puede haber incendio!! Que se envie un dron.", 0, CONFIG_MQTT_QOS, 0);

    // Lectura del GPS
    float longitud, latitud;
    read_GPS( &longitud, &latitud);

    sprintf(to_send, "%f;%f", longitud, latitud );
    ESP_LOGD(TAG, "[EnvioSenalEmergencia] %s", to_send);

    sprintf(topic, "%s/%d/%s", CONFIG_MQTT_TOPIC_PREFIX, CONFIG_BALIZA_ID, CONFIG_MQTT_TOPIC_GPS);
    esp_mqtt_client_publish((*fp->client), topic, to_send, 0, CONFIG_MQTT_QOS, 0);
    
    return;
};

void SendDatosYEmergencia (fsm_t* this){
    EnvioSenalEmergencia(this);
    SendDatos(this);
}