#include "fsm_emergencia.h"
#include "drivers_emergencia.h"

// Explicit FSM description

static fsm_trans_t emergencia_tt[] = {
    {IDLE_EMERGENCIA, SenalIncendio , EMERGENCIA, EnvioSenalEmergencia},
    {IDLE_EMERGENCIA, SolicitudDatos , IDLE_EMERGENCIA, SendDatos},
    {IDLE_EMERGENCIA, SenalIncendioSil , EMERGENCIA_SILENCIOSA, NULL},
    {EMERGENCIA, SenalNoIncendio , IDLE_EMERGENCIA, NULL},
    {EMERGENCIA, SolicitudDatos , EMERGENCIA, SendDatos},
    {EMERGENCIA_SILENCIOSA, SenalNoIncendio , IDLE_EMERGENCIA, NULL},
    {EMERGENCIA_SILENCIOSA, SolicitudDatos , EMERGENCIA_SILENCIOSA, SendDatos},
    {-1, NULL, -1, NULL},

};


void  fsm_emergencia_init ( fsm_emergencia_t * this, QueueHandle_t *incendioQueue)
{
    fsm_init((fsm_t *)this, emergencia_tt);
    this->incendioQueue = incendioQueue;
#ifdef DEBUG_PRINT_ENABLE
    printf("FSM emergencia inicializada.\n");
#endif /* DEBUG_PRINT_ENABLE */
};
