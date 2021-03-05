#include "fsm_sensores.h"
#include "drivers.h"
#include <stddef.h>


static fsm_trans_t sensores_tt[] = {
    {Idle, checkStart_ON, Medidas, Activa_Sensores},
    {Medidas, Deadline, Lectura, Lectura_Sensores},
    {Lectura, LecturaFinalizadaOK, Calculos, Dato_Valido},
    {Calculos, 1, Idle, Dato_NoValido},
    {-1, NULL, -1, NULL}
};

static int DatoValido = 0;

void fsm_init_sensores(fsm_t *this){

    fsm_init(this, sensores_tt);
   
}

int checkStart_ON(fsm_t *this){

    return checkStart();

}

void Activa_Sensores(fsm_t *this){

	Activa_Temporizador();
    Activa_Sensor();

}

int Deadline(fsm_t *this){
	
	return Estado_Temporizador();

}

void Lectura_Sensores(fsm_t *this){

    Lee_Sensor();

}

void Dato_Valido(fsm_t *this){

    DatoValido = 1;

}

int LecturaFinalizadaOK(fsm_t *this){

	return Estado_Lectura();
	
}

void Dato_NoValido(fsm_t *this){

    DatoValido = 0;

}
