#include "drivers_emergencia.h"


int SolicitudDatos (fsm_t *this)
{
    //hay que implementar la recepcion de la solicitud Datos con MQTT
    return 0;
};

int SenalIncendio (fsm_t* this) 
{
    fsm_emergencia_t *fp = (fsm_emergencia_t*) this;

    if( *fp->incendioQueue != 0 )
   {
        bool * rxIncendio;

       // Receive a message on the created queue.  If a
       // message is not immediately available we use the default sampling period.
       if( xQueueReceive( *(fp->incendioQueue), &( rxIncendio ), ( TickType_t ) 0))       
       {
           // rxIncendio now points to the bool variable.
           if(MODO_SILENCIOSO == 0)
           {
           return * rxIncendio;
           }
           else{
               return 0;
           }
       }
       else
       {
            return 0;
       }
   }
   else
   {
       printf("Error en abrir cola receive incendio");
       return 0;
   }
   return 0;
};


int SenalNoIncendio (fsm_t* this) 
{
      fsm_emergencia_t *fp = (fsm_emergencia_t*) this;

    if( *fp->incendioQueue != 0 )
   {
        bool * rxIncendio;

       // Receive a message on the created queue.  If a
       // message is not immediately available we use the default sampling period.
       if( xQueueReceive( *(fp->incendioQueue), &( rxIncendio ), ( TickType_t ) 0))       
       {
           // rxIncendio now points to the bool variable.
           return !(* rxIncendio);
       }
       else
       {
            return 0;
       }
   }
   else
   {
       printf("Error en abrir cola receive no incendio");
       return 0;
   }
   return 0;
};

int SenalIncendioSil (fsm_t* this) 
{
    fsm_emergencia_t *fp = (fsm_emergencia_t*) this;

    if( *fp->incendioQueue != 0 )
   {
        bool * rxIncendio;

       // Receive a message on the created queue.  If a
       // message is not immediately available we use the default sampling period.
       if( xQueueReceive( *(fp->incendioQueue), &( rxIncendio ), ( TickType_t ) 0))       
       {
           // rxIncendio now points to the bool variable.
           if(MODO_SILENCIOSO == 1)
           {
           return * rxIncendio;
           }
           else{
               return 0;
           }
       }
       else
       {
            return 0;
       }
   }
   else
   {
       printf("Error en abrir cola receive incendio");
       return 0;
   }
   return 0;
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

void NoSenalEmergencia (fsm_t* this)
{
    return;
};