#include <stdbool.h>

#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "drivers_timer.h"
#include "fsm_timer.h"

#include "config.h"

static const char* TAG = "drivers_timer";


int ReceiveMuestreoRapido (fsm_t* this) 
{
    fsm_timer_t *fp = (fsm_timer_t*) this;
    bool rxMuestreoRapido = false;

    if( *(fp->muestreoRapidoQueue) != 0 )
    {
        // Receive a message on the created queue. 
        if (xQueueReceive( *(fp->muestreoRapidoQueue), (void *) &rxMuestreoRapido, ( TickType_t ) 0) == pdTRUE) {
            ESP_LOGD(TAG, "[ReceiveMuestreoRapido] Se ha recibido %d en la cola de muestreoRapido.", rxMuestreoRapido);
        } else {
            ESP_LOGD(TAG, "[ReceiveMuestreoRapido] Cola de muestreoRapido vacia.");
        }
    } else {
        ESP_LOGW(TAG, "[ReceiveMuestreoRapido] Error al abrir cola de muestreoRapido.");
    }
    return rxMuestreoRapido;
}

int WaitNormal (fsm_t* this) 
{
    TickType_t xDelay;

    xDelay = (CONFIG_TIMER_MEASURE_NORMAL * 1000) / portTICK_PERIOD_MS;
    vTaskDelay( xDelay );

    ESP_LOGD(TAG, "[WaitNormal] Fin de espera Tick normal.");
    return 1;
}


int ReceiveMuestreoNormal (fsm_t* this) 
{
    fsm_timer_t *fp = (fsm_timer_t*) this;
    bool rxMuestreoRapido = true;

    if( *(fp->muestreoRapidoQueue) != 0 )
    {
        // Receive a message on the created queue.  If a
        // message is not immediately available we use the default sampling period.
        if (xQueueReceive( *(fp->muestreoRapidoQueue), (void *) &rxMuestreoRapido, ( TickType_t ) 0) == pdTRUE) {
            ESP_LOGD(TAG, "[ReceiveMuestreoNormal] Se ha recibido %d en la cola de muestreoRapido.", rxMuestreoRapido);
        } else {
            ESP_LOGD(TAG, "[ReceiveMuestreoNormal] Cola de muestreoRapido vacia.");
        }
    } else {
        ESP_LOGW(TAG, "[ReceiveMuestreoNormal] Error al abrir cola de muestreoRapido.");
        return 0;
   }
    return !rxMuestreoRapido;
}

int WaitRapido (fsm_t* this) 
{
    TickType_t xDelay;

    xDelay = (CONFIG_TIMER_MEASURE_QUICK * 1000) / portTICK_PERIOD_MS;
    vTaskDelay( xDelay );

    ESP_LOGD(TAG, "[WaitRapido] Fin de espera Tick rapido.");
    return 1;
}


void SendTick (fsm_t* this)
{
    fsm_timer_t *fp = (fsm_timer_t*) this;
    bool Tick = true;

    if( *(fp->tickQueue) != 0 )
    {
        // Send a bool (tick) to fsm_sensor. 
        if( xQueueGenericSend( *(fp->tickQueue), (void *) &Tick, ( TickType_t ) 0, queueSEND_TO_BACK) != pdPASS  )
        {
            ESP_LOGW(TAG, "[SendTick] Error al enviar tick.");
            return;
        }
        else
        {
            ESP_LOGD(TAG, "[SendTick] Tick enviado correctamente.");
        }
    }
}
