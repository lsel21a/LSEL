#include "unity.h"
#include <string.h>
#include "fsm.h"
#include "fsm_sensores.h"
#include "mock_drivers.h"

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

    TEST_ASSERT(f.current_state == Idle);
}

void test_fsm_fsmCheckTransitionIdleToMedidas(){
    fsm_t f;
    fsm_init_sensores((fsm_t *)&f);

    checkStart_ON_ExpectAnyArgsAndReturn(True);
	Activa_Sensores_ExpectAnyArgs();
    fsm_fire(&f);

    TEST_ASSERT(f.current_state == Medidas);
}

void test_fsm_fsmCheckNotTransitionIdleToMedidas(){
    fsm_t f;
    fsm_init_sensores((fsm_t *)&f);

    checkStart_ON_ExpectAnyArgsAndReturn(False);
    fsm_fire(&f);

    TEST_ASSERT(f.current_state == Idle);
}

void test_fsm_fsmCheckTransitionMedidasToLectura(){
    fsm_t f;
    fsm_init_sensores((fsm_t *)&f);
	
	f.current_state = Medidas;
	Deadline_ExpectAnyArgsAndReturn(True);
	Lectura_Sensores_ExpectAnyArgs();
	fsm_fire(&f);

    TEST_ASSERT(f.current_state == Lectura);
}

void test_fsm_fsmCheckNotTransitionMedidasToLectura(){
    fsm_t f;
    fsm_init_sensores((fsm_t *)&f);
	
	f.current_state = Medidas;
	Deadline_ExpectAnyArgsAndReturn(False);
	fsm_fire(&f);

    TEST_ASSERT(f.current_state == Medidas);	
}

void test_fsm_fsmCheckTransitionLecturaToCalculos(){
    fsm_t f;
    fsm_init_sensores((fsm_t *)&f);
	
	f.current_state = Lectura;
	LecturaFinalizadaOK_ExpectAnyArgsAndReturn(True);
	Dato_Valido_ExpectAnyArgs();
	fsm_fire(&f);

    TEST_ASSERT(f.current_state == Calculos);
}

void test_fsm_fsmCheckNotTransitionLecturaToCalculos(){
    fsm_t f;
    fsm_init_sensores((fsm_t *)&f);
	
	f.current_state = Lectura;
	LecturaFinalizadaOK_ExpectAnyArgsAndReturn(False);
	fsm_fire(&f);

    TEST_ASSERT(f.current_state == Lectura);
}

void test_fsm_fsmCheckTransitionCalculosToIdle(){
    fsm_t f;
    fsm_init_sensores((fsm_t *)&f);
	
	f.current_state = Calculos;
	ReturnTrue_ExpectAnyArgsAndReturn(True);
	Dato_NoValido_ExpectAnyArgs();
	fsm_fire(&f);

    TEST_ASSERT(f.current_state == Idle);
}