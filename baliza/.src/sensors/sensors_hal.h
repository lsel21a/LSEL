/**
 * \file sensors_hal.h
 * \brief 
 * \version 0.1
 * \date 2021-03-03
 */


#ifndef SENSORS_HAL_H
#define SENSORS_HAL_H


#include "sensors_defs.h"

#include "bme68x/bme68x.h"

/* CPP guard */
#ifdef __cplusplus
extern "C" {
#endif


/**
 * \brief           Init the HAL system [Ex. init I2C]
 * 
 * \return          \ref SENSORS_OK on success, member of \ref sensors_status_t otherwise
 */
sensors_status_t sensors_hal_init(void);

/**
 * \brief           BME68x lib wrapper to read sensor data using the I2C bus 
 * 
 * \param[in]       reg_addr: I2C address of the sensor
 * \param[out]      reg_data: Pointer where the readed data is going to be stored
 * \param[in]       length: Number of bytes to read
 * \param[in,out]   intf_ptr: Multipurpose void pointer
 * \return          Read result. BME68X_INTF_RET_TYPE
 */
BME68X_INTF_RET_TYPE bme68x_i2c_read(uint8_t reg_addr, uint8_t* reg_data, uint32_t length, void* intf_ptr);

/**
 * \brief           BME68x lib wrapper to write sensor data using the I2C bus 
 * 
 * \param[in]       reg_addr: I2C address of the sensor
 * \param[in]       reg_data: Pointer with the data
 * \param[in]       length: Number of bytes to write
 * \param[in,out]   intf_ptr: Multipurpose void pointer
 * \return          Write result. BME68X_INTF_RET_TYPE 
 */
BME68X_INTF_RET_TYPE bme68x_i2c_write(uint8_t reg_addr, const uint8_t* reg_data, uint32_t length, void* intf_ptr);

/**
 * \brief           BME68x lib wrapper to make delay [in uS]
 * 
 * \param[in]       period: uS
 * \param[in,out]   intf_ptr: Multipurpose void pointer
 */
void bme68x_delay_us(uint32_t period, void* intf_ptr);


/* End of CPP guard */
#ifdef __cplusplus
}
#endif

#endif /* SENSORS_HAL_H */
