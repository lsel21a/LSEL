#include "drivers_emergencia.h"


int SolicitudDatos (fsm_t *this)
{
    //hay que implementar la recepcion de la solicitud Datos con MQTT
    return 0;
};

int SenalIncendio (fsm_t* this) 
{
    fsm_emergencia_t *fp = (fsm_emergencia_t*) this;
    bool rxIncendio = false;

    if( *fp->incendioQueue != 0 )
   {
       // Receive a message on the created queue.  If a
       // message is not immediately available we use the default sampling period.
       xQueueReceive( *(fp->incendioQueue), &(rxIncendio), ( TickType_t ) 0);
   }
   else
   {
       printf("Error en abrir cola receive incendio");
       return 0;
   }
   return (rxIncendio && !MODO_SILENCIOSO);
};


int SenalNoIncendio (fsm_t* this) 
{
    fsm_emergencia_t *fp = (fsm_emergencia_t*) this;
    bool rxIncendio = true;

    if( *fp->incendioQueue != 0 )
   {
       // Receive a message on the created queue.  If a
       // message is not immediately available we use the default sampling period.
       xQueueReceive( *(fp->incendioQueue), &(rxIncendio), ( TickType_t ) 0);
   }
   else
   {
       printf("Error en abrir cola receive no incendio");
       return 0;
   }
   return !rxIncendio;
};

int SenalIncendioSil (fsm_t* this) 
{
    fsm_emergencia_t *fp = (fsm_emergencia_t*) this;
    bool rxIncendio = false;

    if( *fp->incendioQueue != 0 )
   {
       // Receive a message on the created queue.  If a
       // message is not immediately available we use the default sampling period.
       xQueueReceive( *(fp->incendioQueue), &(rxIncendio), ( TickType_t ) 0);
   }
   else
   {
       printf("Error en abrir cola receive incendio");
       return 0;
   }
   return (rxIncendio && MODO_SILENCIOSO);
};


void SendDatos (fsm_t* this)
{
    //hay que implementar envio de los datos al servidor
    return;
};


void EnvioSenalEmergencia (fsm_t* this)
{
    //hay que implementar envio de la senal de emergencia al servidor
    return;
};