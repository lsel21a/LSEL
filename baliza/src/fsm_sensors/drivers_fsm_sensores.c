#include <string.h>
#include <stdbool.h>

#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "drivers_fsm_sensores.h"
#include "fsm_sensores.h"

#include "config.h"

static bool deadline = false;
static bool datos_validos = false;

sensors_config_t devices[CONFIG_SENSOR_NUM];

static const char* TAG = "drivers_fsm_sensores";


int checkStart_ON(fsm_t *this)
{
    fsm_sensores_t *fp = (fsm_sensores_t*) this;
    bool tick = false;

    if( *(fp->tickQueue) != 0 ) {
        // Receive a message on the created queue.  
        xQueueReceive( *(fp->tickQueue), (void *) &tick, ( TickType_t ) 0);
        ESP_LOGD(TAG, "[checkStart_ON] Tick %d recibido en CheckStart_ON.", tick);
    } else {
        ESP_LOGE(TAG, "[checkStart_ON] Cola de incendio no esta correctamente creada.");
        return 0;
    }
    return tick;
}

void Activa_Sensores(fsm_t *this){

    //sensors_status_t result;
    ESP_LOGD(TAG, "[Activa_Sensores] Activación sensores.");

    int i;
    for (i=0; i<CONFIG_SENSOR_NUM; i++){
        if(sensors_init(&(devices[i])) != SENSORS_OK)
        {
            ESP_LOGE(TAG, "[Activa_Sensores] Error al activar sensor %d.", i);
        }
    }

    ESP_LOGD(TAG, "[Activa_Sensores] Activación de sensores OK.");
    deadline = true;
}

int Deadline(fsm_t *this){
    ESP_LOGD(TAG, "[Deadline] Lectura deadline.");
    return deadline;
}

void Lectura_Sensores(fsm_t *this){

    deadline = false;

    ESP_LOGD(TAG, "[Lectura_Sensores] Lectura de los sensores.");

    int i;
    for(i=0; i<CONFIG_SENSOR_NUM; i++){
        if(get_data(&(devices[i])) != SENSORS_OK) {
            ESP_LOGE(TAG, "[Lectura_Sensores] Error en la lectura del sensor %d.", i);
        } else {
            ESP_LOGD(TAG, "[Lectura_Sensores] Se ha leído %d ºC en el sensor %d.", (int) devices[i].data.temperature, i);
        }
    }

    ESP_LOGD(TAG, "[Lectura_Sensores] Lectura de los sensores OK.");
}

int LecturaFinalizadaOK(fsm_t *this){
    fsm_sensores_t *fp = (fsm_sensores_t*) this;

    if( *(fp->datoValidoQueue) != 0 )
    {
        bool DatoValido = true;
        // Send a bool (datoValido) to fsm_deteccion_incendio. 
        if( xQueueSend( *(fp->datoValidoQueue), ( void * ) &DatoValido, ( TickType_t ) 0) != pdPASS  )
        {
            // Failed to post the message.
            ESP_LOGE(TAG, "[LecturaFinalizadaOK] Error en el envio del dato válido.");
            return 0;
        }
        else{
            ESP_LOGD(TAG, "[LecturaFinalizadaOK] Envío dato válido por cola.");
            return 1;
        }
    }
    else
    {
        ESP_LOGE(TAG, "[LecturaFinalizadaOK] Cola de datoValido no esta correctamente creada.");
        return 0;
    }
	return 0;
}

void Send_Data(fsm_t *this) {
    fsm_sensores_t *fp = (fsm_sensores_t*) this;

    sensors_data_t txDataSensor[CONFIG_SENSOR_NUM];

    int i;
    for (i=0;i<CONFIG_SENSOR_NUM;i++) {
        memcpy(&(txDataSensor[i]), &(devices[i].data), sizeof(sensors_data_t));

        ESP_LOGD(TAG, "[Send_Data] \tSe envia %f ºC.", txDataSensor[i].temperature);
        ESP_LOGD(TAG, "[Send_Data] \tSe envia %f %%H20.", txDataSensor[i].humidity);
        ESP_LOGD(TAG, "[Send_Data] \tSe envia %f Ohms de gas.", txDataSensor[i].gas_resistance);
    }
    xQueueSend( *(fp->datosSensoresQueue), ( void * ) txDataSensor, ( TickType_t ) 0);
    
    ESP_LOGD(TAG, "[Send_Data] Datos de los sensores enviados.");
    datos_validos = true;
}


void Apagar_Sensores(fsm_t *this){
    
    datos_validos = false;

    ESP_LOGD(TAG, "[Apagar_Sensores] Apagado de los sensores.");

    int i;
    for(i=0; i<CONFIG_SENSOR_NUM; i++){
        if(sleep_data(&(devices[i])) != SENSORS_OK)
        {
            ESP_LOGW(TAG, "[Apagar_Sensores] Error al desactivar sensor %d.", i);
        }
    }
}

int ReturnTrue(fsm_t *this){
	
	return 1;
	
}