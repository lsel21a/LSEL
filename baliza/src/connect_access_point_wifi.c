#include <freertos/FreeRTOS.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <esp_event.h>
#include <esp_event_loop.h>
#include <nvs_flash.h>
#include <tcpip_adapter.h>

esp_err_t event_handler(void *ctx, system_event_t *event)
{
	if(event->event_id == SYSTEM_EVENT_STA_GOT_IP)
	{
		printf("Nuestra dirección IP es " IPSTR "\n",
			IP2STR(&event->event_info.got_ip.ip_info.ip));
		printf("Nos hemos conectado a una estación\n");
	}
	if(event->event_id == SYSTEM_EVENT_STA_START)
	{
		ESP_ERROR_CHECK(esp_wifi_connect());
	}
		return ESP_OK;
	}
	
int app_main(void)
{
	nvs_flash_init();
	tcpip_adapter_init();
	ESP_ERROR_CHECK(esp_event_loop_init (event_handler, NULL));
	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_wifi_init(&cfg));
	ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM);
	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
	wifi_config_t sta_config = {
		.sta = {
		.ssid = "RASPBERRY",
		.password = "password",
		.bassid_set = 0
		}
	};
	ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &sta_config);
	ESP_ERROR_CHECK(esp_wifi_start());
		
	return 0;
}
