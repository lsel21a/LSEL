#include <stdio.h>
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"

#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22
#define I2C_MASTER_FREQ_HZ 10000

uint8_t i2c_driver_init();
void check_rslt(int);
int i2c_send_data(uint8_t , uint8_t *, uint8_t , uint32_t );
int i2c_recv_data(uint8_t , uint8_t *, uint8_t , uint32_t );