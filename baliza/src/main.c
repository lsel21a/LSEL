#include "esp_log.h"

#include "fsm_sensors/fsm_sensores.h"
#include "fsm_fire_detection/fsm_deteccion_incendio.h"
#include "fsm_timer/fsm_timer.h"
#include "fsm_emergency/fsm_emergencia.h"
#include "fsm_emergency/gps/drivers_gps.h"
#include "mqtt/mqtt.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "config.h"

esp_mqtt_client_handle_t* p_client;
QueueHandle_t datoValidoQueue, datosSensoresQueue, tickQueue, incendioQueue, muestreoRapidoQueue, solicitudDatosQueue, datosMQTTQueue;

static const char* TAG = "main";

static void fsm_sensor_task(void *arg)
{
  fsm_sensores_t f;
  fsm_init_sensores(&f, &datoValidoQueue, &datosSensoresQueue, &tickQueue);
  
  while (1) {
    ESP_LOGD(TAG, "[fsm_sensor_task] Disparo de la FSM de sensores.");
    fsm_fire((fsm_t*)(&f));
    vTaskDelay(CONFIG_XTASK_DELAY);
  }
  return;
}


static void fsm_deteccion_task(void *arg)
{
  fsm_deteccion_incendio_t f;
  fsm_deteccion_incendio_init(&f, &datoValidoQueue, &datosSensoresQueue, &incendioQueue, &muestreoRapidoQueue, &datosMQTTQueue);
  
  while (1) {
    ESP_LOGD(TAG, "[fsm_deteccion_task] Disparo de la FSM de detección de incendio.");
    fsm_fire((fsm_t*)(&f));
    vTaskDelay(CONFIG_XTASK_DELAY);
  }
  return;
}

static void fsm_timer_task(void *arg)
{
  fsm_timer_t f;
  fsm_timer_init(&f, &muestreoRapidoQueue, &tickQueue);
  
  while (1) {
    ESP_LOGD(TAG, "[fsm_timer_task] Disparo de la FSM de timer.");
    fsm_fire((fsm_t*)(&f));
    vTaskDelay(CONFIG_XTASK_DELAY);
  }
  return;
}

static void fsm_emergencia_task(void *arg)
{
  fsm_emergencia_t f;
  fsm_emergencia_init(&f, &incendioQueue, &solicitudDatosQueue, &datosMQTTQueue, p_client);
  
  while (1) {
    ESP_LOGD(TAG, "[fsm_emergencia_task] Disparo de la FSM de emergencia.");
    fsm_fire ((fsm_t*)(&f));
    vTaskDelay(CONFIG_XTASK_DELAY);
  }
  return;
}

void app_main()
{
  BaseType_t rslt;

  // Set logs verbosity level
#ifdef DEBUG_PRINT_ENABLE
  esp_log_level_set("*", ESP_LOG_VERBOSE);
#else
  esp_log_level_set("*", ESP_LOG_INFO);
#endif /* DEBUG_PRINT_ENABLE */
  esp_log_level_set("i2cdev", ESP_LOG_INFO);
  esp_log_level_set("bme680", ESP_LOG_INFO);

  // Creamos las colas
  datoValidoQueue = xQueueCreate(1, sizeof(bool));
  datosSensoresQueue = xQueueCreate(1, CONFIG_SENSOR_NUM * sizeof(sensors_data_t));
  datosMQTTQueue = xQueueCreate(1, CONFIG_SENSOR_NUM * sizeof(sensors_data_t));
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
  mqtt_app_start(&p_client);  

  // Init I2C
  i2cdev_init();

  // Init GPS
  init_GPS(UART_NUM_2);

  // Creamos las tareas
  rslt = xTaskCreate(fsm_timer_task, "fsm_timer_task", 4096, NULL, 4, NULL);
  if(rslt == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY )
    ESP_LOGE(TAG, "[fsm_timer_task] Error allocating memory!");

  rslt = xTaskCreate(fsm_sensor_task, "fsm_sensor_task", 4096, NULL, 3, NULL);
  if(rslt == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY )
    ESP_LOGE(TAG, "[fsm_sensor_task] Error allocating memory!");

  rslt = xTaskCreate(fsm_deteccion_task, "fsm_deteccion_incendio_task", 4096, NULL, 2, NULL);
  if(rslt == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY )
    ESP_LOGE(TAG, "[fsm_deteccion_task] Error allocating memory!");

  rslt = xTaskCreate(fsm_emergencia_task, "fsm_emergencia_task", 4096, NULL, 1, NULL);
  if(rslt == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY )
    ESP_LOGE(TAG, "[fsm_emergencia_task] Error allocating memory!");
}
