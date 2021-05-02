#include <string.h>
#include <stdint.h>

#include "unity.h"

#include "esp_err.h"

#ifdef TEST
#include "mock_bme680.h"
#else
#include "bme680.h"
#endif /* TEST */

#include "sensors/sensor.h"
#include "sensors/sensor_defs.h"

#define BME680_LIB_MOCK() do {                                      \
    bme680_init_desc_ExpectAnyArgsAndReturn(ESP_OK);                \
    bme680_init_sensor_ExpectAnyArgsAndReturn(ESP_OK);              \
    bme680_set_oversampling_rates_ExpectAnyArgsAndReturn(ESP_OK);   \
    bme680_set_filter_size_ExpectAnyArgsAndReturn(ESP_OK);          \
    bme680_set_heater_profile_ExpectAnyArgsAndReturn(ESP_OK);       \
    bme680_use_heater_profile_ExpectAnyArgsAndReturn(ESP_OK);       \
    bme680_set_ambient_temperature_ExpectAnyArgsAndReturn(ESP_OK);  \
} while(0)

#define TEST_RMOX 112233.11;
#define TEST_HUM  11.29;
#define TEST_PRES 0.10203;
#define TEST_TEMP 21.09;


void
setUp(void) {
}

void
tearDown(void) {
}

void
test_sensors_InitOk() {
    /* If the BME680 lib calls are correct, everything should be ok */
    sensors_config_t sensors;

    memset(&sensors, 0, sizeof(sensors_config_t));
    sensors.sensors_select = SENSORS_SELECT1;

    BME680_LIB_MOCK();
    TEST_ASSERT_EQUAL(SENSORS_OK, sensors_init(&sensors));
}

// TODO. How can we choose the sensor??
void
test_selectAnotherSensor() {
    TEST_IGNORE_MESSAGE("How can we choose the sensor?");
}

void
test_getDataOk() {
    /* Valid request of get_data(...) */
    sensors_config_t sensors;
    sensors_config_t* p_sensors = &sensors;

    bme680_values_float_t values;
    values.gas_resistance = TEST_RMOX;
    values.humidity = TEST_HUM;
    values.pressure = TEST_PRES;
    values.temperature = TEST_TEMP;

    BME680_LIB_MOCK();
    sensors_init(p_sensors);

    bme680_get_measurement_duration_ExpectAnyArgsAndReturn(ESP_OK);
    bme680_force_measurement_ExpectAnyArgsAndReturn(ESP_OK);
    bme680_get_results_float_ExpectAnyArgsAndReturn(ESP_OK);
    bme680_get_results_float_ReturnThruPtr_results(&values);

    TEST_ASSERT_EQUAL(SENSORS_OK, get_data(p_sensors));
    TEST_ASSERT_EQUAL(values.gas_resistance, sensors.data.gas_resistance);
    TEST_ASSERT_EQUAL(values.humidity, sensors.data.humidity);
    TEST_ASSERT_EQUAL(values.pressure, sensors.data.pressure);
    TEST_ASSERT_EQUAL(values.temperature, sensors.data.temperature);
}

void
test_getDataFail1() {
    /* When bme680_get_measurement_duration fails, get_data(...) should fail too */
    sensors_config_t sensors;
    sensors_config_t* p_sensors = &sensors;

    BME680_LIB_MOCK();
    sensors_init(p_sensors);

    bme680_get_measurement_duration_ExpectAnyArgsAndReturn(ESP_FAIL);

    TEST_ASSERT_EQUAL(SENSORS_ERR, get_data(p_sensors));
}

void
test_getDataFail2() {
    /* When bme680_force_measurement fails, get_data(...) should fail too */
    sensors_config_t sensors;
    sensors_config_t* p_sensors = &sensors;

    BME680_LIB_MOCK();
    sensors_init(p_sensors);

    bme680_get_measurement_duration_ExpectAnyArgsAndReturn(ESP_OK);
    bme680_force_measurement_ExpectAnyArgsAndReturn(ESP_FAIL);

    TEST_ASSERT_EQUAL(SENSORS_ERR, get_data(p_sensors));
}

void
test_getDataFail3() {
    /* When bme680_get_results_float fails, get_data(...) should fail too */
    sensors_config_t sensors;
    sensors_config_t* p_sensors = &sensors;

    BME680_LIB_MOCK();
    sensors_init(p_sensors);

    bme680_get_measurement_duration_ExpectAnyArgsAndReturn(ESP_OK);
    bme680_force_measurement_ExpectAnyArgsAndReturn(ESP_OK);
    bme680_get_results_float_ExpectAnyArgsAndReturn(ESP_FAIL);

    TEST_ASSERT_EQUAL(SENSORS_ERR, get_data(p_sensors));
}

void
test_sensors_DeinitOk() {
    /* sensors_deinit(...) should have a valid return value */
    sensors_config_t sensors;

    TEST_ASSERT_EQUAL(SENSORS_OK, sensors_deinit(&sensors));
}

void
test_EnterSleep() {
    TEST_IGNORE_MESSAGE("Not implemented!");
}
