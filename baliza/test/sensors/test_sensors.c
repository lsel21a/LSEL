#define TEST
#ifdef TEST

#include <string.h>
#include <stdint.h>

#include "unity.h"

#include "sensors/sensors.h"
// #include "sensors/sensors_hal.h"
#include "mock_sensors_hal.h"
#include "sensors/sensors_defs.h"

#include "bme68x/bme68x_defs.h"
#include "mock_bme68x.h"


void
setUp(void) {
}

void
tearDown(void) {
}

void
test_sensors_InitOk() {
    /* If the BME68x lib calls are correct, everything should be ok */
    sensors_config_t sensors;

    memset(&sensors, 0, sizeof(sensors_config_t));
    sensors.sensors_select = SENSORS_SELECT1;

    sensors_hal_init_ExpectAndReturn(BME68X_OK);
    bme68x_init_ExpectAnyArgsAndReturn(BME68X_OK);
    bme68x_set_conf_ExpectAnyArgsAndReturn(BME68X_OK);
    bme68x_set_heatr_conf_ExpectAnyArgsAndReturn(BME68X_OK);

    TEST_ASSERT_EQUAL(SENSORS_OK, sensors_init(&sensors));
}

void
test_sensors_InitOkValidAddr() {
    /* If sensors_init(...) is valid, bme_dev must have a valid BME68x address */
    sensors_config_t sensors1, sensors2;

    memset(&sensors1, 0, sizeof(sensors_config_t));
    memset(&sensors2, 0, sizeof(sensors_config_t));
    sensors1.sensors_select = SENSORS_SELECT1;
    sensors2.sensors_select = SENSORS_SELECT2;

    sensors_hal_init_ExpectAndReturn(BME68X_OK);
    bme68x_init_ExpectAnyArgsAndReturn(BME68X_OK);
    bme68x_set_conf_ExpectAnyArgsAndReturn(BME68X_OK);
    bme68x_set_heatr_conf_ExpectAnyArgsAndReturn(BME68X_OK);

    TEST_ASSERT_EQUAL(SENSORS_OK, sensors_init(&sensors1));
    TEST_ASSERT_EQUAL(sensors1._bme_dev_addr, BME68X_I2C_ADDR_LOW);
    TEST_ASSERT_EQUAL(sensors1.bme_dev.intf_ptr, &(sensors1._bme_dev_addr));


    sensors_hal_init_ExpectAndReturn(BME68X_OK);
    bme68x_init_ExpectAnyArgsAndReturn(BME68X_OK);
    bme68x_set_conf_ExpectAnyArgsAndReturn(BME68X_OK);
    bme68x_set_heatr_conf_ExpectAnyArgsAndReturn(BME68X_OK);

    TEST_ASSERT_EQUAL(SENSORS_OK, sensors_init(&sensors2));
    TEST_ASSERT_EQUAL(sensors2._bme_dev_addr, BME68X_I2C_ADDR_HIGH);
    TEST_ASSERT_EQUAL(sensors2.bme_dev.intf_ptr, &(sensors2._bme_dev_addr));
}

void
test_sensors_InitErrNotValidSelection() {
    /* If an invalid sensors_select value is given, sensors_init(...) should fail */
    sensors_config_t sensors;

    memset(&sensors, 0, sizeof(sensors_config_t));
    sensors.sensors_select = -1;

    sensors_hal_init_ExpectAndReturn(BME68X_OK);

    TEST_ASSERT_EQUAL(SENSORS_SELECT_NOT_VALID, sensors_init(&sensors));
}

void
test_sensors_InitOkChkHAL() {
    /* Check if bme68x_dev has valid pointers to the HAL functions */
    sensors_config_t sensors;

    memset(&sensors, 0, sizeof(sensors_config_t));
    sensors.sensors_select = SENSORS_SELECT1;

    sensors_hal_init_ExpectAndReturn(BME68X_OK);
    bme68x_init_ExpectAnyArgsAndReturn(BME68X_OK);
    bme68x_set_conf_ExpectAnyArgsAndReturn(BME68X_OK);
    bme68x_set_heatr_conf_ExpectAnyArgsAndReturn(BME68X_OK);

    TEST_ASSERT_EQUAL(SENSORS_OK, sensors_init(&sensors));
    TEST_ASSERT_NOT_EQUAL_MESSAGE(sensors.bme_dev.read, NULL, "HAL read not set!");
    TEST_ASSERT_NOT_EQUAL_MESSAGE(sensors.bme_dev.write, NULL, "HAL write not set!");
    TEST_ASSERT_NOT_EQUAL_MESSAGE(sensors.bme_dev.delay_us, NULL, "HAL delay not set!");
}

void
test_sensors_InitBMEError() {
    /* sensors_init(...) should give an error if any of the BME68x function calls are not valid */
    sensors_config_t sensors1, sensors2, sensors3;

    memset(&sensors1, 0, sizeof(sensors_config_t));
    memset(&sensors2, 0, sizeof(sensors_config_t));
    memset(&sensors3, 0, sizeof(sensors_config_t));
    sensors1.sensors_select = SENSORS_SELECT1;
    sensors2.sensors_select = SENSORS_SELECT1;
    sensors3.sensors_select = SENSORS_SELECT1;

    sensors_hal_init_ExpectAndReturn(BME68X_OK);
    bme68x_init_ExpectAnyArgsAndReturn(BME68X_E_COM_FAIL);

    TEST_ASSERT_EQUAL(SENSORS_ERR, sensors_init(&sensors1));

    sensors_hal_init_ExpectAndReturn(BME68X_OK);
    bme68x_init_ExpectAnyArgsAndReturn(BME68X_OK);
    bme68x_set_conf_ExpectAnyArgsAndReturn(BME68X_E_COM_FAIL);

    TEST_ASSERT_EQUAL(SENSORS_ERR, sensors_init(&sensors2));

    sensors_hal_init_ExpectAndReturn(BME68X_OK);
    bme68x_init_ExpectAnyArgsAndReturn(BME68X_OK);
    bme68x_set_conf_ExpectAnyArgsAndReturn(BME68X_OK);
    bme68x_set_heatr_conf_ExpectAnyArgsAndReturn(BME68X_E_COM_FAIL);

    TEST_ASSERT_EQUAL(SENSORS_ERR, sensors_init(&sensors3));
}

void
test_sensors_InitHALError() {
    /* sensors_init(...) should give an error if the hal init call fails */
    sensors_config_t sensors;

    memset(&sensors, 0, sizeof(sensors_config_t));
    sensors.sensors_select = SENSORS_SELECT1;

    sensors_hal_init_ExpectAndReturn(SENSORS_ERR);

    TEST_ASSERT_EQUAL(SENSORS_HAL_ERR, sensors_init(&sensors));
}

void
test_sensors_DeinitOk() {
    /* sensors_deinit(...) should have a valid return value */
    sensors_config_t sensors;

    TEST_ASSERT_EQUAL(SENSORS_OK, sensors_deinit(&sensors));
}

#endif /* TEST */
