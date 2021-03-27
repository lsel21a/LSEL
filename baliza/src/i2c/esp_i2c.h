#ifndef ESP_I2C_H
#define ESP_I2C_H

#include <stdio.h>
#include "driver/i2c.h"
#include "drivers_esp.h"
#include "freertos/FreeRTOS.h"

#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22
#define I2C_PORT I2C_NUM_0
#define I2C_MASTER_FREQ_HZ 1000000

uint8_t i2c_init();
uint8_t i2c_detect();
uint8_t i2c_send_data(uint8_t dev_addr, uint8_t *data, uint8_t data_size, uint32_t timeout);
uint8_t i2c_recv_data(uint8_t dev_addr, uint8_t *data, uint8_t data_size, uint32_t timeout);
uint8_t i2c_read_data(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint8_t data_size, uint32_t timeout);
#endif