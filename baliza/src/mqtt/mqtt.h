#ifndef _MQTT_H
#define _MQTT_H

void connect_WIFI();
void init_mqtt(QueueHandle_t *solicitudDatosQ);
void mqtt_app_start(esp_mqtt_client_handle_t **client);

#endif /* _MQTT_H */
