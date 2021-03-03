#ifdef TEST

#include <string.h>
#include <stdint.h>
#include <malloc.h>

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
/* Tests
 *  - Init ok, result ok and valid init_done value
 *  - Init error, any of the BME functions fail
 * 
 *  - Deinit check ok
 *  - Deinit check valid free
 */
void
test_sensors_InitOk() {
    /* If the BME68x lib calls are correct, everything should be ok */
    sensors_config_t sensors;

    memset(&sensors, 0, sizeof(sensors_config_t));

    sensors_hal_init_ExpectAndReturn(BME68X_OK);
    bme68x_init_ExpectAnyArgsAndReturn(BME68X_OK);
    bme68x_set_conf_ExpectAnyArgsAndReturn(BME68X_OK);
    bme68x_set_heatr_conf_ExpectAnyArgsAndReturn(BME68X_OK);

    TEST_ASSERT_EQUAL(SENSORS_OK, sensors_init(&sensors));
    TEST_ASSERT_MESSAGE(sensors.p_bme_dev != NULL, "bme68x device struct not created!");
}

void
test_sensors_InitInputErr() {
    /* If the given sensors_config_t is not valid, It should fail */
    sensors_config_t sensors;
    struct bme68x_dev bme_dev;

    memset(&sensors, 0, sizeof(sensors_config_t));
    sensors.p_bme_dev = &bme_dev;

    TEST_ASSERT_EQUAL(SENSORS_IN_USE_ERR, sensors_init(&sensors));
    TEST_ASSERT_EQUAL_MESSAGE(&bme_dev, sensors.p_bme_dev, "bme68x device changed!");
}

void
test_sensors_InitBMEError() {
    /* sensors_init(...) should give an error if any of the BME68x function calls are not valid */
    sensors_config_t sensors1, sensors2, sensors3;

    memset(&sensors1, 0, sizeof(sensors_config_t));
    memset(&sensors2, 0, sizeof(sensors_config_t));
    memset(&sensors3, 0, sizeof(sensors_config_t));

    sensors_hal_init_ExpectAndReturn(BME68X_OK);
    bme68x_init_ExpectAnyArgsAndReturn(BME68X_E_COM_FAIL);

    TEST_ASSERT_EQUAL(SENSORS_ERR, sensors_init(&sensors1));
    TEST_ASSERT_MESSAGE(sensors1.p_bme_dev == NULL, "bme68x device struct [1] not NULL!");

    sensors_hal_init_ExpectAndReturn(BME68X_OK);
    bme68x_init_ExpectAnyArgsAndReturn(BME68X_OK);
    bme68x_set_conf_ExpectAnyArgsAndReturn(BME68X_E_COM_FAIL);

    TEST_ASSERT_EQUAL(SENSORS_ERR, sensors_init(&sensors2));
    TEST_ASSERT_MESSAGE(sensors2.p_bme_dev == NULL, "bme68x device struct [2] not NULL!");

    sensors_hal_init_ExpectAndReturn(BME68X_OK);
    bme68x_init_ExpectAnyArgsAndReturn(BME68X_OK);
    bme68x_set_conf_ExpectAnyArgsAndReturn(BME68X_OK);
    bme68x_set_heatr_conf_ExpectAnyArgsAndReturn(BME68X_E_COM_FAIL);

    TEST_ASSERT_EQUAL(SENSORS_ERR, sensors_init(&sensors3));
    TEST_ASSERT_MESSAGE(sensors3.p_bme_dev == NULL, "bme68x device struct [3] not NULL!");
}

void
test_sensors_InitHALError() {
    /* sensors_init(...) should give an error if the hal init call fails */
    sensors_config_t sensors;

    memset(&sensors, 0, sizeof(sensors_config_t));

    sensors_hal_init_ExpectAndReturn(SENSORS_ERR);

    TEST_ASSERT_EQUAL(SENSORS_HAL_ERR, sensors_init(&sensors));
    TEST_ASSERT_MESSAGE(sensors.p_bme_dev == NULL, "bme68x device struct not NULL!");
}

void
test_sensors_DeinitOk() {
    /* If sensors_deinit(...) call is valid, the config pointers should be NULL  */
    sensors_config_t sensors;

    memset(&sensors, 0, sizeof(sensors_config_t));
    
    struct bme68x_dev* p_bme_dev = (struct bme68x_dev*)malloc(sizeof(struct bme68x_dev));
    uint8_t* p_bme_dev_i2c_addr = (uint8_t*)malloc(sizeof(uint8_t));

    sensors.p_bme_dev = p_bme_dev;
    sensors.p_bme_dev->intf_ptr = p_bme_dev_i2c_addr;

    TEST_ASSERT_EQUAL(SENSORS_OK, sensors_deinit(&sensors));
    TEST_ASSERT_MESSAGE(sensors.p_bme_dev == NULL, "bme68x device struct not NULL!");
}

#endif /* TEST */
