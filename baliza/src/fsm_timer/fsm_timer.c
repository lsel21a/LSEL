#include "fsm_timer.h"
#include "drivers_timer.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

// Explicit FSM description
static fsm_trans_t timer_tt[] = {
    {MUESTREO_NORMAL, ReceiveMuestreoRapido, MUESTREO_RAPIDO, NULL},
    {MUESTREO_NORMAL, WaitNormal , MUESTREO_NORMAL, SendTick},
    {MUESTREO_RAPIDO, ReceiveMuestreoNormal, MUESTREO_NORMAL, NULL},
    {MUESTREO_RAPIDO, WaitRapido  , MUESTREO_RAPIDO, SendTick},
    {-1, NULL, -1, NULL},
};

void  fsm_timer_init ( fsm_timer_t * this, QueueHandle_t *muestreoRapidoQueue, QueueHandle_t *tickQueue)
{
    fsm_init((fsm_t *)this, timer_tt);
    this->muestreoRapidoQueue = muestreoRapidoQueue;
    this->tickQueue = tickQueue;
#ifdef DEBUG_PRINT_ENABLE
    printf("FSM timer inicializada.\n");
#endif /* DEBUG_PRINT_ENABLE */
}
