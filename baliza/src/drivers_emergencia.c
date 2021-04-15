#include "drivers_emergencia.h"


static int SolicitudDatos (fsm_t* this) 
{
    //hay que implementar la recepcion de la solicitud Datos con MQTT

}

static int SeñalIncendio (fsm_t* this) 
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
}


static int SeñalNoIncendio (fsm_t* this) 
{
      fsm_emergencia_t *fp = (fsm_emergencia_t*) this;

    if( *fp->incendioQueue != 0 )
   {
        bool * rxIncendio;
        
        TickType_t xDelay;

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
}

static int SeñalIncendioSil (fsm_t* this) 
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
}


static void SendDatos (fsm_t* this)
{
    //hay que implementar envio de los datos al servidor
    return;
}


static void EnvioSeñalEmergencia (fsm_t* this)
{
    //hay que implementar envio de la senal de emergencia al servidor
    return;
}


static void BackToIdle (fsm_t* this)
{
    return;
}


static void NoSeñalEmergencia (fsm_t* this)
{
    return;
}