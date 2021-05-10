#include "unity.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <freertos/queue.h>

#include "fsm.h"
#include "fsm_fire_detection/fsm_deteccion_incendio.h"
#include "mock_drivers_fsm_deteccion_incendio.h"


void
setUp(void) {
}

void
tearDown(void) {
}

void
test_fsm_fsmInitFillsStructWithSomething(){
    fsm_deteccion_incendio_t zero, fsm;
    QueueHandle_t datovalidoqueue, datossensoresqueue, incendioqueue, muestreorapidoqueue;
    float temperatura, humedad, gases;

    memset(&zero, 0, sizeof(fsm_deteccion_incendio_t));
    memset(&fsm, 0, sizeof(fsm_deteccion_incendio_t));

    fsm_deteccion_incendio_init(&fsm, &temperatura, &humedad, &gases, &datovalidoqueue, &datossensoresqueue, &incendioqueue, &muestreorapidoqueue);
    TEST_ASSERT(memcmp(&zero, &fsm, sizeof(fsm_deteccion_incendio_t)) != 0);
}

void
test_fsm_fsmInitCheckInitialState(){
    fsm_deteccion_incendio_t fsm;
    QueueHandle_t datovalidoqueue, datossensoresqueue, incendioqueue, muestreorapidoqueue;
    float temperatura, humedad, gases;
    
    fsm_deteccion_incendio_init(&fsm, &temperatura, &humedad, &gases, &datovalidoqueue, &datossensoresqueue, &incendioqueue, &muestreorapidoqueue);

    TEST_ASSERT(fsm.fsm.current_state == IDLE_INCENDIO);
}

void
test_fsm_IDLE_INCENDIOToDATA_SENSOR() {
    fsm_deteccion_incendio_t fsm;
    QueueHandle_t datovalidoqueue, datossensoresqueue, incendioqueue, muestreorapidoqueue;
    float temperatura, humedad, gases;
    
    fsm_deteccion_incendio_init(&fsm, &temperatura, &humedad, &gases, &datovalidoqueue, &datossensoresqueue, &incendioqueue, &muestreorapidoqueue);

    ReceiveDatoValido_ExpectAnyArgsAndReturn(true);
    GetDataFromFsmSensor_ExpectAnyArgs();

    fsm_fire(&(fsm.fsm));

    TEST_ASSERT(fsm.fsm.current_state == DATA_SENSOR);
}

void
test_fsm_IDLE_INCENDIONoChange() {
    fsm_deteccion_incendio_t fsm;
    QueueHandle_t datovalidoqueue, datossensoresqueue, incendioqueue, muestreorapidoqueue;
    float temperatura, humedad, gases;
    
    fsm_deteccion_incendio_init(&fsm, &temperatura, &humedad, &gases, &datovalidoqueue, &datossensoresqueue, &incendioqueue, &muestreorapidoqueue);

    ReceiveDatoValido_ExpectAnyArgsAndReturn(false);

    fsm_fire(&(fsm.fsm));

    TEST_ASSERT(fsm.fsm.current_state == IDLE_INCENDIO);
}

void
test_fsm_DATA_SENSORToIDLE_INCENDIO() {
    fsm_deteccion_incendio_t fsm;
    QueueHandle_t datovalidoqueue, datossensoresqueue, incendioqueue, muestreorapidoqueue;
    float temperatura, humedad, gases;
    
    fsm_deteccion_incendio_init(&fsm, &temperatura, &humedad, &gases, &datovalidoqueue, &datossensoresqueue, &incendioqueue, &muestreorapidoqueue);

    fsm.fsm.current_state = DATA_SENSOR;

    NoHayPeligro_ExpectAnyArgsAndReturn(true);
    BackToIdle_ExpectAnyArgs();

    fsm_fire(&(fsm.fsm));

    TEST_ASSERT(fsm.fsm.current_state == IDLE_INCENDIO);
}

