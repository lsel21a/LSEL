#include "unity.h"
#include <string.h>
#include "fsm.h"
#include "fsm_sensores.h"
#include "mock_sensores.h"


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

int test_Activa_Sensor_Callback(int NumCalls){
    switch(NumCalls){
        case 0: // Primera llamada a la función
            TEST_ASSERT(TRUE);
            break;
        default:
            TEST_ASSERT(FALSE);
    }
    return TRUE;
}

void test_fsm_fsmCheckTransitionIdleToMedidas(){
    fsm_t f;
    fsm_init_sensores((fsm_t *)&f);

    checkStart_ExpectAndReturn(TRUE);
    Activa_Sensor_Stub(test_Activa_Sensor_Callback);
    fsm_fire(&f);

    TEST_ASSERT(f.current_state == Medidas);
}

void test_fsm_fsmCheckNotTransitionIdleToMedidas(){
    fsm_t f;
    fsm_init_sensores((fsm_t *)&f);

    checkStart_ExpectAndReturn(FALSE);
    fsm_fire(&f);

    TEST_ASSERT(f.current_state == Idle);
}

void test_fsm_fsmCheckTransitionMedidasToLectura(){
    
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












