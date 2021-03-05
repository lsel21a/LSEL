#include "drivers.h"


static int temporizador = 0, lectura = 0;

int Activa_Temporizador(void){
	
	temporizador = 0; // Temporizado contando
	return 0;

}

int Activa_Sensor(void){
	
	return 0;

}

int Estado_Temporizador(void){

	return temporizador;

}

int Lee_Sensor(void){
	
	lectura = 1; // Lectura finalizada
    return 0;

}

int Estado_Lectura(void){
	
	return lectura;

}

int checkStart(void){

    return 1;

}