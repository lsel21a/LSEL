
#include "fsm_deteccion_incendio.h"
#include "fsm_sensores.h"
#include "fsm.h"

static void fsm_sensor_task(void *arg)
{

  fsm_t* my_fsm = fsm_new (sensores_tt);
  
  while (1) {

    fsm_fire (my_fsm);

  }

  return;
}


static void fsm_deteccion_task(void *arg)
{

  fsm_t* my_fsm = fsm_new (DeteccionIncendio);
  
  while (1) {

    fsm_fire (my_fsm);

  }

  return;
}


void app_main() 
{
      xTaskCreate(fsm_sensor_task, "fsm_sensor_task", 1024, NULL, 8, NULL);
      xTaskCreate(fsm_deteccion_task, "fsm_sensor_task", 1024, NULL, 10, NULL);
}