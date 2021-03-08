#define TEST
#ifdef TEST

#include "unity.h"

#include "sensor.h"
#include "common.h"
#include "bme68x_defs.h"
#include "mock_bme68x.h"

struct bme68x_dev bme;

void setUp(void)
{
}

void tearDown(void)
{
}

void test_sensor_getTempDataReturnsNULLPtrIfDataIsInvalid(void)
{
	float * act;
	
	bme68x_get_data_ExpectAnyArgsAndReturn(-3);   //there are different types of error (from -1 to -4) returned from the function 
	act = get_temp_data(&bme);
	
	TEST_ASSERT_NULL(act);
}


void test_sensor_getTempDataReturnsSomethingIfDataIsValid(void)
{
	float * act;
	
	bme68x_get_data_ExpectAnyArgsAndReturn(0);   //  0 means BME68X_OK 
	act = get_temp_data(&bme);
	
	TEST_ASSERT_NOT_NULL(act);
}

void test_sensor_getGasDataReturnsNULLPtrIfDataIsInvalid(void)
{
	float * act;
	
	bme68x_get_data_ExpectAnyArgsAndReturn(-2);   //there are different types of error (from -1 to -4) returned from the function 
	act = get_gas_data(&bme);
	
	TEST_ASSERT_NULL(act);
}


void test_sensor_getGasDataReturnsSomethingIfDataIsValid(void)
{
	float * act;
	
	bme68x_get_data_ExpectAnyArgsAndReturn(0);   //  0 means BME68X_OK 
	act = get_gas_data(&bme);
	
	TEST_ASSERT_NOT_NULL(act);
}

void test_sensor_getHumidityDataReturnsNULLPtrIfDataIsInvalid(void)
{
	float * act;
	
	bme68x_get_data_ExpectAnyArgsAndReturn(-1);   //there are different types of error (from -1 to -4) returned from the function 
	act = get_humidity_data(&bme);
	
	TEST_ASSERT_NULL(act);
}


void test_sensor_getHumidityDataReturnsSomethingIfDataIsValid(void)
{
	float * act;
	
	bme68x_get_data_ExpectAnyArgsAndReturn(0);   //  0 means BME68X_OK 
	act = get_humidity_data(&bme);
	
	TEST_ASSERT_NOT_NULL(act);
}
#endif // TEST
