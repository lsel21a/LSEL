#include <stddef.h>

#include "esp_log.h"

#include "fsm_sensores.h"
#include "drivers_fsm_sensores.h"

static const char* TAG = "fsm_sensores";


static fsm_trans_t sensores_tt[] = {
    {IDLE, checkStart_ON, MEDIDAS, Activa_Sensores},
    {MEDIDAS, Deadline, LECTURA, Lectura_Sensores},
    {LECTURA, LecturaFinalizadaOK, VALIDACION, Send_Data},
    {VALIDACION, ReturnTrue, IDLE, Apagar_Sensores},
    {-1, NULL, -1, NULL}
};

void fsm_init_sensores(fsm_sensores_t *this, QueueHandle_t *datoValidoQueue, QueueHandle_t *datosSensoresQueue, QueueHandle_t *tickQueue){

    fsm_init((fsm_t *)this, sensores_tt);
    this->datosSensoresQueue = datosSensoresQueue;
    this->datoValidoQueue = datoValidoQueue;
    this->tickQueue = tickQueue;

    ESP_LOGD(TAG, "[fsm_init_sensores] FSM sensores inicializada.");
}


