#ifndef DRIVERS_FSM_DET_INC_H
#define DRIVERS_FSM_DET_INC_H

#include "fsm.h"

int ReceiveDatoValido (fsm_t* this);
int ReceiveDatoValidoIncendio (fsm_t* this);
int NoHayPeligro (fsm_t * this);
int PuedeSerIncendio (fsm_t* this);
int HayIncendio (fsm_t* this);
int Incendio_to_Idle (fsm_t* this);
void GetDataFromFsmSensor (fsm_t* this);
void BackToIdle (fsm_t* this);
void GetMuestreoRapido (fsm_t* this);
void SendDatoIncendio (fsm_t* this);

#endif
