#include "drivers_timer.h"


static int ReceiveMuestreoRapido (fsm_t* this) 
{
    fsm_timer_t *fp = (fsm_timer_t*) this;

    if( *fp->muestreoRapidoQueue != 0 )
   {
        bool * rxMuestreoRapido;
        bool muestreo_rapido = false;
        TickType_t xDelay;

       // Receive a message on the created queue.  If a
       // message is not immediately available we use the default sampling period.
       if( xQueueReceive( *(fp->muestreoRapidoQueue), &( rxMuestreoRapido ), ( TickType_t ) 0))       
       {
           // rxDatoValido now points to the bool variable posted by LecturaFinalizadaOK from fsm_sensores (drivers.c file).
           return * rxMuestreoRapido;
       }
       else
       {
            return 0;
       }
   }
   else
   {
       printf("Error en abrir cola receive muestreo rapido");
       return 0;
   }

}

static int WaitNormal (fsm_t* this) 
{
    TickType_t xDelay;

    xDelay= 2000 / portTICK_PERIOD_MS;
    vTaskDelay( xDelay );
    return 1;
}


static int ReceiveMuestreoNormal (fsm_t* this) 
{
    fsm_timer_t *fp = (fsm_timer_t*) this;

    if( *fp->muestreoRapidoQueue != 0 )
   {
        bool * rxMuestreoRapido;
        bool muestreo_rapido = false;
        TickType_t xDelay;

       // Receive a message on the created queue.  If a
       // message is not immediately available we use the default sampling period.
       if( xQueueReceive( *(fp->muestreoRapidoQueue), &( rxMuestreoRapido ), ( TickType_t ) 0))       
       {
           // rxDatoValido now points to the bool variable posted by LecturaFinalizadaOK from fsm_sensores (drivers.c file).
           return !(* rxMuestreoRapido);
       }
       else
       {
            return 0;
       }
   }
   else
   {
       printf("Error en abrir cola receive muestreo rapido");
       return 0;
   }

}

static int WaitRapido (fsm_t* this) 
{
    TickType_t xDelay;

    xDelay= 4000 / portTICK_PERIOD_MS;
    vTaskDelay( xDelay );
    return 1;
}


static void SetMuestreoRapido (fsm_t* this)
{
    return;
}


static void SetMuestreoNormal (fsm_t* this)
{
    return;
}


static void SendTick (fsm_t* this)
{
    fsm_timer_t *fp = (fsm_timer_t*) this;

    if( *fp->tickQueue != 0 )
    {
        bool Tick = true;
        // Send a bool (tick) to fsm_sensor. 
        if( xQueueGenericSend( *(fp->tickQueue), ( void * ) &Tick, ( TickType_t ) 0, queueSEND_TO_BACK) != pdPASS  )
        {
            printf("Error en enviar el tick.\n");
            return;
        }
    }
}