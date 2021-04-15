#ifndef DRIVERS_TIMER_H
#define DRIVERS_TIMER_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "fsm_timer.h"
#include <stdbool.h>


static int ReceiveMuestreoRapido (fsm_t *this);
static int WaitNormal (fsm_t* this);
static int ReceiveMuestreoNormal (fsm_t* this);
static int WaitRapido (fsm_t* this);
static void SetMuestreoRapido (fsm_t* this);
static void SetMuestreoNormal (fsm_t* this);
static void SendTick (fsm_t* this);


#endif