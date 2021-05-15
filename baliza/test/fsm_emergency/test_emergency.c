#include "unity.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "fsm.h"
#include "fsm_emergency/fsm_emergencia.h"
#include "mock_drivers_emergencia.h"


#define True 1
#define False 0

void setUp(void)
{

}

void tearDown(void)
{

}

void test_fsm_timer_fsmInitFillsStructWithSomething(){
    fsm_emergencia_t zero;
    fsm_emergencia_t f;
    memset(&zero, 0, sizeof(fsm_emergencia_t));
    memset(&f, 0, sizeof(fsm_emergencia_t));

    QueueHandle_t incendioQueue, solicitudDatosQueue, datosMQTTQueue;
    esp_mqtt_client_handle_t client;
    
    fsm_emergencia_init(&f, &incendioQueue, &solicitudDatosQueue, &datosMQTTQueue, &client);
    TEST_ASSERT(memcmp(&zero, &f, sizeof(fsm_emergencia_t)) != 0);
}

void test_fsm_fsmInitCheckInitialState(){
    fsm_emergencia_t f;
    
    QueueHandle_t incendioQueue, solicitudDatosQueue, datosMQTTQueue;
    esp_mqtt_client_handle_t client;
    
    fsm_emergencia_init(&f, &incendioQueue, &solicitudDatosQueue, &datosMQTTQueue, &client);

    TEST_ASSERT(f.fsm.current_state == IDLE_EMERGENCIA);
}

void test_fsm_fsmCheckTransitionIDLE_EMERGENCIAToEMERGENCIA(){
    fsm_emergencia_t f;

    QueueHandle_t incendioQueue, solicitudDatosQueue, datosMQTTQueue;
    esp_mqtt_client_handle_t client;
    
    fsm_emergencia_init(&f, &incendioQueue, &solicitudDatosQueue, &datosMQTTQueue, &client);

    SenalIncendio_ExpectAnyArgsAndReturn(True);
    EnvioSenalEmergencia_ExpectAnyArgs();
    fsm_fire(&(f.fsm));

    TEST_ASSERT(f.fsm.current_state == EMERGENCIA);
}

void test_fsm_fsmCheckNotTransitionIDLE_EMERGENCIAToEMERGENCIA(){
    fsm_emergencia_t f;
    
    QueueHandle_t incendioQueue, solicitudDatosQueue, datosMQTTQueue;
    esp_mqtt_client_handle_t client;
    
    fsm_emergencia_init(&f, &incendioQueue, &solicitudDatosQueue, &datosMQTTQueue, &client);

    SenalIncendio_ExpectAnyArgsAndReturn(False);
    SolicitudDatos_ExpectAnyArgsAndReturn(True);
    SendDatos_ExpectAnyArgs();
    fsm_fire(&(f.fsm));

    TEST_ASSERT(f.fsm.current_state == IDLE_EMERGENCIA);
}

void test_fsm_fsmCheckNotNotTransitionIDLE_EMERGENCIAToEMERGENCIA(){
    fsm_emergencia_t f;

    QueueHandle_t incendioQueue, solicitudDatosQueue, datosMQTTQueue;
    esp_mqtt_client_handle_t client;
    
    fsm_emergencia_init(&f, &incendioQueue, &solicitudDatosQueue, &datosMQTTQueue, &client);

    SenalIncendio_ExpectAnyArgsAndReturn(False);
    SolicitudDatos_ExpectAnyArgsAndReturn(False);
    SenalIncendioSil_ExpectAnyArgsAndReturn(True);
    fsm_fire(&(f.fsm));

    TEST_ASSERT(f.fsm.current_state == EMERGENCIA_SILENCIOSA);
}

void test_fsm_fsmCheckNotNotNotTransitionIDLE_EMERGENCIAToEMERGENCIA(){
    fsm_emergencia_t f;
    
    QueueHandle_t incendioQueue, solicitudDatosQueue, datosMQTTQueue;
    esp_mqtt_client_handle_t client;
    
    fsm_emergencia_init(&f, &incendioQueue, &solicitudDatosQueue, &datosMQTTQueue, &client);

    SenalIncendio_ExpectAnyArgsAndReturn(False);
    SolicitudDatos_ExpectAnyArgsAndReturn(False);
    SenalIncendioSil_ExpectAnyArgsAndReturn(False);
    fsm_fire(&(f.fsm));

    TEST_ASSERT(f.fsm.current_state == IDLE_EMERGENCIA);
}

void test_fsm_fsmCheckTransitionEMERGENCIAToIDLE_EMERGENCIA(){
    fsm_emergencia_t f;
    
    QueueHandle_t incendioQueue, solicitudDatosQueue, datosMQTTQueue;
    esp_mqtt_client_handle_t client;
    
    fsm_emergencia_init(&f, &incendioQueue, &solicitudDatosQueue, &datosMQTTQueue, &client);
	
	f.fsm.current_state = EMERGENCIA;
    SenalNoIncendio_ExpectAnyArgsAndReturn(True);

	fsm_fire(&(f.fsm));

    TEST_ASSERT(f.fsm.current_state == IDLE_EMERGENCIA);
}

void test_fsm_fsmCheckNotTransitionEMERGENCIAToIDLE_EMERGENCIA(){
    fsm_emergencia_t f;
    
    QueueHandle_t incendioQueue, solicitudDatosQueue, datosMQTTQueue;
    esp_mqtt_client_handle_t client;
    
    fsm_emergencia_init(&f, &incendioQueue, &solicitudDatosQueue, &datosMQTTQueue, &client);
	
	f.fsm.current_state = EMERGENCIA;
    SenalNoIncendio_ExpectAnyArgsAndReturn(False);
    SolicitudDatos_ExpectAnyArgsAndReturn(True);
    SendDatosYEmergencia_ExpectAnyArgs();
	fsm_fire(&(f.fsm));

    TEST_ASSERT(f.fsm.current_state == EMERGENCIA);
}

void test_fsm_fsmCheckTransitionEMERGENCIA_SILENCIOSAToIDLE_EMERGENCIA(){
    fsm_emergencia_t f;
    
    QueueHandle_t incendioQueue, solicitudDatosQueue, datosMQTTQueue;
    esp_mqtt_client_handle_t client;
    
    fsm_emergencia_init(&f, &incendioQueue, &solicitudDatosQueue, &datosMQTTQueue, &client);
	
	f.fsm.current_state = EMERGENCIA_SILENCIOSA;
    SenalNoIncendio_ExpectAnyArgsAndReturn(True);

	fsm_fire(&(f.fsm));

    TEST_ASSERT(f.fsm.current_state == IDLE_EMERGENCIA);
}

void test_fsm_fsmCheckNotTransitionEMERGENCIA_SILENCIOSAToEMERGENCIA_SILENCIOSA(){
    fsm_emergencia_t f;
    
    QueueHandle_t incendioQueue, solicitudDatosQueue, datosMQTTQueue;
    esp_mqtt_client_handle_t client;
    
    fsm_emergencia_init(&f, &incendioQueue, &solicitudDatosQueue, &datosMQTTQueue, &client);
	
	f.fsm.current_state = EMERGENCIA_SILENCIOSA;
    SenalNoIncendio_ExpectAnyArgsAndReturn(False);
    SolicitudDatos_ExpectAnyArgsAndReturn(True);
    SendDatosYEmergencia_ExpectAnyArgs();
	fsm_fire(&(f.fsm));

    TEST_ASSERT(f.fsm.current_state == EMERGENCIA_SILENCIOSA);
}