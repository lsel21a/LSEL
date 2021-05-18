#include "esp_log.h"

#include "drivers_gps.h"
#include "config.h"

uart_port_t uart_num;

static const char* TAG = "drivers_gps";


void init_GPS(uart_port_t uart){
	uart_num = uart;
    uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_CTS_RTS,
        .rx_flow_ctrl_thresh = 122,
    };
    // Configuración de los parámetros del UART
    ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));

    // Pines UART (TX, RX, RTS, CTS)
    ESP_ERROR_CHECK(uart_set_pin(uart_num, 15, CONFIG_GPS_RX_PIN, 18, 19));

    // Se crea la cola para los datos de la UART
    static QueueHandle_t uart_queue;

    // Se instala el driver del UART
    ESP_ERROR_CHECK(uart_driver_install(uart_num, CONFIG_GPS_UART_RX_BUF_SIZE, CONFIG_GPS_UART_RX_BUF_SIZE, 10, &uart_queue, 0));
}

char *readLine(uart_port_t uart) {
	static char line[256];
	int size;
	char *ptr = line;
    // Se lee una línea de la trama recibida por el GPS
	while(1) {
		size = uart_read_bytes(uart_num, (unsigned char *)ptr, sizeof(char), portMAX_DELAY);
		if (size == sizeof(char)) {
			if (*ptr == '\n') {
				ptr++;
				*ptr = 0;
				return line;
			}
			ptr++;
		}
	}
}

void read_GPS(float *longitud, float *latitud) {
    char *line;
    struct minmea_sentence_gll frame;

    // Se lee el GPS hasta que se recibe GPGLL(Posición)
    do {
        line = readLine(uart_num);
        ESP_LOGD(TAG, "[read_GPS] Se lee una línea de la trama de GPS recibida.");
    } while(minmea_sentence_id(line, false) != MINMEA_SENTENCE_GLL);

    // Se decodifica la trama recibida
    if (minmea_parse_gll(&frame, line)) {
        *latitud = minmea_tocoord(&frame.latitude);
        *longitud = minmea_tocoord(&frame.longitude);
        ESP_LOGD(TAG, "[read_GPS] Se obtiene la posición de la baliza.");
    } else{
        ESP_LOGW(TAG, "[read_GPS] Fallo decodificación trama GPS.");
    }
}