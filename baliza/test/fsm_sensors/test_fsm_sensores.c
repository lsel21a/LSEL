#include "unity.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "fsm.h"
#include "fsm_sensores.h"
#include "mock_drivers_fsm_sensores.h"


#define True 1
#define False 0

void setUp(void)
{

}

void tearDown(void)
{

}

void test_fsm_sensores_fsmInitFillsStructWithSomething(){
    fsm_sensores_t zero;
    fsm_sensores_t f;
    memset(&zero, 0, sizeof(fsm_sensores_t));
    memset(&f, 0, sizeof(fsm_sensores_t));

    QueueHandle_t q1, q2, q3;

    fsm_init_sensores(&f, &q1, &q2, &q3);
    TEST_ASSERT(memcmp(&zero, &f, sizeof(fsm_sensores_t)) != 0);
    TEST_ASSERT_EQUAL(f.datoValidoQueue, &(q1));
    TEST_ASSERT_EQUAL(f.datosSensoresQueue, &(q2));
    TEST_ASSERT_EQUAL(f.tickQueue, &(q3));
}

void test_fsm_fsmInitCheckInitialState(){
    fsm_sensores_t f;
    QueueHandle_t q1, q2, q3;
    fsm_init_sensores(&f, &q1, &q2, &q3);

    TEST_ASSERT(f.fsm.current_state == IDLE);
}

void test_fsm_fsmCheckTransitionIdleToMedidas(){
    fsm_sensores_t f;
    QueueHandle_t q1, q2, q3;
    fsm_init_sensores(&f, &q1, &q2, &q3);

    checkStart_ON_ExpectAnyArgsAndReturn(True);
	Activa_Sensores_ExpectAnyArgs();
    fsm_fire(&(f.fsm));

    TEST_ASSERT(f.fsm.current_state == MEDIDAS);
}

void test_fsm_fsmCheckNotTransitionIdleToMedidas(){
    fsm_sensores_t f;
    QueueHandle_t q1, q2, q3;
    fsm_init_sensores(&f, &q1, &q2, &q3);

    checkStart_ON_ExpectAnyArgsAndReturn(False);
    fsm_fire(&(f.fsm));

    TEST_ASSERT(f.fsm.current_state == IDLE);
}

void test_fsm_fsmCheckTransitionMedidasToLectura(){
    fsm_sensores_t f;
    QueueHandle_t q1, q2, q3;
    fsm_init_sensores(&f, &q1, &q2, &q3);
	
	f.fsm.current_state = MEDIDAS;
	Deadline_ExpectAnyArgsAndReturn(True);
	Lectura_Sensores_ExpectAnyArgs();
	fsm_fire(&(f.fsm));

    TEST_ASSERT(f.fsm.current_state == LECTURA);
}

void test_fsm_fsmCheckNotTransitionMedidasToLectura(){
    fsm_sensores_t f;
    QueueHandle_t q1, q2, q3;
    fsm_init_sensores(&f, &q1, &q2, &q3);
	
	f.fsm.current_state = MEDIDAS;
	Deadline_ExpectAnyArgsAndReturn(False);
	fsm_fire(&(f.fsm));

    TEST_ASSERT(f.fsm.current_state == MEDIDAS);	
}

void test_fsm_fsmCheckTransitionLecturaToValidacion(){
    fsm_sensores_t f;
    QueueHandle_t q1, q2, q3;
    fsm_init_sensores(&f, &q1, &q2, &q3);
	
	f.fsm.current_state = LECTURA;
	LecturaFinalizadaOK_ExpectAnyArgsAndReturn(True);
	Send_Data_ExpectAnyArgs();
	fsm_fire(&(f.fsm));

    TEST_ASSERT(f.fsm.current_state == VALIDACION);
}

void test_fsm_fsmCheckNotTransitionLecturaToCalculos(){
    fsm_sensores_t f;
    QueueHandle_t q1, q2, q3;
    fsm_init_sensores(&f, &q1, &q2, &q3);
	
	f.fsm.current_state = LECTURA;
	LecturaFinalizadaOK_ExpectAnyArgsAndReturn(False);
	fsm_fire(&(f.fsm));

    TEST_ASSERT(f.fsm.current_state == LECTURA);
}

void test_fsm_fsmCheckTransitionCalculosToIdle(){
    fsm_sensores_t f;
    QueueHandle_t q1, q2, q3;
    fsm_init_sensores(&f, &q1, &q2, &q3);
	
	f.fsm.current_state = VALIDACION;
	ReturnTrue_ExpectAnyArgsAndReturn(True);
	Apagar_Sensores_ExpectAnyArgs();
	fsm_fire(&(f.fsm));

    TEST_ASSERT(f.fsm.current_state == IDLE);
}