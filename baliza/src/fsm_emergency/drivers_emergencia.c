#include "drivers_emergencia.h"
#include "gps/drivers_gps.h"


int SolicitudDatos (fsm_t *this)
{
    
#ifdef DEBUG_PRINT_ENABLE
    printf("Se comprueba si se han solicitado datos del servidor.\n");
#endif /* DEBUG_PRINT_ENABLE */

    fsm_emergencia_t *fp = (fsm_emergencia_t*) this;
    bool rxSolicitudDatos = false;

    if( *fp->solicitudDatosQueue != 0 ) 
    {
        // Receive a message on the solicitudDatos queue.
        xQueueReceive( *(fp->solicitudDatosQueue), &(rxSolicitudDatos), ( TickType_t ) 0);
#ifdef DEBUG_PRINT_ENABLE
        printf("Se ha recibido %d en la cola de solicitudDatos.\n", rxSolicitudDatos);
#endif /* DEBUG_PRINT_ENABLE */
    } else {
#ifdef DEBUG_PRINT_ENABLE
        printf("Error en abrir cola receive solicitudDatos.\n");
#endif /* DEBUG_PRINT_ENABLE */
        return 0;
    }
    return rxSolicitudDatos;
};

int SenalIncendio (fsm_t* this) 
{
    fsm_emergencia_t *fp = (fsm_emergencia_t*) this;
    bool rxIncendio = false;

    if( *fp->incendioQueue != 0 ) {
        // Receive a message on the incendio queue.
        xQueueReceive( *(fp->incendioQueue), &(rxIncendio), ( TickType_t ) 0);
#ifdef DEBUG_PRINT_ENABLE
        printf("Se ha recibido %d en la cola de incendio.\n", rxIncendio);
#endif /* DEBUG_PRINT_ENABLE */
    } else {
#ifdef DEBUG_PRINT_ENABLE
        printf("Error en abrir cola receive incendio.\n");
#endif /* DEBUG_PRINT_ENABLE */
        return 0;
    }
    return (rxIncendio && !MODO_SILENCIOSO);
};


int SenalNoIncendio (fsm_t* this) 
{
    fsm_emergencia_t *fp = (fsm_emergencia_t*) this;
    bool rxIncendio = true;

    if( *fp->incendioQueue != 0 ) {
        // Receive a message on the created queue. 
        xQueueReceive( *(fp->incendioQueue), &(rxIncendio), ( TickType_t ) 0);
#ifdef DEBUG_PRINT_ENABLE
        printf("Se ha recibido %d en la cola de incendio.\n", rxIncendio);
#endif /* DEBUG_PRINT_ENABLE */
    }
    else {
#ifdef DEBUG_PRINT_ENABLE
       printf("Error en abrir cola receive no incendio.\n");
#endif /* DEBUG_PRINT_ENABLE */
       return 0;
    }
    return !rxIncendio;
};

int SenalIncendioSil (fsm_t* this) 
{
    fsm_emergencia_t *fp = (fsm_emergencia_t*) this;
    bool rxIncendio = false;

    if( *fp->incendioQueue != 0 ) {
        // Receive a message on the incendio queue. 
        xQueueReceive( *(fp->incendioQueue), &(rxIncendio), ( TickType_t ) 0);
#ifdef DEBUG_PRINT_ENABLE
        printf("Se ha recibido %d en la cola de incendio.\n", rxIncendio);
#endif /* DEBUG_PRINT_ENABLE */
    } else {
#ifdef DEBUG_PRINT_ENABLE
        printf("Error en abrir cola receive incendio.\n");
#endif /* DEBUG_PRINT_ENABLE */
        return 0;
    }
    return (rxIncendio && MODO_SILENCIOSO);
};


