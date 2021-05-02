#include "unity.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "fsm_fire_detection/algorithm/algoritmo.h"


#define TEMP_SEC  ALGO_TMAX - 1
#define TEMP_NSEC ALGO_TMAX + 1
#define HUM_SEC   ALGO_HMIN + 1
#define HUM_NSEC  ALGO_HMIN - 1
#define GAS_SEC   ALGO_GMAX - 1
#define GAS_NSEC  ALGO_GMAX + 1

#define SET_ALL(a, b, n) for(int i = 0; i < n; i++) { a[i] = b; }

void
setUp(void) {
}

void
tearDown(void) {
}

void
test_algo_NoAlarm() {
    int n_items = 3;
    float temp[n_items], hum[n_items], gas[n_items];

    // All inside limits
    SET_ALL(temp, TEMP_SEC, n_items)
    SET_ALL(hum, HUM_SEC, n_items)
    SET_ALL(gas, GAS_SEC, n_items)

    TEST_ASSERT_EQUAL(algoritmo_incendio(temp, hum, gas, n_items), 0);

    // High temp
    SET_ALL(temp, TEMP_NSEC, n_items)

    TEST_ASSERT_EQUAL(algoritmo_incendio(temp, hum, gas, n_items), 0);

    // Low humidity
    SET_ALL(temp, TEMP_SEC, n_items)
    SET_ALL(hum, HUM_NSEC, n_items)

    TEST_ASSERT_EQUAL(algoritmo_incendio(temp, hum, gas, n_items), 0);

    // High gas
    SET_ALL(hum, HUM_SEC, n_items)
    SET_ALL(gas, GAS_NSEC, n_items)

    TEST_ASSERT_EQUAL(algoritmo_incendio(temp, hum, gas, n_items), 0);
}

void
test_algo_Alarm() {
    int n_items = 3;
    float temp[n_items], hum[n_items], gas[n_items];

    // All outside limits
    SET_ALL(temp, TEMP_NSEC, n_items)
    SET_ALL(hum, HUM_NSEC, n_items)
    SET_ALL(gas, GAS_NSEC, n_items)

    TEST_ASSERT_EQUAL(algoritmo_incendio(temp, hum, gas, n_items), 1);

    // Valid temperature
    SET_ALL(temp, TEMP_SEC, n_items)

    TEST_ASSERT_EQUAL(algoritmo_incendio(temp, hum, gas, n_items), 1);

    // Valid humidity
    SET_ALL(temp, TEMP_NSEC, n_items)
    SET_ALL(hum, HUM_SEC, n_items)

    TEST_ASSERT_EQUAL(algoritmo_incendio(temp, hum, gas, n_items), 1);

    // Valid gas
    SET_ALL(hum, HUM_NSEC, n_items)
    SET_ALL(gas, GAS_SEC, n_items)

    TEST_ASSERT_EQUAL(algoritmo_incendio(temp, hum, gas, n_items), 1);
}

void
test_algo_AlarmNotAll() {
    int n_items = 3;
    float temp[n_items], hum[n_items], gas[n_items];

    // All inside limits
    SET_ALL(temp, TEMP_SEC, n_items)
    SET_ALL(hum, HUM_SEC, n_items)
    SET_ALL(gas, GAS_SEC, n_items)

    // Select some invalid temp
    temp[0] = TEMP_NSEC;
    temp[2] = TEMP_NSEC;

    TEST_ASSERT_EQUAL(algoritmo_incendio(temp, hum, gas, n_items), 0);

    // Select some invalid hum
    hum[1] = HUM_NSEC;
    hum[2] = HUM_NSEC;

    TEST_ASSERT_EQUAL(algoritmo_incendio(temp, hum, gas, n_items), 1);

    // Select some invalid hum
    gas[0] = GAS_NSEC;
    gas[1] = GAS_NSEC;

    TEST_ASSERT_EQUAL(algoritmo_incendio(temp, hum, gas, n_items), 1);
}
