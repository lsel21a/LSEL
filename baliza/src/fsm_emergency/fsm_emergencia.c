#include "fsm_emergencia.h"
#include "drivers_emergencia.h"

// Explicit FSM description

static fsm_trans_t emergencia_tt[] = {
    {IDLE_EMERGENCIA, SolicitudDatos , IDLE_EMERGENCIA, SendDatos},
    {IDLE_EMERGENCIA, SenalIncendio , EMERGENCIA, EnvioSenalEmergencia},
    {EMERGENCIA, SolicitudDatos , EMERGENCIA, SendDatos},
    {EMERGENCIA, SenalNoIncendio , IDLE_EMERGENCIA, NULL},
    {IDLE_EMERGENCIA, SenalIncendioSil , EMERGENCIA_SILENCIOSA, NoSenalEmergencia},
    {EMERGENCIA_SILENCIOSA, SolicitudDatos , EMERGENCIA_SILENCIOSA, SendDatos},
    {EMERGENCIA_SILENCIOSA, SenalNoIncendio , IDLE_EMERGENCIA, NULL},    
    {-1, NULL, -1, NULL},

};


void  fsm_emergencia_init ( fsm_emergencia_t * this, QueueHandle_t *modoSilenciosoQueue, QueueHandle_t *incendioQueue)
{
    fsm_init((fsm_t *)this, emergencia_tt);
    this->modoSilenciosoQueue = modoSilenciosoQueue;
    this->incendioQueue = incendioQueue;
    printf("FSM emergencia inicializada.\n");
}
