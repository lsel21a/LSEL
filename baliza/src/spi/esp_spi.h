#ifndef SPI_ESP_H
#define SPI_ESP_H

#include <stdio.h>
#include "driver/spi_master.h"
#include "drivers_esp.h"
#include "freertos/FreeRTOS.h"

#define SPI_MISO_PIN 17     // Hay que revisar puertos porque estos puieden no ser correctos. No encuentro datasheet!!!
#define SPI_MOSI_PIN 27
#define SPI_SCLK_PIN 15
#define SPI_CS_PIN 12
#define SPI_MASTER_FREQ_HZ 10000
#define BME_HOST SPI2_HOST
#define DMA_CHAN 1


uint8_t spi_init();
uint8_t spi_send_data(spi_device_handle_t *spi, uint8_t *data, uint8_t data_size);
uint8_t spi_recv_data(spi_device_handle_t *spi, uint8_t *data, uint8_t data_size);
uint8_t spi_read_data(spi_device_handle_t *spi, uint8_t reg_addr, uint8_t *data, uint8_t data_size);
#endif