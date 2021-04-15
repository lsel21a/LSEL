#ifndef DRIVERS_EMERGENCIA_H
#define DRIVERS_EMERGENCIA_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "fsm_emergencia.h"
#include <stdbool.h>

#define MODO_SILENCIOSO 0

static int SolicitudDatos (fsm_t *this);
static int SeñalIncendio (fsm_t *this);
static int SeñalNoIncendio (fsm_t *this);
static int SeñalIncendioSil (fsm_t *this);
static void SendDatos (fsm_t *this);
static void EnvioSeñalEmergencia (fsm_t *this);
static void BackToIdle (fsm_t *this);
static void NoSeñalEmergencia (fsm_t *this);


#endif