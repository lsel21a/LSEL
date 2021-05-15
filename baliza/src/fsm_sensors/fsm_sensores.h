#ifndef FSM_SENSORES_H
#define FSM_SENSORES_H

#include <stdbool.h>

#include "fsm.h"

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

enum{
    IDLE,
    MEDIDAS,
    LECTURA,
    VALIDACION
};

typedef struct _fsm_sensores{
    fsm_t fsm;
    QueueHandle_t *datoValidoQueue, *datosSensoresQueue, *tickQueue;
}fsm_sensores_t;

void fsm_init_sensores(fsm_sensores_t *this, QueueHandle_t *datoValidoQueue, QueueHandle_t *datosSensoresQueue, QueueHandle_t *tickQueue); 

#endif