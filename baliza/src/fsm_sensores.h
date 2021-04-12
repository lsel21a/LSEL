#ifndef _FSM_SENSORES_H
#define _FSM_SENSORES_H

#include "fsm.h"
#include "sensor_defs.h"
#include <stdbool.h>

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