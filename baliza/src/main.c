#include "fsm_sensors/fsm_sensores.h"
#include "fsm_fire_detection/fsm_deteccion_incendio.h"
#include "fsm_timer/fsm_timer.h"
#include "fsm_emergency/fsm_emergencia.h"
#include "esp_log.h"
#include "mqtt_client.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"


#define XTASK_DELAY 10*portTICK_PERIOD_MS
#define TAREA_SEPARADAS
#define MQTT_URL "mqtt://home.ddns.mrrb.eu:1883"

esp_mqtt_client_handle_t client;
float *temperatura, *humedad, *gases;
QueueHandle_t datoValidoQueue, datosSensoresQueue, tickQueue, incendioQueue, muestreoRapidoQueue, solicitudDatosQueue, datosMQTTQueue;

static const char *TAG = "MQTT_EXAMPLE";


static void log_error_if_nonzero(const char *message, int error_code)
{
    if (error_code != 0) {
        ESP_LOGE(TAG, "Last error %s: 0x%x", message, error_code);
    }
}

/*
 * @brief Event handler registered to receive MQTT events
 *
 *  This function is called by the MQTT client event loop.
 *
 * @param handler_args user data registered to the event.
 * @param base Event base for the handler(always MQTT Base in this example).
 * @param event_id The id for the received event.
 * @param event_data The data for the event, esp_mqtt_event_handle_t.
 */
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
  ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
  esp_mqtt_event_handle_t event = event_data;

  switch ((esp_mqtt_event_id_t)event_id)
  {
  case MQTT_EVENT_CONNECTED:
    ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
    esp_mqtt_client_subscribe(client, "solicitudDatos", 0);
    break;
  case MQTT_EVENT_DISCONNECTED:
    ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
    break;
  case MQTT_EVENT_SUBSCRIBED:
    ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
    printf("sent subscribe successful");
    break;
  case MQTT_EVENT_UNSUBSCRIBED:
    ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
    break;
  case MQTT_EVENT_PUBLISHED:
    ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
    break;
  case MQTT_EVENT_DATA:
    ESP_LOGI(TAG, "MQTT_EVENT_DATA");
    if (solicitudDatosQueue != 0)
    {
#ifdef DEBUG_PRINT_ENABLE
      printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
      printf("DATA=%.*s\r\n", event->data_len, event->data);
#endif /*DEBUG_PRINT_ENABLE*/
      char to_compare[15];
      memcpy(to_compare, event->topic, event->topic_len);
      to_compare[14] = '\0';

      if (strcmp(to_compare, "solicitudDatos") == 0)
      {
        char to_compare2[5];
        memcpy(to_compare2, event->data, event->data_len);
        to_compare[4] = '\0';

        bool txSolicitudDatos = false;

        if (strcmp(to_compare2, "true") == 0)
        {
          txSolicitudDatos = true;
        }
#ifdef DEBUG_PRINT_ENABLE
        printf("Se envia la senal de solicitudDatos a la fsm_emergencia.\n");
#endif /* DEBUG_PRINT_ENABLE */

        // Send solicitudDatos MQTT data to fsm_emergencia.
        if (xQueueGenericSend(solicitudDatosQueue, (void *)&txSolicitudDatos, (TickType_t)0, queueSEND_TO_BACK) != pdPASS)
        {
          // Failed to post the message.
#ifdef DEBUG_PRINT_ENABLE
          printf("Error en enviar senal de solicitudDatos.\n");
#endif /* DEBUG_PRINT_ENABLE */
        }
        else
        {
#ifdef DEBUG_PRINT_ENABLE
          printf("Enviado senal de solicitudDatos = true.\n");
#endif /* DEBUG_PRINT_ENABLE */
        }
      }
    }
    else
    {
#ifdef DEBUG_PRINT_ENABLE
      printf("Error: queue of solicitudDatos is not correctly open.\n");
#endif /* DEBUG_PRINT_ENABLE */
    }
    break;
  /* case MQTT_EVENT_ERROR:
    ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
    if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT)
    {
      log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
      log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
      //log_error_if_nonzero("captured as transport's socket errno",  event->error_handle->esp_transport_sock_errno);
      //ESP_LOGI(TAG, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));
    }
    break; */
  default:
    ESP_LOGI(TAG, "Other event id:%d", event->event_id);
    break;
  }
}

static void mqtt_app_start()
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = MQTT_URL,
    };


    client = esp_mqtt_client_init(&mqtt_cfg);
    /* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);

}

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
  fsm_deteccion_incendio_init (&f, &datoValidoQueue, &datosSensoresQueue, &incendioQueue, &muestreoRapidoQueue, &datosMQTTQueue);
  
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
  fsm_emergencia_init (&f, &incendioQueue, &solicitudDatosQueue, &datosMQTTQueue, &client);
  
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
  ESP_LOGI(TAG, "[APP] Startup..");
  ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
  ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

  esp_log_level_set("*", ESP_LOG_INFO);
  esp_log_level_set("MQTT_CLIENT", ESP_LOG_VERBOSE);
  esp_log_level_set("MQTT_EXAMPLE", ESP_LOG_VERBOSE);
  esp_log_level_set("TRANSPORT_BASE", ESP_LOG_VERBOSE);
  esp_log_level_set("esp-tls", ESP_LOG_VERBOSE);
  esp_log_level_set("TRANSPORT", ESP_LOG_VERBOSE);
  esp_log_level_set("OUTBOX", ESP_LOG_VERBOSE);

  ESP_ERROR_CHECK(nvs_flash_init());
  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());

  /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
    * Read "Establishing Wi-Fi or Ethernet Connection" section in
    * examples/protocols/README.md for more information about this function.
    */
  ESP_ERROR_CHECK(example_connect());


  mqtt_app_start();  
  // Init I2C
  i2cdev_init();

  // Creamos las colas
  datoValidoQueue = xQueueCreate(1, sizeof(bool));
  datosSensoresQueue = xQueueCreate(1, NUM_SENSORS*sizeof(sensors_data_t));
  datosMQTTQueue = xQueueCreate(1, NUM_SENSORS*sizeof(sensors_data_t));
  muestreoRapidoQueue = xQueueCreate(1, sizeof(bool));
  tickQueue = xQueueCreate(1, sizeof(bool));
  incendioQueue = xQueueCreate(1, sizeof(bool));
  solicitudDatosQueue = xQueueCreate(1, sizeof(bool));
  muestreoRapidoQueue = xQueueCreate(1, sizeof(bool));

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
}