#include "unity.h"
#include <string.h>
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
    fsm_t zero;
    fsm_t f;
    bzero(&zero, sizeof(fsm_t));
    bzero(&f, sizeof(fsm_t));

    fsm_init_sensores((fsm_t *)&f);
    TEST_ASSERT(memcmp(&zero, &f, sizeof(fsm_t)) != 0);
}

void test_fsm_fsmInitCheckInitialState(){
    fsm_t f;
    fsm_init_sensores((fsm_t *)&f);

    TEST_ASSERT(f.current_state == IDLE);
}

void test_fsm_fsmCheckTransitionIdleToMedidas(){
    fsm_t f;
    fsm_init_sensores((fsm_t *)&f);

    checkStart_ON_ExpectAnyArgsAndReturn(True);
	Activa_Sensores_ExpectAnyArgs();
    fsm_fire(&f);

    TEST_ASSERT(f.current_state == MEDIDAS);
}

void test_fsm_fsmCheckNotTransitionIdleToMedidas(){
    fsm_t f;
    fsm_init_sensores((fsm_t *)&f);

    checkStart_ON_ExpectAnyArgsAndReturn(False);
    fsm_fire(&f);

    TEST_ASSERT(f.current_state == IDLE);
}

void test_fsm_fsmCheckTransitionMedidasToLectura(){
    fsm_t f;
    fsm_init_sensores((fsm_t *)&f);
	
	f.current_state = MEDIDAS;
	Deadline_ExpectAnyArgsAndReturn(True);
	Lectura_Sensores_ExpectAnyArgs();
	fsm_fire(&f);

    TEST_ASSERT(f.current_state == LECTURA);
}

void test_fsm_fsmCheckNotTransitionMedidasToLectura(){
    fsm_t f;
    fsm_init_sensores((fsm_t *)&f);
	
	f.current_state = MEDIDAS;
	Deadline_ExpectAnyArgsAndReturn(False);
	fsm_fire(&f);

    TEST_ASSERT(f.current_state == MEDIDAS);	
}

void test_fsm_fsmCheckTransitionLecturaToValidacion(){
    fsm_t f;
    fsm_init_sensores((fsm_t *)&f);
	
	f.current_state = LECTURA;
	LecturaFinalizadaOK_ExpectAnyArgsAndReturn(True);
	Send_Data_ExpectAnyArgs();
	fsm_fire(&f);

    TEST_ASSERT(f.current_state == VALIDACION);
}

void test_fsm_fsmCheckNotTransitionLecturaToCalculos(){
    fsm_t f;
    fsm_init_sensores((fsm_t *)&f);
	
	f.current_state = LECTURA;
	LecturaFinalizadaOK_ExpectAnyArgsAndReturn(False);
	fsm_fire(&f);

    TEST_ASSERT(f.current_state == LECTURA);
}

void test_fsm_fsmCheckTransitionCalculosToIdle(){
    fsm_t f;
    fsm_init_sensores((fsm_t *)&f);
	
	f.current_state = VALIDACION;
	ReturnTrue_ExpectAnyArgsAndReturn(True);
	Apagar_Sensores_ExpectAnyArgs();
	fsm_fire(&f);

    TEST_ASSERT(f.current_state == IDLE);
}