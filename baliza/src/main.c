#include "fsm_sensors/fsm_sensores.h"
#include "fsm_fire_detection/fsm_deteccion_incendio.h"
#include "fsm_timer/fsm_timer.h"
#include "fsm_emergency/fsm_emergencia.h"
#include "fsm_emergency/gps/drivers_gps.h"
#include "mqtt/mqtt.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#define XTASK_DELAY 10*portTICK_PERIOD_MS
#define TAREA_SEPARADAS
#define UART_PORT UART_NUM_2

esp_mqtt_client_handle_t *client;
float *temperatura, *humedad, *gases;
QueueHandle_t datoValidoQueue, datosSensoresQueue, tickQueue, incendioQueue, muestreoRapidoQueue, solicitudDatosQueue, datosMQTTQueue;

#ifdef TAREA_SEPARADAS
static void fsm_sensor_task(void *arg)
{
  fsm_sensores_t f;
  fsm_init_sensores(&f, &datoValidoQueue, &datosSensoresQueue, &tickQueue);
  
  while (1) {
#ifdef DEBUG_PRINT_ENABLE
    printf("Disparo de la FSM de sensores.\n");
#endif
    fsm_fire ((fsm_t*)(&f));
    vTaskDelay(XTASK_DELAY);
  }
  return;
}


static void fsm_deteccion_task(void *arg)
{

  fsm_deteccion_incendio_t f;
  fsm_deteccion_incendio_init (&f, &datoValidoQueue, &datosSensoresQueue, &incendioQueue, &muestreoRapidoQueue, &datosMQTTQueue);
  
  while (1) {
#ifdef DEBUG_PRINT_ENABLE
    printf("Disparo de la FSM de detección de incendio.\n");
#endif /* DEBUG_PRINT_ENABLE */
    fsm_fire ((fsm_t*)(&f));
    vTaskDelay(XTASK_DELAY);
  }
  return;
}

static void fsm_timer_task(void *arg)
{

  fsm_timer_t f;
  fsm_timer_init (&f, &muestreoRapidoQueue, &tickQueue);
  
  while (1) {
#ifdef DEBUG_PRINT_ENABLE
    printf("Disparo de la FSM de timer.\n");
#endif /* DEBUG_PRINT_ENABLE */
    fsm_fire ((fsm_t*)(&f));
    vTaskDelay(XTASK_DELAY);
  }
  return;
}

static void fsm_emergencia_task(void *arg)
{

  fsm_emergencia_t f;
  fsm_emergencia_init (&f, &incendioQueue, &solicitudDatosQueue, &datosMQTTQueue, client);
  
  while (1) {
#ifdef DEBUG_PRINT_ENABLE
    printf("Disparo de la FSM de emergencia.\n");
#endif /* DEBUG_PRINT_ENABLE */
    fsm_fire ((fsm_t*)(&f));
    vTaskDelay(XTASK_DELAY);
  }
  return;
}
#endif

void app_main() {

  // Creamos las colas
  datoValidoQueue = xQueueCreate(1, sizeof(bool));
  datosSensoresQueue = xQueueCreate(1, NUM_SENSORS*sizeof(sensors_data_t));
  datosMQTTQueue = xQueueCreate(1, NUM_SENSORS*sizeof(sensors_data_t));
  muestreoRapidoQueue = xQueueCreate(1, sizeof(bool));
  tickQueue = xQueueCreate(1, sizeof(bool));
  incendioQueue = xQueueCreate(1, sizeof(bool));
  solicitudDatosQueue = xQueueCreate(1, sizeof(bool));
  muestreoRapidoQueue = xQueueCreate(1, sizeof(bool));
  
  // Connect WIFI
  connect_WIFI();

  // Init MQTT
  init_mqtt(&solicitudDatosQueue);

  // Start MQTT
  mqtt_app_start(&client);  

  // Init I2C
  i2cdev_init();

  // Init GPS
  init_GPS(UART_PORT);

#ifdef TAREA_SEPARADAS
  // Creamos las tareas
  BaseType_t rslt;
  
  rslt = xTaskCreate(fsm_timer_task, "fsm_timer_task", 4096, NULL, 4, NULL);
  if(rslt == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY )
    printf("Error in allocating memory!\n");

  rslt = xTaskCreate(fsm_sensor_task, "fsm_sensor_task", 4096, NULL, 3, NULL);
  if(rslt == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY )
    printf("Error in allocating memory!\n");

  rslt = xTaskCreate(fsm_deteccion_task, "fsm_deteccion_incendio_task", 4096, NULL, 2, NULL);
  if(rslt == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY )
    printf("Error in allocating memory!\n");

  rslt = xTaskCreate(fsm_emergencia_task, "fsm_emergencia_task", 4096, NULL, 1, NULL);
  if(rslt == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY )
    printf("Error in allocating memory!\n");

#else
  fsm_deteccion_incendio_t f_inc;
  fsm_deteccion_incendio_init (&f, &datoValidoQueue, &datosSensoresQueue, &incendioQueue, &muestreoRapidoQueue, &datosMQTTQueue);

  fsm_sensores_t f_sen;
  fsm_init_sensores(&f, &datoValidoQueue, &datosSensoresQueue, &tickQueue);

  fsm_timer_t f_timer;
  fsm_timer_init (&f, &muestreoRapidoQueue, &tickQueue);

  fsm_emergencia_t f_emer;
  fsm_emergencia_init (&f, &incendioQueue, &solicitudDatosQueue, &datosMQTTQueue, client);
  

  while(1){
    printf("Disparo de la FSM de timer.\n");
    fsm_fire ((fsm_t*)(&f_timer));

    printf("Disparo de la FSM de sensores.\n");
    fsm_fire ((fsm_t*)(&f_sen));

    printf("Disparo de la FSM de detección de incendio.\n");
    fsm_fire ((fsm_t*)(&f_inc));

    printf("Disparo de la FSM de emergencia.\n");
    fsm_fire ((fsm_t*)(&f_emer));
 }
#endif
}