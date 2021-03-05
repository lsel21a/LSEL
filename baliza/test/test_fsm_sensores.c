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

    checkStart_ExpectAndReturn(True);
	Activa_Temporizador_ExpectAndReturn(False);
    Activa_Sensor_ExpectAndReturn(False);
    fsm_fire(&f);

    TEST_ASSERT(f.current_state == Medidas);
}

void test_fsm_fsmCheckNotTransitionIdleToMedidas(){
    fsm_t f;
    fsm_init_sensores((fsm_t *)&f);

    checkStart_ExpectAndReturn(False);
    fsm_fire(&f);

    TEST_ASSERT(f.current_state == Idle);
}

void test_fsm_fsmCheckTransitionMedidasToLectura(){
    fsm_t f;
    fsm_init_sensores((fsm_t *)&f);
	
	f.current_state = Medidas;
	Estado_Temporizador_ExpectAndReturn(True);
	Lee_Sensor_ExpectAndReturn(True);
	fsm_fire(&f);

    TEST_ASSERT(f.current_state == Lectura);
}

void test_fsm_fsmCheckTransitionMedidasToLecturaReturnLecturaSensores(){
    
}

void test_fsm_fsmCheckTransitionLecturaToCalculos(){
    
}

void test_fsm_fsmCheckTransitionLecturaToCalculosReturnDatoValido(){
    
}

void test_fsm_fsmCheckTransitionCalculosToIdle(){
    
}

void test_fsm_fsmCheckTransitionCalculosToIdleReturnDatoNoValido(){
    
}