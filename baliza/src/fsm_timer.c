#include "fsm_timer.h"
#include "drivers_timer.h"

// Explicit FSM description

static fsm_trans_t Timer_tt[] = {
    {MUESTREO_NORMAL, ReceiveMuestreoRapido, MUESTREO_RAPIDO, SetMuestreoRapido},
    {MUESTREO_NORMAL, WaitNormal           , MUESTREO_NORMAL, SendTick},
    {MUESTREO_RAPIDO, ReceiveMuestreoNormal, MUESTREO_NORMAL, SetMuestreoNormal },
    {MUESTREO_RAPIDO, WaitRapido           , MUESTREO_RAPIDO, SendTick},
    {-1, NULL, -1, NULL},

};



void  fsm_timer_init ( fsm_timer_t * this, QueueHandle_t *muestreoRapidoQueue, QueueHandle_t *tickQueue)
{
    fsm_init((fsm_t *)this, Timer_tt);
    this->muestreoRapidoQueue = muestreoRapidoQueue;
    this->tickQueue = tickQueue;
    printf("FSM timer inicializada.\n");
}
