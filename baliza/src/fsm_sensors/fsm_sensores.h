#ifndef _FSM_SENSORES_H
#define _FSM_SENSORES_H

#include "fsm.h"

enum{
    Idle, Medidas, Lectura, Calculos
};

void fsm_init_sensores(fsm_t *this);

#endif