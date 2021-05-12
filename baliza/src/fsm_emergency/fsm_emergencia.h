#ifndef _FSM_EMERGENCIA_H
#define _FSM_EMERGENCIA_H
#include <stdbool.h>
#include <stdio.h>
//#include <signal.h>
#include "fsm.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "mqtt_client.h"
#include "sensors/sensor_defs.h"

typedef struct  _fsm_emergencia_t {
    fsm_t fsm;
    QueueHandle_t *incendioQueue, *solicitudDatosQueue, *datosMQTTQueue;
    esp_mqtt_client_handle_t *client;
    } fsm_emergencia_t ;


enum emergencia_State{  

  IDLE_EMERGENCIA,
  EMERGENCIA,
  EMERGENCIA_SILENCIOSA,

};

void  fsm_emergencia_init ( fsm_emergencia_t *this, QueueHandle_t *incendioQueue, QueueHandle_t *solicitudDatosQueue, QueueHandle_t *datosMQTTQueue, esp_mqtt_client_handle_t *client);

# endif 