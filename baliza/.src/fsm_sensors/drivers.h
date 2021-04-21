#ifndef DRIVERS_H
#define DRIVERS_H

#include "fsm.h"

int checkStart_ON(fsm_t *this);
void Activa_Sensores(fsm_t *this);
int Deadline(fsm_t *this);
void Lectura_Sensores(fsm_t *this);
int LecturaFinalizadaOK(fsm_t *this);
void Dato_Valido(fsm_t *this);
void Dato_NoValido(fsm_t *this);
int ReturnTrue(fsm_t *this);

#endif