#include "unity.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "fsm.h"
#include "fsm_timer/fsm_timer.h"
#include "mock_drivers_timer.h"


#define True 1
#define False 0

void setUp(void)
{

}

void tearDown(void)
{

}

void test_fsm_timer_fsmInitFillsStructWithSomething(){
    fsm_timer_t zero;
    fsm_timer_t f;
    memset(&zero, 0, sizeof(fsm_timer_t));
    memset(&f, 0, sizeof(fsm_timer_t));

    QueueHandle_t q1, q2;

    fsm_timer_init(&f, &q1, &q2);
    TEST_ASSERT(memcmp(&zero, &f, sizeof(fsm_timer_t)) != 0);
}

void test_fsm_fsmInitCheckInitialState(){
    fsm_timer_t f;
    QueueHandle_t q1, q2;
    fsm_timer_init(&f, &q1, &q2);

    TEST_ASSERT(f.fsm.current_state == MUESTREO_NORMAL);
}

void test_fsm_fsmCheckTransitionMUESTREO_NORMALToMUESTREORAPIDO(){
    fsm_timer_t f;
    QueueHandle_t q1, q2;
    fsm_timer_init(&f, &q1, &q2);

    ReceiveMuestreoRapido_ExpectAnyArgsAndReturn(True);
	 
    fsm_fire(&(f.fsm));

    TEST_ASSERT(f.fsm.current_state == MUESTREO_RAPIDO);
}

void test_fsm_fsmCheckNotTransitionMUESTREO_NORMALToMUESTREO_RAPIDO(){
    fsm_timer_t f;
    QueueHandle_t q1, q2;
    fsm_timer_init(&f, &q1, &q2);

    ReceiveMuestreoRapido_ExpectAnyArgsAndReturn(False);
    WaitNormal_ExpectAnyArgsAndReturn(True);
    SendTick_ExpectAnyArgs();
    fsm_fire(&(f.fsm));

    TEST_ASSERT(f.fsm.current_state == MUESTREO_NORMAL);
}

void test_fsm_fsmCheckNotNotTransitionMUESTREO_NORMALToMUESTREO_NORMAL(){
    fsm_timer_t f;
    QueueHandle_t q1, q2;
    fsm_timer_init(&f, &q1, &q2);

    ReceiveMuestreoRapido_ExpectAnyArgsAndReturn(False);
    WaitNormal_ExpectAnyArgsAndReturn(False);
    fsm_fire(&(f.fsm));

    TEST_ASSERT(f.fsm.current_state == MUESTREO_NORMAL);
}

void test_fsm_fsmCheckTransitionMUESTREO_RAPIDOToMUESTREO_NORMAL(){
    fsm_timer_t f;
    QueueHandle_t q1, q2;
    fsm_timer_init(&f, &q1, &q2);
	
	f.fsm.current_state = MUESTREO_RAPIDO;
	ReceiveMuestreoNormal_ExpectAnyArgsAndReturn(True);
	 
	fsm_fire(&(f.fsm));

    TEST_ASSERT(f.fsm.current_state == MUESTREO_NORMAL);
}

void test_fsm_fsmCheckNotTransitionMUESTREO_RAPIDOToMUESTREO_RAPIDO(){
    fsm_timer_t f;
    QueueHandle_t q1, q2;
    fsm_timer_init(&f, &q1, &q2);
	
	f.fsm.current_state = MUESTREO_RAPIDO;
	ReceiveMuestreoNormal_ExpectAnyArgsAndReturn(False);
	WaitRapido_ExpectAnyArgsAndReturn(True);
    SendTick_ExpectAnyArgs();
	fsm_fire(&(f.fsm));

    TEST_ASSERT(f.fsm.current_state == MUESTREO_RAPIDO);
}

void test_fsm_fsmCheckNotMUESTREO_RAPIDOToMUESTREO_RAPIDO(){
    fsm_timer_t f;
    QueueHandle_t q1, q2;
    fsm_timer_init(&f, &q1, &q2);
	
	f.fsm.current_state = MUESTREO_RAPIDO;
	ReceiveMuestreoNormal_ExpectAnyArgsAndReturn(False);
    WaitRapido_ExpectAnyArgsAndReturn(False);
	fsm_fire(&(f.fsm));

    TEST_ASSERT(f.fsm.current_state == MUESTREO_RAPIDO);
}