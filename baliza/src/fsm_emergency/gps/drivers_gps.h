#include "esp_log.h"
#include "driver/uart.h"
#include "../minmea/minmea.h"

void init_GPS(uart_port_t uart);
void read_GPS( float *longitud, float *latitud);
