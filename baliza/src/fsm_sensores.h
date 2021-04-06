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

void fsm_init_sensores(fsm_t *this); 

static fsm_trans_t sensores_tt[];


#endif  