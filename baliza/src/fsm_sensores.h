#ifndef _FSM_SENSORES_H
#define _FSM_SENSORES_H

#include "fsm.h"


#define TRUE 5
#define FALSE 0

enum{
    Idle, Medidas, Lectura, Calculos
};

void fsm_init_sensores(fsm_t *this);
int checkStart_ON(fsm_t *this);
void Activa_Sensores(fsm_t *this);
void Lectura_Sensores(fsm_t *this);
void Dato_Valido(fsm_t *this);
void Dato_NoValido(fsm_t *this);
int Return_Always_True(fsm_t *this);

#endif