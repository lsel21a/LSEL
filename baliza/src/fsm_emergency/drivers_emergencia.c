#include "drivers_emergencia.h"


int SolicitudDatos (fsm_t *this)
{
    //hay que implementar la recepcion de la solicitud Datos con MQTT
#ifdef DEBUG_PRINT_ENABLE
    printf("Se comprueba si se han solicitado datos del servidor.\n");
#endif /* DEBUG_PRINT_ENABLE */
    return 0;
};

int SenalIncendio (fsm_t* this) 
{
    fsm_emergencia_t *fp = (fsm_emergencia_t*) this;
    bool rxIncendio = false;

    if( *fp->incendioQueue != 0 ) {
        // Receive a message on the created queue.  If a
        // message is not immediately available we use the default sampling period.
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
        // Receive a message on the created queue.  If a
        // message is not immediately available we use the default sampling period.
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
        // Receive a message on the created queue.  If a
        // message is not immediately available we use the default sampling period.
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
    //hay que implementar envio de los datos al servidor
#ifdef DEBUG_PRINT_ENABLE
    printf("Se envían los datos de los sensores al servidor.\n");
#endif /* DEBUG_PRINT_ENABLE */
    return;
};


void EnvioSenalEmergencia (fsm_t* this)
{
    //hay que implementar envio de la senal de emergencia al servidor
#ifdef DEBUG_PRINT_ENABLE
    printf("Se envía la señal de emergencia al servidor.\n");
#endif /* DEBUG_PRINT_ENABLE */
    return;
};