/*
 * \file sensors_hal.c
 * \brief 
 * \version 0.1
 * \date 2021-03-03
 */

#include "sensor_hal.h"
#include "sensor_defs.h"
#include "i2c/esp_i2c.h"
#include "spi/esp_spi.h"
#include "bme68x.h"


sensors_status_t
sensors_hal_init(void) {
    i2c_init();
    // spi_init(&spi);
    return SENSORS_OK;
}


BME68X_INTF_RET_TYPE
bme68x_i2c_read(uint8_t reg_addr, uint8_t* reg_data, uint32_t length, void* intf_ptr) {
    // Se contruye la trama que hay que enviar al sensor
    // Para más información consultar datasheet de BME680
    uint8_t dev_addr = *(uint8_t*)intf_ptr;
    return i2c_read_data(dev_addr, reg_addr, reg_data, (uint16_t)length, 20) != ESP_OK;
    // return BME68X_INTF_RET_SUCCESS;
}


BME68X_INTF_RET_TYPE
bme68x_i2c_write(uint8_t reg_addr, const uint8_t* reg_data, uint32_t length, void* intf_ptr) {
    uint8_t dev_addr = *(uint8_t*)intf_ptr;

    // Se contruye la trama que hay que enviar al sensor
    // Para más información consultar datasheet de BME680
    uint8_t data2send[length+1];
    data2send[0] = reg_addr;
    int i;
    for(i=1;i<=length;i++){
    	data2send[i] = reg_data[i-1];
    };

    return i2c_send_data(dev_addr, (uint8_t *) data2send, (uint16_t)(length+1), 20) != ESP_OK;
    // return BME68X_INTF_RET_SUCCESS;
}

BME68X_INTF_RET_TYPE
bme68x_spi_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t length, void *intf_ptr){
    spi_device_handle_t *spi = (spi_device_handle_t *)intf_ptr;
    uint8_t rslt = 0;

    int i = 0;
    for (i=0; i<=length; i++){
        // Leer el datasheet para más info
        uint8_t data = 0x00 | (reg_addr>>1);
        // Se envía el registro y el dato a escribir N veces, siendo N el número de bytes que se quieren escribir definido por len
        rslt += spi_send_data(spi, (&data) + i, 1);
        rslt += spi_send_data(spi, (uint8_t *)(reg_data + i), 1);
    }
    return rslt != ESP_OK;
}

BME68X_INTF_RET_TYPE
bme68x_spi_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t length, void *intf_ptr){
    spi_device_handle_t *spi = (spi_device_handle_t *)intf_ptr;
    uint8_t rslt = 0;

    // Leer el datasheet para más info

    int i = 0;
    for (i=0; i<=length; i++){
        uint8_t data = 0xFF | (reg_addr>>1);
        rslt += spi_send_data(spi, (&data) + i, 1);
        rslt += spi_recv_data(spi, reg_data+i, 1);
    }
    
    return  rslt != ESP_OK;
}


void delay_us(uint32_t timeout_us) {
    uint32_t us_act = micros();

    if(timeout_us){
        uint32_t wait = (us_act + timeout_us);
        if(us_act > wait){ // Overflow
            while(micros() > timeout_us){
                // Espera hasta que pase el tiempo ( 0 + timeout)
            }
        }
        while(micros() < wait){
            // Espera hasta que pase el tiempo deseado (act + timeout)
        }
    }
}

void
bme68x_delay_us(uint32_t period, void* intf_ptr) {
    delay_us(period);
}
