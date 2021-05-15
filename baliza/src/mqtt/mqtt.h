#include "esp_log.h"
#include "mqtt_client.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"

void connect_WIFI();
void init_mqtt(QueueHandle_t *solicitudDatosQ);
void mqtt_app_start(esp_mqtt_client_handle_t **client);