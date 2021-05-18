#include "esp_log.h"

#include <stddef.h>
#include <stdio.h>
#include "mqtt_client.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "connect.h"

#include "mqtt.h"
#include "config.h"

static esp_mqtt_client_handle_t client_st;
static QueueHandle_t* p_solicitudDatosQueue = NULL;

static const char *TAG = "MQTT_DRIVER";


void init_mqtt(QueueHandle_t *solicitudDatosQ){
    p_solicitudDatosQueue = solicitudDatosQ;
}

void connect_WIFI(){

    ESP_LOGD(TAG, "[init_mqtt] Startup..");
    ESP_LOGD(TAG, "[init_mqtt] Free memory: %d bytes", esp_get_free_heap_size());
    ESP_LOGD(TAG, "[init_mqtt] IDF version: %s", esp_get_idf_version());

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    ESP_ERROR_CHECK(connect());

}

static void log_error_if_nonzero(const char *message, int error_code)
{
    if (error_code != 0) {
        ESP_LOGE(TAG, "[log_error_if_nonzero] Last error %s: 0x%x", message, error_code);
    }
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
  ESP_LOGD(TAG, "[mqtt_event_handler] Event dispatched from event loop base=%s, event_id=%d", base, event_id);
  esp_mqtt_event_handle_t event = event_data;

  switch ((esp_mqtt_event_id_t)event_id)
  {
  case MQTT_EVENT_CONNECTED:
    ESP_LOGD(TAG, "[mqtt_event_handler] MQTT_EVENT_CONNECTED");
    esp_mqtt_client_subscribe(client_st, CONFIG_MQTT_TOPIC_SOLICITUD, 0);
    break;

  case MQTT_EVENT_DISCONNECTED:
    ESP_LOGD(TAG, "[mqtt_event_handler] MQTT_EVENT_DISCONNECTED");
    break;

  case MQTT_EVENT_SUBSCRIBED:
    ESP_LOGD(TAG, "[mqtt_event_handler] MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
    ESP_LOGD(TAG, "[mqtt_event_handler] Sent subscribe successful");
    break;

  case MQTT_EVENT_UNSUBSCRIBED:
    ESP_LOGD(TAG, "[mqtt_event_handler] MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
    break;

  case MQTT_EVENT_PUBLISHED:
    ESP_LOGD(TAG, "[mqtt_event_handler] MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
    break;

  case MQTT_EVENT_DATA:
    ESP_LOGD(TAG, "[mqtt_event_handler] MQTT_EVENT_DATA");
    if (p_solicitudDatosQueue != NULL)
    {
      char to_compare[512];
  
      ESP_LOGI(TAG, "[mqtt_event_handler] Recv MQTT - TOPIC=%.*s", event->topic_len, event->topic);
      ESP_LOGI(TAG, "[mqtt_event_handler] Recv MQTT - DATA=%.*s", event->data_len, event->data);
  
      sprintf(to_compare, "%.*s", event->topic_len, event->topic);

      if (strcmp(to_compare, CONFIG_MQTT_TOPIC_SOLICITUD) == 0)
      {
        sprintf(to_compare, "%.*s", event->data_len, event->data);

        bool txSolicitudDatos = false;

        if (strcmp(to_compare, CONFIG_MQTT_TOPIC_SOLICITUD_MSG) == 0)
        {
          txSolicitudDatos = true;
        }
        ESP_LOGD(TAG, "[mqtt_event_handler] Se envia la senal de solicitudDatos a la fsm_emergencia.");

        // Send solicitudDatos MQTT data to fsm_emergencia.
        if (xQueueSend(*p_solicitudDatosQueue, (void *)&txSolicitudDatos, (TickType_t)0) != pdTRUE)
        {
          // Failed to post the message.
          ESP_LOGW(TAG, "[mqtt_event_handler] Error en enviar senal de solicitudDatos.");
        }
        else
        {
          ESP_LOGD(TAG, "[mqtt_event_handler] Enviado senal de solicitudDatos = true.");
        }
      }
    }
    else
    {
      ESP_LOGE(TAG, "[mqtt_event_handler] Queue of solicitudDatos is not correctly open.");
    }
    break;

  case MQTT_EVENT_ERROR: 
    ESP_LOGI(TAG, "[mqtt_event_handler] MQTT_EVENT_ERROR");
    if (event->error_handle->error_type != MQTT_ERROR_TYPE_NONE)
    {
      log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
      log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
    }
    break;

  default:
    ESP_LOGI(TAG, "[mqtt_event_handler] Other event id: %d", event->event_id);
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