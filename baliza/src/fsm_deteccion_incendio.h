#ifndef _FSM_DETECCION_H
#define _FSM_DETECCION_H
#include <stddef.h>
#include <stdio.h>
//#include <signal.h>
#include "fsm.h"
#include "algoritmo.h"
#include "drivers.h"

typedef struct  _fsm_deteccion_incendio_t {
    fsm_t fsm;
    int *temperatura;
    int *humedad;
    int *gases;
    } fsm_deteccion_incendio_t ;


enum DeteccionIncendio_State{  

  IDLE_INCENDIO,
  DATA_SENSOR,
  POSIBLE_INCENDIO,
  DATA_SENSOR_POSIBLE_INCENDIO,
  INCENDIO,

};

void  fsm_deteccion_incendio_init ( fsm_deteccion_incendio_t * f, int *temperatura, int *humedad, int *gases);

static fsm_trans_t DeteccionIncendio[];

# endif 