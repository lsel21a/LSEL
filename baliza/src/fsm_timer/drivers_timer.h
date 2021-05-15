#ifndef DRIVERS_TIMER_H
#define DRIVERS_TIMER_H

#include <stdbool.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "fsm.h"


int ReceiveMuestreoRapido (fsm_t* this);
int WaitNormal (fsm_t* this);
int ReceiveMuestreoNormal (fsm_t* this);
int WaitRapido (fsm_t* this);
void SetMuestreoRapido (fsm_t* this);
void SetMuestreoNormal (fsm_t* this);
void SendTick (fsm_t* this);

#endif