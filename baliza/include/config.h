#ifndef _CONFIG_H
#define _CONFIG_H

// Baliza ID
#define CONFIG_BALIZA_ID 112

// WIFI
#define CONFIG_ENABLE_WIFI 1

#define CONFIG_WIFI_SSID "POCO_F1"
#define CONFIG_WIFI_PASS "9ccd57df57e5"

#define CONFIG_WIFI_AUTH_WPA2_PSK 1

// MQTT
#define CONFIG_MQTT_URL "mqtt://home.ddns.mrrb.eu:1883"

#define CONFIG_MQTT_TOPIC_PREFIX "baliza"

#define CONFIG_MQTT_TOPIC_SOLICITUD CONFIG_MQTT_TOPIC_PREFIX"/solicitudDatos"

#define CONFIG_MQTT_TOPIC_INCENDIO "incendio"
#define CONFIG_MQTT_TOPIC_TEMPERATURA "Datos_Baliza/temperatura"
#define CONFIG_MQTT_TOPIC_HUMEDAD "Datos_Baliza/humedad"
#define CONFIG_MQTT_TOPIC_GASES "Datos_Baliza/gases"
#define CONFIG_MQTT_TOPIC_PRESION "Datos_Baliza/presion"
#define CONFIG_MQTT_TOPIC_GPS "Datos_Baliza/posicion"

// FSM emergencia
#define CONFIG_EMERGENCIA_MODO_SILENCIOSO 0

// FSM timer
#define CONFIG_TIMER_MEASURE_NORMAL 4 /* Segundos */
#define CONFIG_TIMER_MEASURE_QUICK 1 /* Segundos */

// Algoritmo
#define CONFIG_ALGO_TMAX 65
#define CONFIG_ALGO_GMAX 74
#define CONFIG_ALGO_HMIN 20

// GPS
#define CONFIG_GPS_RX_PIN (13)
#define CONFIG_GPS_UART_RX_BUF_SIZE 1024

// Sensors
#define CONFIG_SENSOR_NUM 1
#define CONFIG_SENSORS_BME68X_I2C_ADDR BME680_I2C_ADDR_0
#define CONFIG_SENSORS_AMBIENT_TEMP 25
#define CONFIG_SENSOR_SDA_GPIO 21
#define CONFIG_SENSOR_SCL_GPIO 22
#define CONFIG_SENSOR_PORT 0

// FreeRTOS
#define CONFIG_XTASK_DELAY 10*portTICK_PERIOD_MS

#endif /* _CONFIG_H */