void SendDatos (fsm_t* this)
{
#ifdef DEBUG_PRINT_ENABLE
    printf("Se envían los datos de la baliza al servidor.\n");
#endif /* DEBUG_PRINT_ENABLE */

    fsm_emergencia_t *fp = (fsm_emergencia_t *)this;
    sensors_data_t rxDataSensor[NUM_SENSORS];

    // Se lee la cola de datos
    if ( xQueuePeek(*(fp->datosMQTTQueue), (void *) rxDataSensor, ( TickType_t ) 0) != pdTRUE ){
#ifdef DEBUG_PRINT_ENABLE
    printf("No hemos recibido ningún dato.\n");
#endif
        return;
    }

    char topic[512], to_send[512]; //size of the message

    for (int i = 0; i < NUM_SENSORS; i++)
    {
        sprintf(to_send, "%f", rxDataSensor[i].temperature);
#ifdef DEBUG_PRINT_ENABLE
        printf(to_send);
        printf("\n");
#endif /* DEBUG_PRINT_ENABLE */
        sprintf(topic, "Datos_Baliza/temperatura/%d", i);
        esp_mqtt_client_publish((*fp->client), topic, to_send, 0, 0, 0);

        sprintf(to_send, "%f", rxDataSensor[i].humidity);
#ifdef DEBUG_PRINT_ENABLE
        printf(to_send);
        printf("\n");
#endif /* DEBUG_PRINT_ENABLE */
        sprintf(topic, "Datos_Baliza/humedad/%d", i);
        esp_mqtt_client_publish((*fp->client), topic, to_send, 0, 0, 0);

        sprintf(to_send, "%f", rxDataSensor[i].gas_resistance);
#ifdef DEBUG_PRINT_ENABLE
        printf(to_send);
        printf("\n");
#endif /* DEBUG_PRINT_ENABLE */
        sprintf(topic, "Datos_Baliza/gases/%d", i);
        esp_mqtt_client_publish((*fp->client), topic, to_send, 0, 0, 0);

        sprintf(to_send, "%f", rxDataSensor[i].pressure);
#ifdef DEBUG_PRINT_ENABLE
        printf(to_send);
        printf("\n");
#endif /* DEBUG_PRINT_ENABLE */
        sprintf(topic, "Datos_Baliza/presion/%d", i);
        esp_mqtt_client_publish((*fp->client), topic, to_send, 0, 0, 0);
    }

    // Lectura del GPS
    float longitud, latitud;
    read_GPS( &longitud, &latitud);

    sprintf(to_send, "La posicion de la baliza es: %f, %f \n", longitud, latitud );
    sprintf(topic, "Datos_Baliza/posicion/%d", 0);
#ifdef DEBUG_PRINT_ENABLE
    printf(to_send);
    printf("\n");
#endif /* DEBUG_PRINT_ENABLE */
    esp_mqtt_client_publish((*fp->client), topic, to_send, 0, 0, 0);

    return;
};

void EnvioSenalEmergencia (fsm_t* this)
{

#ifdef DEBUG_PRINT_ENABLE
    printf("Se envía la señal de emergencia al servidor.\n");
#endif /* DEBUG_PRINT_ENABLE */

    fsm_emergencia_t *fp = (fsm_emergencia_t*) this;

    char topic[512], to_send[512];
    sprintf(topic, "incendio/%d", 0);
    esp_mqtt_client_publish((*fp->client), topic, "Puede haber incendio!! Que se envie un dron.\n", 0, 0, 0);

    // Lectura del GPS
    float longitud, latitud;
    read_GPS( &longitud, &latitud);

    sprintf(to_send, "La posicion de la baliza es: %f, %f \n", longitud, latitud );
    sprintf(topic, "Datos_Baliza/posicion/%d", 0);
#ifdef DEBUG_PRINT_ENABLE
    printf(to_send);
    printf("\n");
#endif /* DEBUG_PRINT_ENABLE */
    esp_mqtt_client_publish((*fp->client), topic, to_send, 0, 0, 0);
    
    return;
};

void SendDatosYEmergencia (fsm_t* this){
    EnvioSenalEmergencia (this);
    SendDatos (this);
}