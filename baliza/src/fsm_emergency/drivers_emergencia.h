#ifndef DRIVERS_EMERGENCIA_H
#define DRIVERS_EMERGENCIA_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "fsm_emergencia.h"
#include <stdbool.h>

int SolicitudDatos (fsm_t *this);
int SenalIncendio (fsm_t *this);
int SenalNoIncendio (fsm_t *this);
int SenalIncendioSil (fsm_t *this);
void SendDatos (fsm_t *this);
void EnvioSenalEmergencia (fsm_t *this);
void BackToIdle (fsm_t *this);
void NoSenalEmergencia (fsm_t *this);
void SendDatosYEmergencia (fsm_t* this);

#endif