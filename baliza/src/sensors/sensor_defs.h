/**
 * \file sensors_defs.h
 * \brief 
 * \version 0.1
 * \date 2021-03-03
 */


#ifndef SENSORS_DEFS_H
#define SENSORS_DEFS_H

#include <stdint.h>
#include <stddef.h>
#include <bme680.h>

/**
 * \brief           Select which sensors group to use. In this case, it select the BME68x address
 */
typedef enum {
    SENSORS_SELECT1,                            /*! Use BME68X_I2C_ADDR_LOW */
    SENSORS_SELECT2,                            /*! Use BME68X_I2C_ADDR_HIGH */
} sensors_select_t;


typedef enum {
    SENSORS_OK,                                 /* Everything Ok 👌 */
    SENSORS_ERR,                                /* Something went wrong */
    SENSORS_BME680_ERR,                         /* BME680 lib init error */
    SENSORS_IN_USE_ERR,                         /* Already initialized */
    SENSORS_MEM_ERR,                            /* Dynamic memory error */
    SENSORS_HAL_ERR                             /* Sensors HAL error */
} sensors_status_t;

typedef struct {
    /*! Temperature in degree celsius */
    float temperature;

    /*! Pressure in Pascal */
    float pressure;

    /*! Humidity in % relative humidity x1000 */
    float humidity;

    /*! Gas resistance in Ohms */
    float gas_resistance;
} sensors_data_t;

typedef struct {
    bme680_t bme_dev;                           /* BME680 device */
    sensors_data_t data;                        /* BME68x data */        
    sensors_select_t sensors_select;            /*! Sensor selection */
    uint8_t _bme_dev_addr;                      /*! [Internal] BME68x I2C address */
} sensors_config_t;



#endif /* SENSORS_DEFS_H */