void
test_fsm_DATA_SENSORToPOSIBLE_INCENDIO() {
    fsm_deteccion_incendio_t fsm;
    QueueHandle_t datovalidoqueue, datossensoresqueue, incendioqueue, muestreorapidoqueue;
    float temperatura, humedad, gases;
    
    fsm_deteccion_incendio_init(&fsm, &temperatura, &humedad, &gases, &datovalidoqueue, &datossensoresqueue, &incendioqueue, &muestreorapidoqueue);

    fsm.fsm.current_state = DATA_SENSOR;

    NoHayPeligro_ExpectAnyArgsAndReturn(false);
    PuedeSerIncendio_ExpectAnyArgsAndReturn(true);
    GetMuestreoRapido_ExpectAnyArgs();

    fsm_fire(&(fsm.fsm));

    TEST_ASSERT(fsm.fsm.current_state == POSIBLE_INCENDIO);
}

void
test_fsm_DATA_SENSORNoChange() {
    fsm_deteccion_incendio_t fsm;
    QueueHandle_t datovalidoqueue, datossensoresqueue, incendioqueue, muestreorapidoqueue;
    float temperatura, humedad, gases;
    
    fsm_deteccion_incendio_init(&fsm, &temperatura, &humedad, &gases, &datovalidoqueue, &datossensoresqueue, &incendioqueue, &muestreorapidoqueue);

    fsm.fsm.current_state = DATA_SENSOR;

    NoHayPeligro_ExpectAnyArgsAndReturn(false);
    PuedeSerIncendio_ExpectAnyArgsAndReturn(false);

    fsm_fire(&(fsm.fsm));

    TEST_ASSERT(fsm.fsm.current_state == DATA_SENSOR);
}

void
test_fsm_POSIBLE_INCENDIOToDATA_SENSOR_POSIBLE_INCENDIO() {
    fsm_deteccion_incendio_t fsm;
    QueueHandle_t datovalidoqueue, datossensoresqueue, incendioqueue, muestreorapidoqueue;
    float temperatura, humedad, gases;
    
    fsm_deteccion_incendio_init(&fsm, &temperatura, &humedad, &gases, &datovalidoqueue, &datossensoresqueue, &incendioqueue, &muestreorapidoqueue);

    fsm.fsm.current_state = POSIBLE_INCENDIO;

    ReceiveDatoValido_ExpectAnyArgsAndReturn(true);
    GetDataFromFsmSensor_ExpectAnyArgs();

    fsm_fire(&(fsm.fsm));

    TEST_ASSERT(fsm.fsm.current_state == DATA_SENSOR_POSIBLE_INCENDIO);
}

void
test_fsm_POSIBLE_INCENDIONoChange() {
    fsm_deteccion_incendio_t fsm;
    QueueHandle_t datovalidoqueue, datossensoresqueue, incendioqueue, muestreorapidoqueue;
    float temperatura, humedad, gases;
    
    fsm_deteccion_incendio_init(&fsm, &temperatura, &humedad, &gases, &datovalidoqueue, &datossensoresqueue, &incendioqueue, &muestreorapidoqueue);

    fsm.fsm.current_state = POSIBLE_INCENDIO;

    ReceiveDatoValido_ExpectAnyArgsAndReturn(false);

    fsm_fire(&(fsm.fsm));

    TEST_ASSERT(fsm.fsm.current_state == POSIBLE_INCENDIO);
}

void
test_fsm_DATA_SENSOR_POSIBLE_INCENDIOToIDLE_INCENDIO() {
    fsm_deteccion_incendio_t fsm;
    QueueHandle_t datovalidoqueue, datossensoresqueue, incendioqueue, muestreorapidoqueue;
    float temperatura, humedad, gases;
    
    fsm_deteccion_incendio_init(&fsm, &temperatura, &humedad, &gases, &datovalidoqueue, &datossensoresqueue, &incendioqueue, &muestreorapidoqueue);

    fsm.fsm.current_state = DATA_SENSOR_POSIBLE_INCENDIO;

    NoHayPeligro_ExpectAnyArgsAndReturn(true);
    BackToIdle_ExpectAnyArgs();

    fsm_fire(&(fsm.fsm));

    TEST_ASSERT(fsm.fsm.current_state == IDLE_INCENDIO);
}

