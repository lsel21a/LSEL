#include "fsm_sensores.h"
#include "sensores.h"
#include <stddef.h>


static fsm_trans_t sensores_tt[] = {
    {Idle, checkStart_ON, Medidas, Activa_Sensores},
    {Medidas, Return_Always_True, Lectura, Lectura_Sensores},
    {Lectura, Return_Always_True, Calculos, Dato_Valido},
    {Calculos, Return_Always_True, Idle, Dato_NoValido},
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

    Activa_Sensor();

}

void Lectura_Sensores(fsm_t *this){

    Lee_Sensor();

}

void Dato_Valido(fsm_t *this){

    DatoValido = 1;

}

void Dato_NoValido(fsm_t *this){

    DatoValido = 0;

}

int Return_Always_True(fsm_t *this){
    return TRUE;
}
