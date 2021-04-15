
#include "fsm_deteccion_incendio.h"
#include "fsm_sensores.h"
#include "fsm.h"

static void fsm_sensor_task(void *arg)
{
  fsm_t f;

  fsm_init_sensores(&f); 

//  fsm_t* my_fsm = fsm_new (sensores_tt);
  
  while (1) {

    fsm_fire ((fsm_t*)(&f));

  }

  return;
}


static void fsm_deteccion_task(void *arg)
{

  fsm_deteccion_incendio_t f;

  fsm_deteccion_incendio_init ( &f, NULL, NULL, NULL);

//  fsm_t* my_fsm = fsm_new (sensores_tt);
  
  while (1) {

    fsm_fire ((fsm_t*)(&f));

  }

  return;
}


void app_main() 
{
      xTaskCreate(fsm_sensor_task, "fsm_sensor_task", 1024, NULL, 8, NULL);
      xTaskCreate(fsm_deteccion_task, "fsm_sensor_task", 1024, NULL, 10, NULL);
}