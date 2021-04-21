#include "fsm_sensores.h"
#include "drivers.h"
#include <stddef.h>


static fsm_trans_t sensores_tt[] = {
    {Idle, checkStart_ON, Medidas, Activa_Sensores},
    {Medidas, Deadline, Lectura, Lectura_Sensores},
    {Lectura, LecturaFinalizadaOK, Calculos, Dato_Valido},
    {Calculos, ReturnTrue, Idle, Dato_NoValido},
    {-1, NULL, -1, NULL}
};


void fsm_init_sensores(fsm_t *this){

    fsm_init(this, sensores_tt);
   
}
