#ifndef _FSM_TIMER_H
#define _FSM_TIMER_H

#include <stdbool.h>
#include <stdio.h>
//#include <signal.h>

#include "fsm.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

enum {
  MUESTREO_NORMAL,
  MUESTREO_RAPIDO,
};

typedef struct  _fsm_timer_t {
  fsm_t fsm;
  QueueHandle_t *muestreoRapidoQueue, *tickQueue;
} fsm_timer_t ;

void  fsm_timer_init ( fsm_timer_t * this, QueueHandle_t *muestreoRapidoQueue, QueueHandle_t *tickQueue );

# endif 
