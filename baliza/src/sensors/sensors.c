/**
 * \file sensors.c
 * \brief 
 * \version 0.1
 * \date 2021-03-03
 */

#include "sensors.h"
#include "sensors_hal.h"
#include "sensors_defs.h"


sensors_status_t
sensors_init(sensors_config_t* p_config) {
    int8_t bme_result;
    uint8_t* p_bme_dev_i2c_addr;

    struct bme68x_conf bme_conf;
    struct bme68x_heatr_conf bme_heater_conf;

    /* Check if the BME device already has been initialized */
    if (p_config->p_bme_dev != NULL) {
        return SENSORS_IN_USE_ERR;
    }

    /* HAL init */
    if (sensors_hal_init() != SENSORS_OK) {
        sensors_deinit(p_config);
        return SENSORS_HAL_ERR;
    }

    /* Mem. alloc */
    p_config->p_bme_dev = (struct bme68x_dev*)malloc(sizeof(struct bme68x_dev));
    if (p_config->p_bme_dev == NULL) {
        sensors_deinit(p_config);
        return SENSORS_MEM_ERR;
    }

    p_bme_dev_i2c_addr = (uint8_t*)malloc(sizeof(uint8_t));
    if (p_bme_dev_i2c_addr == NULL) {
        sensors_deinit(p_config);
        return SENSORS_MEM_ERR;
    }

    *p_bme_dev_i2c_addr = SENSORS_BME68X_I2C_ADDR;

    /* Setup specific uC functions */
    p_config->p_bme_dev->read     = &bme68x_i2c_read;
    p_config->p_bme_dev->write    = &bme68x_i2c_write;
    p_config->p_bme_dev->delay_us = &bme68x_delay_us;

    /* Config device */
    p_config->p_bme_dev->intf     = BME68X_I2C_INTF;
    p_config->p_bme_dev->intf_ptr = p_bme_dev_i2c_addr;
    p_config->p_bme_dev->amb_temp = SENSORS_AMBIENT_TEMP;

    bme_result = bme68x_init(p_config->p_bme_dev);
    if (bme_result != BME68X_OK) {
        sensors_deinit(p_config);
        return SENSORS_ERR;
    }

    /* Default sensor config. From BME68x-Sensor-API examples. */
    bme_conf.filter  = BME68X_FILTER_OFF;       /* Activación del filtro interno IIR */
    bme_conf.odr     = BME68X_ODR_NONE;         /* Tiempo de inactividad */
    bme_conf.os_hum  = BME68X_OS_16X;           /* Oversampling de humedad */
    bme_conf.os_pres = BME68X_OS_1X;            /* Oversampling de presión */
    bme_conf.os_temp = BME68X_OS_2X;            /* Oversampling de temperatura */
    bme_result = bme68x_set_conf(&bme_conf, p_config->p_bme_dev);
    if (bme_result != BME68X_OK) {
        sensors_deinit(p_config);
        return SENSORS_ERR;
    }

    /* Default heater config. From BME68x-Sensor-API examples. */
    bme_heater_conf.enable     = BME68X_ENABLE; /* Permite mediciones de gas */
    bme_heater_conf.heatr_temp = 300;           /* Temperatura */
    bme_heater_conf.heatr_dur  = 100;           /* Duración */
    bme_result = bme68x_set_heatr_conf(BME68X_FORCED_MODE, &bme_heater_conf, p_config->p_bme_dev);
    if (bme_result != BME68X_OK) {
        sensors_deinit(p_config);
        return SENSORS_ERR;
    }

    return SENSORS_OK;
}


sensors_status_t
sensors_deinit(sensors_config_t* p_config) {
    if (p_config->p_bme_dev != NULL) {
        if (p_config->p_bme_dev->intf_ptr != NULL) {
            free(p_config->p_bme_dev->intf_ptr);
            p_config->p_bme_dev->intf_ptr = NULL;
        }

        free(p_config->p_bme_dev);
        p_config->p_bme_dev = NULL;
    }

    return SENSORS_OK;
}
