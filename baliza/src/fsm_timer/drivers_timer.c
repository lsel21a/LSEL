#include "drivers_timer.h"


int ReceiveMuestreoRapido (fsm_t* this) 
{
    fsm_timer_t *fp = (fsm_timer_t*) this;
    bool rxMuestreoRapido = 0;

    if( *fp->muestreoRapidoQueue != 0 )
   {
       // Receive a message on the created queue. 
       xQueueReceive( *(fp->muestreoRapidoQueue), &(rxMuestreoRapido), ( TickType_t ) 0);   
       printf("Muestreo rapido recibido = %d. \n", rxMuestreoRapido);
   }
   else
   {
       printf("Error en abrir cola receive muestreo rapido");
   }
    return rxMuestreoRapido;
}

int WaitNormal (fsm_t* this) 
{
    TickType_t xDelay;

    xDelay= 2000 / portTICK_PERIOD_MS;
    vTaskDelay( xDelay );
    printf("Fin de espera Tick normal. \n");
    return 1;
}


int ReceiveMuestreoNormal (fsm_t* this) 
{
    fsm_timer_t *fp = (fsm_timer_t*) this;
    bool rxMuestreoRapido = 1;

    if( *fp->muestreoRapidoQueue != 0 )
   {
       // Receive a message on the created queue.  If a
       // message is not immediately available we use the default sampling period.
       xQueueReceive( *(fp->muestreoRapidoQueue), &(rxMuestreoRapido), ( TickType_t ) 0);   
       printf("Muestreo rapido recibido = %d. \n", rxMuestreoRapido);
   }
   else
   {
       printf("Error en abrir cola receive muestreo rapido");
       return 0;
   }
    return !rxMuestreoRapido;
}

int WaitRapido (fsm_t* this) 
{
    TickType_t xDelay;

    xDelay= 1000 / portTICK_PERIOD_MS;
    vTaskDelay( xDelay );
    printf("Fin de espera Tick rapido. \n");
    return 1;
}


void SendTick (fsm_t* this)
{
    fsm_timer_t *fp = (fsm_timer_t*) this;
    bool Tick = true;

    if( *fp->tickQueue != 0 )
    {
        // Send a bool (tick) to fsm_sensor. 
        if( xQueueGenericSend( *(fp->tickQueue), ( void * ) &Tick, ( TickType_t ) 0, queueSEND_TO_BACK) != pdPASS  )
        {
            printf("Error en enviar el tick.\n");
            return;
        }
        else
        {
            printf("Tick enviado correctamente.\n");
        }
    }
}