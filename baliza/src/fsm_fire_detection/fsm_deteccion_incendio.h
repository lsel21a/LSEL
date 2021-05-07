#ifndef _FSM_DETECCION_H
#define _FSM_DETECCION_H

#include <stddef.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "fsm.h"
#include "algorithm/algoritmo.h"
#include "drivers_fsm_deteccion_incendio.h"
#include "sensors/sensor_defs.h"


typedef struct _fsm_deteccion_incendio_t {
  fsm_t fsm;
  QueueHandle_t *datoValidoQueue, *datosSensoresQueue, *incendioQueue, *muestreoRapidoQueue;
  float *temperatura, *humedad, *gases;
} fsm_deteccion_incendio_t;


enum DeteccionIncendio_State{  
  IDLE_INCENDIO,
  DATA_SENSOR,
  POSIBLE_INCENDIO,
  DATA_SENSOR_POSIBLE_INCENDIO,
  INCENDIO,
};

void  fsm_deteccion_incendio_init ( fsm_deteccion_incendio_t * f, float *temperatura, float *humedad, float *gases, QueueHandle_t *datoValidoQueue, QueueHandle_t *datosSensoresQueue, QueueHandle_t *incendioQueue, QueueHandle_t *muestreoRapidoQueue);
# endif