#ifndef _FSM_EMERGENCIA_H
#define _FSM_EMERGENCIA_H
#include <stdbool.h>
#include <stdio.h>
//#include <signal.h>
#include "fsm.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

typedef struct  _fsm_emergencia_t {
    fsm_t fsm;
    QueueHandle_t *incendioQueue;
    } fsm_emergencia_t ;


enum emergencia_State{  

  IDLE_EMERGENCIA,
  EMERGENCIA,
  EMERGENCIA_SILENCIOSA,

};

void  fsm_emergencia_init ( fsm_emergencia_t *this, QueueHandle_t *incendioQueue);

# endif 