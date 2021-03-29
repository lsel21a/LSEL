/**
 * \file sensors_hal.c
 * \brief 
 * \version 0.1
 * \date 2021-03-03
 */

#include "sensor_hal.h"
#include "sensor_defs.h"
#include "bme68x/bme68x.h"


sensors_status_t
sensors_hal_init(void) {
    // TODO - Init I2C
    return SENSORS_OK;
}


BME68X_INTF_RET_TYPE
bme68x_i2c_read(uint8_t reg_addr, uint8_t* reg_data, uint32_t length, void* intf_ptr) {
    // TODO - Add call(s) to the sdk I2C read function(s)
    return BME68X_INTF_RET_SUCCESS;
}


BME68X_INTF_RET_TYPE
bme68x_i2c_write(uint8_t reg_addr, const uint8_t* reg_data, uint32_t length, void* intf_ptr) {
    // TODO - Add call(s) to the sdk I2C write function(s)
    return BME68X_INTF_RET_SUCCESS;
}


void
bme68x_delay_us(uint32_t period, void* intf_ptr) {
    // TODO - Add call(s) to the sdk delay_us function(s)
}
