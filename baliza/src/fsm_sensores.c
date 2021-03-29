#include "fsm_sensores.h"
#include "drivers.h"
#include <stddef.h>




static fsm_trans_t sensores_tt[] = {
    {IDLE, checkStart_ON, MEDIDAS, Activa_Sensores},
    {MEDIDAS, Deadline, LECTURA, Lectura_Sensores},
    {LECTURA, LecturaFinalizadaOK, VALIDACION, Send_Data},
    {VALIDACION, ReturnTrue, IDLE, Apagar_Sensores},
    {-1, NULL, -1, NULL}
};


void fsm_init_sensores(fsm_t *this){

    fsm_init(this, sensores_tt);
   
}


