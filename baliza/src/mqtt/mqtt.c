#include "esp_log.h"
#include "mqtt_client.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"

#include "mqtt.h"
#include "config.h"

static const char *TAG = "MQTT_EXAMPLE";
static esp_mqtt_client_handle_t client_st;
QueueHandle_t *solicitudDatosQueue;

void init_mqtt(QueueHandle_t *solicitudDatosQ){
    solicitudDatosQueue = solicitudDatosQ;
}

void connect_WIFI(){

#ifdef DEBUG_PRINT_ENABLE
    ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());
#endif

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

}

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
#ifdef DEBUG_PRINT_ENABLE
    ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
#endif
    esp_mqtt_client_subscribe(client_st, CONFIG_MQTT_TOPIC_SOLICITUD, 0);
    break;
  case MQTT_EVENT_DISCONNECTED:
#ifdef DEBUG_PRINT_ENABLE
    ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
#endif
    break;
  case MQTT_EVENT_SUBSCRIBED:
#ifdef DEBUG_PRINT_ENABLE
    ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
    printf("sent subscribe successful");
#endif
    break;
  case MQTT_EVENT_UNSUBSCRIBED:
#ifdef DEBUG_PRINT_ENABLE
    ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
#endif
    break;
  case MQTT_EVENT_PUBLISHED:
#ifdef DEBUG_PRINT_ENABLE
    ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
#endif
    break;
  case MQTT_EVENT_DATA:
#ifdef DEBUG_PRINT_ENABLE
    ESP_LOGI(TAG, "MQTT_EVENT_DATA");
#endif
    if (solicitudDatosQueue != 0)
    {
#ifdef DEBUG_PRINT_ENABLE
      printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
      printf("DATA=%.*s\r\n", event->data_len, event->data);
#endif /*DEBUG_PRINT_ENABLE*/
      char to_compare[15];
      memcpy(to_compare, event->topic, event->topic_len);
      to_compare[14] = '\0';

      if (strcmp(to_compare, CONFIG_MQTT_TOPIC_SOLICITUD) == 0)
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
        if (xQueueGenericSend(*solicitudDatosQueue, (void *)&txSolicitudDatos, (TickType_t)0, queueSEND_TO_BACK) != pdPASS)
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
  case MQTT_EVENT_ERROR: 
#ifdef DEBUG_PRINT_ENABLE
    ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
#endif
    if (event->error_handle->error_type != MQTT_ERROR_TYPE_NONE)
    {
      log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
      log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
    }
    break;
  default:
#ifdef DEBUG_PRINT_ENABLE
    ESP_LOGI(TAG, "Other event id:%d", event->event_id);
#endif
    break;
  }
}

void mqtt_app_start(esp_mqtt_client_handle_t **client)
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = CONFIG_MQTT_URL,
    };


    // Inicialización del cliente
    client_st = esp_mqtt_client_init(&mqtt_cfg);
    *client = &(client_st);

    /* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
    esp_mqtt_client_register_event(client_st, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client_st);

}