#include "drivers_emergencia.h"


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

    for (int i = 0; i < NUM_SENSORS; i++)
    {
        char to_send[15]; //size of the message
        sprintf(to_send, "%g", fp->temperatura[i]);
#ifdef DEBUG_PRINT_ENABLE
        printf(to_send);
        printf("\n");
#endif /* DEBUG_PRINT_ENABLE */
        esp_mqtt_client_publish((*fp->client), "Datos_Baliza/temperatura", to_send, 0, 0, 0);

        sprintf(to_send, "%g", fp->humedad[i]);
#ifdef DEBUG_PRINT_ENABLE
        printf(to_send);
        printf("\n");
#endif /* DEBUG_PRINT_ENABLE */
        esp_mqtt_client_publish((*fp->client), "Datos_Baliza/humedad", to_send, 0, 0, 0);

        sprintf(to_send, "%g", fp->gases[i]);
#ifdef DEBUG_PRINT_ENABLE
        printf(to_send);
        printf("\n");
#endif /* DEBUG_PRINT_ENABLE */
        esp_mqtt_client_publish((*fp->client), "Datos_Baliza/gases", to_send, 0, 0, 0);
    }

    return;
};

void EnvioSenalEmergencia (fsm_t* this)
{

#ifdef DEBUG_PRINT_ENABLE
    printf("Se envía la señal de emergencia al servidor.\n");
#endif /* DEBUG_PRINT_ENABLE */

    fsm_emergencia_t *fp = (fsm_emergencia_t*) this;

    esp_mqtt_client_publish((*fp->client), "incendio", "Puede haber incendio!! Que se envie un dron.\n", 0, 0, 0);
    
    return;
};