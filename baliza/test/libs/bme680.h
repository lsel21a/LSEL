#ifndef BME680_H
#define BME680_H

#include "esp_err.h"
#include "esp_gpio.h"
#include <stdbool.h>

#define BME680_I2C_ADDR_0 0
#define BME680_OSR_4X     0
#define BME680_OSR_NONE   0
#define BME680_OSR_2X     0
#define BME680_IIR_SIZE_7 0

typedef int i2c_dev_t;
typedef int bme680_settings_t;
typedef int bme680_calib_data_t;
typedef int bme680_oversampling_rate_t;
typedef int bme680_filter_size_t;

typedef struct
{
    i2c_dev_t i2c_dev;
    bool meas_started;
    uint8_t meas_status;
    bme680_settings_t settings;
    bme680_calib_data_t calib_data;
} bme680_t;

typedef struct
{
    float temperature;
    float pressure;
    float humidity;
    float gas_resistance;
} bme680_values_float_t;

esp_err_t bme680_init_desc(bme680_t *dev, uint8_t addr, i2c_port_t port, gpio_num_t sda_gpio, gpio_num_t scl_gpio);
esp_err_t bme680_init_sensor(bme680_t *dev);
esp_err_t bme680_set_oversampling_rates(bme680_t *dev, bme680_oversampling_rate_t osr_t,
                                        bme680_oversampling_rate_t osr_p, bme680_oversampling_rate_t osr_h);
esp_err_t bme680_set_filter_size(bme680_t *dev, bme680_filter_size_t size);
esp_err_t bme680_set_heater_profile(bme680_t *dev, uint8_t profile, uint16_t temperature, uint16_t duration);
esp_err_t bme680_use_heater_profile(bme680_t *dev, int8_t profile);
esp_err_t bme680_set_ambient_temperature(bme680_t *dev, int16_t temperature);
esp_err_t bme680_get_measurement_duration(bme680_t *dev, uint32_t *duration);
esp_err_t bme680_force_measurement(bme680_t *dev);
esp_err_t bme680_get_results_float(bme680_t *dev, bme680_values_float_t *results);

#endif /* BME680_H */