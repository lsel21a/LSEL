#include "esp_log.h"

#include "fsm_emergencia.h"
#include "drivers_emergencia.h"
#include "mqtt_client.h"

static const char* TAG = "fsm_emergencia";


// Explicit FSM description
static fsm_trans_t emergencia_tt[] = {
    {IDLE_EMERGENCIA, SenalIncendio , EMERGENCIA, EnvioSenalEmergencia},
    {IDLE_EMERGENCIA, SolicitudDatos , IDLE_EMERGENCIA, SendDatos},
    {IDLE_EMERGENCIA, SenalIncendioSil , EMERGENCIA_SILENCIOSA, NULL},
    {EMERGENCIA, SenalNoIncendio , IDLE_EMERGENCIA, NULL},
    {EMERGENCIA, SolicitudDatos , EMERGENCIA, SendDatosYEmergencia},
    {EMERGENCIA_SILENCIOSA, SenalNoIncendio , IDLE_EMERGENCIA, NULL},
    {EMERGENCIA_SILENCIOSA, SolicitudDatos , EMERGENCIA_SILENCIOSA, SendDatosYEmergencia},
    {-1, NULL, -1, NULL},

};

void  fsm_emergencia_init ( fsm_emergencia_t * this, QueueHandle_t *incendioQueue, QueueHandle_t *solicitudDatosQueue, QueueHandle_t *datosMQTTQueue, esp_mqtt_client_handle_t *client)
{
    fsm_init((fsm_t *)this, emergencia_tt);
    this->incendioQueue = incendioQueue;
    this->solicitudDatosQueue = solicitudDatosQueue;
    this->datosMQTTQueue = datosMQTTQueue;
    this->client = client;
    ESP_LOGD(TAG, "[fsm_emergencia_init] FSM emergencia inicializada.");
};
