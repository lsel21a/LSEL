/**
 * \file sensors.c
 * \brief 
 * \version 0.1
 * \date 2021-03-03
 */

#include <bme680.h>
#include <string.h>

#include "sensor.h"
#include "sensor_defs.h"


sensors_status_t
sensors_init(sensors_config_t* p_config) {

  //
  memset(&(p_config->bme_dev), 0, sizeof(bme680_t));

  // Init I2C
  i2cdev_init();

  //
  ESP_ERROR_CHECK(bme680_init_sensor(&(p_config->bme_dev)));

  //
  bme680_init_sensor(&(p_config->bme_dev));


  //
  bme680_set_oversampling_rates(&(p_config->bme_dev), BME680_OSR_4X, BME680_OSR_NONE, BME680_OSR_2X);

  // Change the IIR filter size for temperature and pressure to 7.
  bme680_set_filter_size(&(p_config->bme_dev), BME680_IIR_SIZE_7);

  // Change the heater profile 0 to 200 degree Celsius for 100 ms.
  bme680_set_heater_profile(&(p_config->bme_dev), 0, 200, 100);
  bme680_use_heater_profile(&(p_config->bme_dev), 0);

  // Set ambient temperature to 10 degree Celsius
  bme680_set_ambient_temperature(&(p_config->bme_dev), SENSORS_AMBIENT_TEMP);

  return SENSORS_OK;
}

sensors_status_t
sensors_deinit(sensors_config_t* p_config) {
    return SENSORS_OK;
}

sensors_status_t
get_data(sensors_config_t* p_config) {
  uint32_t duration;
  bme680_get_measurement_duration(&(p_config->bme_dev), &duration);

  bme680_values_float_t values;

  if (bme680_force_measurement(&(p_config->bme_dev)) == ESP_OK) {
    vTaskDelay(duration);

    if (bme680_get_results_float(&(p_config->bme_dev), &values) != ESP_OK) {
      return SENSORS_ERR;
    }
    else {
      printf("BME680 Sensor: %.2f °C, %.2f %%, %.2f hPa, %.2f Ohm\n",
              values.temperature, values.humidity, values.pressure, values.gas_resistance);

      p_config->data.temperature = values.temperature;
      p_config->data.pressure = values.pressure;
      p_config->data.humidity = values.humidity;
      p_config->data.gas_resistance = values.gas_resistance;
  
      return SENSORS_OK;
    }
  }

  return SENSORS_ERR;
}

sensors_status_t
sleep_data(sensors_config_t* p_config) {
  return SENSORS_OK;
}
