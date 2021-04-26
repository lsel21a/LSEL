<<<<<<< HEAD
#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include <bme680.h>
#include <string.h>

#define SDA_GPIO 13
#define SCL_GPIO 14
#define PORT 0
#define ADDR BME680_I2C_ADDR_0

#if defined(CONFIG_IDF_TARGET_ESP32S2)
#define APP_CPU_NUM PRO_CPU_NUM
#endif

void bme680_test(void *pvParameters)
{
    bme680_t sensor;
    memset(&sensor, 0, sizeof(bme680_t));

    ESP_ERROR_CHECK(bme680_init_desc(&sensor, ADDR, PORT, SDA_GPIO, SCL_GPIO));

    // init the sensor
    ESP_ERROR_CHECK(bme680_init_sensor(&sensor));

    // Changes the oversampling rates to 4x oversampling for temperature
    // and 2x oversampling for humidity. Pressure measurement is skipped.
    bme680_set_oversampling_rates(&sensor, BME680_OSR_4X, BME680_OSR_NONE, BME680_OSR_2X);

    // Change the IIR filter size for temperature and pressure to 7.
    bme680_set_filter_size(&sensor, BME680_IIR_SIZE_7);

    // Change the heater profile 0 to 200 degree Celsius for 100 ms.
    bme680_set_heater_profile(&sensor, 0, 200, 100);
    bme680_use_heater_profile(&sensor, 0);

    // Set ambient temperature to 10 degree Celsius
    bme680_set_ambient_temperature(&sensor, 10);

    // as long as sensor configuration isn't changed, duration is constant
    uint32_t duration;
    bme680_get_measurement_duration(&sensor, &duration);

    TickType_t last_wakeup = xTaskGetTickCount();

    bme680_values_float_t values;
    while (1)
    {
        // trigger the sensor to start one TPHG measurement cycle
        if (bme680_force_measurement(&sensor) == ESP_OK)
        {
            // passive waiting until measurement results are available
            vTaskDelay(duration);

            // get the results and do something with them
            if (bme680_get_results_float(&sensor, &values) == ESP_OK)
                printf("BME680 Sensor: %.2f °C, %.2f %%, %.2f hPa, %.2f Ohm\n",
                        values.temperature, values.humidity, values.pressure, values.gas_resistance);
        }
        // passive waiting until 1 second is over
        vTaskDelayUntil(&last_wakeup, 1000 / portTICK_PERIOD_MS);
    }
}

void app_main()
{
    ESP_ERROR_CHECK(i2cdev_init());
    xTaskCreatePinnedToCore(bme680_test, "bme680_test", configMINIMAL_STACK_SIZE * 8, NULL, 5, NULL, APP_CPU_NUM);
=======
#include "fsm_sensors/fsm_sensores.h"
#include "fsm_fire_detection/fsm_deteccion_incendio.h"
#include "fsm_timer/fsm_timer.h"
#include "fsm_emergency/fsm_emergencia.h"

#define XTASK_DELAY 10*portTICK_PERIOD_MS
#define TAREA_SEPARADAS

QueueHandle_t datoValidoQueue, datosSensoresQueue, tickQueue, incendioQueue, muestreoRapidoQueue;

#ifdef TAREA_SEPARADAS
static void fsm_sensor_task(void *arg)
{
  fsm_sensores_t f;
  fsm_init_sensores(&f, &datoValidoQueue, &datosSensoresQueue, &tickQueue);
  
  while (1) {
    printf("Disparo de la FSM de sensores.\n");
    fsm_fire ((fsm_t*)(&f));
    vTaskDelay(XTASK_DELAY);
  }
  return;
}


static void fsm_deteccion_task(void *arg)
{

  fsm_deteccion_incendio_t f;
  fsm_deteccion_incendio_init (&f, &datoValidoQueue, &datosSensoresQueue, &incendioQueue, &muestreoRapidoQueue);
  
  while (1) {
    printf("Disparo de la FSM de detección de incendio.\n");
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
    printf("Disparo de la FSM de timer.\n");
    fsm_fire ((fsm_t*)(&f));
    vTaskDelay(XTASK_DELAY);
  }
  return;
}

static void fsm_emergencia_task(void *arg)
{

  fsm_emergencia_t f;
  fsm_emergencia_init (&f, &incendioQueue);
  
  while (1) {
    printf("Disparo de la FSM de emergencia.\n");
    fsm_fire ((fsm_t*)(&f));
    vTaskDelay(XTASK_DELAY);
  }
  return;
}
#endif

void app_main() 
{   
  // Init I2C
  i2cdev_init();

  // Creamos las colas
  datoValidoQueue = xQueueCreate(1, sizeof(bool));
  datosSensoresQueue = xQueueCreate(1, NUM_SENSORS*sizeof(sensors_data_t));
  tickQueue = xQueueCreate(1, sizeof(bool));
  incendioQueue = xQueueCreate(1, sizeof(bool));
  muestreoRapidoQueue = xQueueCreate(1, sizeof(bool));

#ifdef TAREA_SEPARADAS
  // Creamos las tareas
  BaseType_t rslt;
  
  rslt = xTaskCreate(fsm_timer_task, "fsm_timer_task", 4096, NULL, 4, NULL);
  if(rslt == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY )
    printf("Lo vas a arreglar tú, Marcos!!!\n");

  rslt = xTaskCreate(fsm_sensor_task, "fsm_sensor_task", 4096, NULL, 3, NULL);
  if(rslt == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY )
    printf("Lo vas a arreglar tú, Francesco!!!\n");

  rslt = xTaskCreate(fsm_deteccion_task, "fsm_deteccion_incendio_task", 4096, NULL, 2, NULL);
  if(rslt == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY )
    printf("Lo vas a arreglar tú, Marcos!!!\n");

  rslt = xTaskCreate(fsm_emergencia_task, "fsm_emergencia_task", 4096, NULL, 1, NULL);
  if(rslt == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY )
    printf("Lo vas a arreglar tú, Marcos!!!\n");
#endif

#ifndef TAREA_SEPARADAS
  fsm_deteccion_incendio_t f_inc;
  fsm_deteccion_incendio_init (&f_inc, &datoValidoQueue, &datosSensoresQueue, &incendioQueue, &muestreoRapidoQueue);

  fsm_sensores_t f_sen;
  fsm_init_sensores(&f_sen, &datoValidoQueue, &datosSensoresQueue, &tickQueue);

  fsm_timer_t f_timer;
  fsm_timer_init (&f_timer, &muestreoRapidoQueue, &tickQueue);

  fsm_emergencia_t f_emer;
  fsm_emergencia_init (&f_emer, &incendioQueue);
  

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
>>>>>>> origin/fsm_juntas
}