void
test_fsm_DATA_SENSOR_POSIBLE_INCENDIOToINCENDIO() {
    fsm_deteccion_incendio_t fsm;
    QueueHandle_t datovalidoqueue, datossensoresqueue, incendioqueue, muestreorapidoqueue;
    float temperatura, humedad, gases;
    
    fsm_deteccion_incendio_init(&fsm, &temperatura, &humedad, &gases, &datovalidoqueue, &datossensoresqueue, &incendioqueue, &muestreorapidoqueue);

    fsm.fsm.current_state = DATA_SENSOR_POSIBLE_INCENDIO;

    NoHayPeligro_ExpectAnyArgsAndReturn(false);
    HayIncendio_ExpectAnyArgsAndReturn(true);
    SendDatoIncendio_ExpectAnyArgs();

    fsm_fire(&(fsm.fsm));

    TEST_ASSERT(fsm.fsm.current_state == INCENDIO);
}

void
test_fsm_DATA_SENSOR_POSIBLE_INCENDIONoChange() {
    fsm_deteccion_incendio_t fsm;
    QueueHandle_t datovalidoqueue, datossensoresqueue, incendioqueue, muestreorapidoqueue;
    float temperatura, humedad, gases;
    
    fsm_deteccion_incendio_init(&fsm, &temperatura, &humedad, &gases, &datovalidoqueue, &datossensoresqueue, &incendioqueue, &muestreorapidoqueue);

    fsm.fsm.current_state = DATA_SENSOR_POSIBLE_INCENDIO;

    NoHayPeligro_ExpectAnyArgsAndReturn(false);
    HayIncendio_ExpectAnyArgsAndReturn(false);

    fsm_fire(&(fsm.fsm));

    TEST_ASSERT(fsm.fsm.current_state == DATA_SENSOR_POSIBLE_INCENDIO);
}

void
test_fsm_INCENDIOToINCENDIO() {
    fsm_deteccion_incendio_t fsm;
    QueueHandle_t datovalidoqueue, datossensoresqueue, incendioqueue, muestreorapidoqueue;
    float temperatura, humedad, gases;
    
    fsm_deteccion_incendio_init(&fsm, &temperatura, &humedad, &gases, &datovalidoqueue, &datossensoresqueue, &incendioqueue, &muestreorapidoqueue);

    fsm.fsm.current_state = INCENDIO;

    ReceiveDatoValidoIncendio_ExpectAnyArgsAndReturn(true);
    GetDataFromFsmSensor_ExpectAnyArgs();

    fsm_fire(&(fsm.fsm));

    TEST_ASSERT(fsm.fsm.current_state == INCENDIO);
}

void
test_fsm_INCENDIOToIDLE_INCENDIO() {
    fsm_deteccion_incendio_t fsm;
    QueueHandle_t datovalidoqueue, datossensoresqueue, incendioqueue, muestreorapidoqueue;
    float temperatura, humedad, gases;
    
    fsm_deteccion_incendio_init(&fsm, &temperatura, &humedad, &gases, &datovalidoqueue, &datossensoresqueue, &incendioqueue, &muestreorapidoqueue);

    fsm.fsm.current_state = INCENDIO;

    ReceiveDatoValidoIncendio_ExpectAnyArgsAndReturn(false);
    Incendio_to_Idle_ExpectAnyArgsAndReturn(true);
    BackToIdle_ExpectAnyArgs();

    fsm_fire(&(fsm.fsm));

    TEST_ASSERT(fsm.fsm.current_state == IDLE_INCENDIO);
}

void
test_fsm_INCENDIOTNoChange() {
    fsm_deteccion_incendio_t fsm;
    QueueHandle_t datovalidoqueue, datossensoresqueue, incendioqueue, muestreorapidoqueue;
    float temperatura, humedad, gases;
    
    fsm_deteccion_incendio_init(&fsm, &temperatura, &humedad, &gases, &datovalidoqueue, &datossensoresqueue, &incendioqueue, &muestreorapidoqueue);

    fsm.fsm.current_state = INCENDIO;

    ReceiveDatoValidoIncendio_ExpectAnyArgsAndReturn(false);
    Incendio_to_Idle_ExpectAnyArgsAndReturn(false);

    fsm_fire(&(fsm.fsm));

    TEST_ASSERT(fsm.fsm.current_state == INCENDIO);
}
