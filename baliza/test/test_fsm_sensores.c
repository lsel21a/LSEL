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

}

void test_fsm_fsmCheckTransitionIdleToMedidas(){

}

void test_fsm_fsmCheckNotTransitionIdleToMedidas(){

}

void test_fsm_fsmCheckTransitionIdleToMedidasReturnActivaSensores(){
    
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












