#ifndef DRIVERS_FSM_SENSORES_H
#define DRIVERS_FSM_SENSORES_H

#include <stdbool.h>

#include "fsm.h"
#include "sensors/sensor.h"

int checkStart_ON(fsm_t *this);
void Activa_Sensores(fsm_t *this);
int Deadline(fsm_t *this);
void Lectura_Sensores(fsm_t *this);
int LecturaFinalizadaOK(fsm_t *this);
void Send_Data(fsm_t *this);
int ReturnTrue(fsm_t *this);
void Apagar_Sensores(fsm_t *this);

#endif
