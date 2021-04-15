/**
 * \file sensors.c
 * \brief 
 * \version 0.1
 * \date 2021-03-03
 */

#include "sensor.h"
#include "sensor_hal.h"
#include "sensor_defs.h"


sensors_status_t
sensors_init(sensors_config_t* p_config) {
    int8_t bme_result;
    uint8_t* p_bme_dev_i2c_addr;

    struct bme68x_conf bme_conf;
    struct bme68x_heatr_conf bme_heater_conf;

    /* HAL init */
    if (sensors_hal_init() != SENSORS_OK) {
        sensors_deinit(p_config);
        return SENSORS_HAL_ERR;
    }

    p_config->_bme_dev_addr = SENSORS_BME68X_I2C_ADDR;

    /* Setup specific uC functions */
    p_config->bme_dev.read     = &bme68x_i2c_read;
    p_config->bme_dev.write    = &bme68x_i2c_write;
    p_config->bme_dev.delay_us = &bme68x_delay_us;

    /* Config device */
    p_config->bme_dev.intf     = BME68X_I2C_INTF;
    p_config->bme_dev.intf_ptr = &(p_config->_bme_dev_addr);
    p_config->bme_dev.amb_temp = SENSORS_AMBIENT_TEMP;

    bme_result = bme68x_init(&(p_config->bme_dev));
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
    bme_result = bme68x_set_conf(&bme_conf, &(p_config->bme_dev));
    if (bme_result != BME68X_OK) {
        sensors_deinit(p_config);
        return SENSORS_ERR;
    }

    /* Default heater config. From BME68x-Sensor-API examples. */
    bme_heater_conf.enable     = BME68X_ENABLE; /* Permite mediciones de gas */
    bme_heater_conf.heatr_temp = 300;           /* Temperatura */
    bme_heater_conf.heatr_dur  = 100;           /* Duración */
    bme_result = bme68x_set_heatr_conf(BME68X_FORCED_MODE, &bme_heater_conf, &(p_config->bme_dev));
    if (bme_result != BME68X_OK) {
        sensors_deinit(p_config);
        return SENSORS_ERR;
    }

    return SENSORS_OK;
}


sensors_status_t
sensors_deinit(sensors_config_t* p_config) {
    return SENSORS_OK;
}



sensors_status_t get_data(sensors_config_t* p_config)
{
  int8_t rslt;
  uint8_t n_fields;
  

  rslt = bme68x_get_data(BME68X_FORCED_MODE, &p_config->data, &n_fields, &p_config->bme_dev);


  if (rslt != BME68X_OK)
  {
    return SENSORS_ERR;
  }
  else
  {
    return SENSORS_OK;
  }
}

sensors_status_t sleep_data(sensors_config_t* p_config)
{
  int8_t rslt;
  uint8_t n_fields;
  
  rslt = bme68x_get_data(BME68X_SLEEP_MODE, &p_config->data, &n_fields, &p_config->bme_dev);

  if (rslt != BME68X_OK)
  {
    return SENSORS_ERR;
  }
  else
  {
    return SENSORS_OK;
  }
}