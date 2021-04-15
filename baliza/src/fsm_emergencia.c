#include "fsm_emergencia.h"
#include "drivers_emergencia.h"

// Explicit FSM description

static fsm_trans_t Emergencia_tt[] = {
    {IDLE_EMERGENCIA,       SolicitudDatos       , IDLE_EMERGENCIA,             SendDatos},
    {IDLE_EMERGENCIA,       SeñalIncendio        , EMERGENCIA,                  EnvioSeñalEmergencia},
    {EMERGENCIA,            SolicitudDatos       , EMERGENCIA,                  SendDatos},
    {EMERGENCIA,            SeñalNoIncendio      , IDLE_EMERGENCIA,             BackToIdle},
    {IDLE_EMERGENCIA,       SeñalIncendioSil     , EMERGENCIA_SILENCIOSA,       NoSeñalEmergencia},
    {EMERGENCIA_SILENCIOSA, SolicitudDatos       , EMERGENCIA_SILENCIOSA,       SendDatos},
    {EMERGENCIA_SILENCIOSA, SeñalNoIncendio      , IDLE_EMERGENCIA,             BackToIdle},    
    {-1, NULL, -1, NULL},

};


void  fsm_emergencia_init ( fsm_emergencia_t * this, QueueHandle_t *modoSilenciosoQueue, QueueHandle_t *incendioQueue)
{
    fsm_init((fsm_t *)this, Emergencia_tt);
    this->modoSilenciosoQueue = modoSilenciosoQueue;
    this->incendioQueue = incendioQueue;
    printf("FSM emergencia inicializada.\n");
}
