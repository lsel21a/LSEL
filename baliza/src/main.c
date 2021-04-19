#include "fsm_sensors/fsm_sensores.h"
#include "fsm_fire_detection/fsm_deteccion_incendio.h"
#include "fsm_timer/fsm_timer.h"
#include "fsm_emergency/fsm_emergencia.h"

QueueHandle_t datoValidoQueue, datosSensoresQueue, tickQueue, incendioQueue, muestreoRapidoQueue, modoSilenciosoQueue;

static void fsm_sensor_task(void *arg)
{
  fsm_sensores_t f;
  fsm_init_sensores(&f, &datoValidoQueue, &datosSensoresQueue, &tickQueue);
  
  while (1) {

    fsm_fire ((fsm_t*)(&f));

  }

  return;
}


static void fsm_deteccion_task(void *arg)
{

  fsm_deteccion_incendio_t f;
  fsm_deteccion_incendio_init (&f, &datoValidoQueue, &datosSensoresQueue, &incendioQueue, &muestreoRapidoQueue);
  
  while (1) {

    fsm_fire ((fsm_t*)(&f));

  }

  return;
}

static void fsm_timer_task(void *arg)
{

  fsm_timer_t f;
  fsm_timer_init (&f, &muestreoRapidoQueue, &tickQueue);
  
  while (1) {

    fsm_fire ((fsm_t*)(&f));

  }

  return;
}

static void fsm_emergencia_task(void *arg)
{

  fsm_emergencia_t f;
  fsm_emergencia_init (&f, &incendioQueue, &modoSilenciosoQueue);
  
  while (1) {

    fsm_fire ((fsm_t*)(&f));

  }

  return;
}





void app_main() 
{
  BaseType_t rslt;
    
  // Creamos las colas
  datoValidoQueue = xQueueCreate(1, sizeof(bool));
  datosSensoresQueue = xQueueCreate(NUM_SENSORS, sizeof(sensors_data_t));
  tickQueue = xQueueCreate(1, sizeof(bool));
  incendioQueue = xQueueCreate(1, sizeof(bool));
  muestreoRapidoQueue = xQueueCreate(1, sizeof(bool));
  modoSilenciosoQueue = xQueueCreate(1, sizeof(bool));

  rslt = xTaskCreate(fsm_timer_task, "fsm_timer_task", 1024, NULL, 10, NULL);
  if(rslt == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY )
    printf("Lo vas a arreglar tú, Marcos!!!\n");

  rslt = xTaskCreate(fsm_emergencia_task, "fsm_emergencia_task", 1024, NULL, 10, NULL);
  if(rslt == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY )
    printf("Lo vas a arreglar tú, Marcos!!!\n");

  rslt = xTaskCreate(fsm_sensor_task, "fsm_sensor_task", 1024, NULL, 8, NULL);
  if(rslt == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY )
    printf("Lo vas a arreglar tú, Francesco!!!\n");
    
  rslt = xTaskCreate(fsm_deteccion_task, "fsm_sensor_task", 1024, NULL, 10, NULL);
  if(rslt == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY )
    printf("Lo vas a arreglar tú, Marcos!!!\n");
}