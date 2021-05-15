#include "esp_log.h"
#include "driver/uart.h"
#include "../minmea/minmea.h"

#define GPS_RX_PIN (13)
#define UART_RX_BUF_SIZE 1024

void init_GPS(uart_port_t uart);
void read_GPS( float *longitud, float *latitud);
