#ifndef DRIVERS_H
#define DRIVERS_H

#include "fsm.h"
#include "sensor_defs.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "sensor.h"
#include <stdbool.h>

//bool DatoValido = false;
//bool Incendio = false;



sensors_config_t devices[3];
QueueHandle_t datoValidoQueue, datosSensoresQueue, muestreoRapidoQueue, incendioQueue;


int checkStart_ON(fsm_t *this);
void Activa_Sensores(fsm_t *this);
int Deadline(fsm_t *this);
void Lectura_Sensores(fsm_t *this);
int LecturaFinalizadaOK(fsm_t *this);
void Send_Data(fsm_t *this);
int ReturnTrue(fsm_t *this);
void Apagar_Sensores(fsm_t *this);


#endif