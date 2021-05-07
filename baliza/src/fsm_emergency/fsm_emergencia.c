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


void  fsm_emergencia_init ( fsm_emergencia_t * this, float *temperatura, float *humedad, float *gases, QueueHandle_t *incendioQueue, QueueHandle_t *solicitudDatosQueue, esp_mqtt_client_handle_t *client)
{
    fsm_init((fsm_t *)this, emergencia_tt);
    this->temperatura = temperatura;
    this->humedad = humedad;
    this->gases = gases;
    this->incendioQueue = incendioQueue;
    this->solicitudDatosQueue = solicitudDatosQueue;
    this->client = client;
    printf("FSM emergencia inicializada.\n");
};
