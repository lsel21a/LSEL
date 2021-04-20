#include "drivers_fsm_sensores.h"

static bool deadline = false;
// static bool valores_sensores = false;
static bool datos_validos = false;
// static bool Start = false;


int checkStart_ON(fsm_t *this)
{
    fsm_sensores_t *fp = (fsm_sensores_t*) this;
    bool tick = 0;

    if( fp->tickQueue != 0 )
    {
       // Receive a message on the created queue.  
       xQueueReceive(fp->tickQueue, &(tick), portMAX_DELAY);
       printf("Tick recibido en CheckStart_ON. \n");
    }
    else
    {
        printf("Error: cola de incendio no esta correctamente creada.\n");
        return 0;
    };
    return tick;
};

void Activa_Sensores(fsm_t *this){

    //sensors_status_t result;
    printf("Activación sensores.\n");

    if(sensors_init(&devices[0]) != SENSORS_OK)
    {
        printf("Error en activación sensor 1.\n");
    }
    
    if(sensors_init(&devices[1]) != SENSORS_OK)
    {
        printf("Error en activación sensor 2.\n");
    }
    
    if(sensors_init(&devices[2]) != SENSORS_OK)
    {
        printf("Error en activación sensor 3.\n");
    }

    printf("Activación de sensores OK. \n");
    deadline = true;
}

int Deadline(fsm_t *this){
    
    printf("Lectura deadline.\n");
    return deadline;

}

void Lectura_Sensores(fsm_t *this){

    deadline = false;
    printf("Lectura de los sensores.\n");

    if(get_data(&devices[0]) != SENSORS_OK)
    {
        printf("Error en lectura sensor 1.\n");
    }

    if(get_data(&devices[1]) != SENSORS_OK)
    {
        printf("Error en lectura sensor 2.\n");
    }

    if(get_data(&devices[2]) != SENSORS_OK)
    {
        printf("Error en lectura sensor 3.\n");
    }
    //valores_sensores = true;
    printf("Lectura de los sensores OK.\n");
}

int LecturaFinalizadaOK(fsm_t *this){

    fsm_sensores_t *fp = (fsm_sensores_t*) this;

    if( fp->datoValidoQueue != 0 )
    {
        bool DatoValido = true;
        // Send a bool (datoValido) to fsm_deteccion_incendio. 
        if( xQueueGenericSend( fp->datoValidoQueue, ( void * ) &DatoValido, ( TickType_t ) 0, queueSEND_TO_BACK) != pdPASS  )
        {
            printf("Error en el envio del dato válido.\n");
            // Failed to post the message.
            return 0;
        }
        else{
            printf("Envío dato válido por cola.\n");
            return 1;
        }
    }
    else
    {
        printf("Cola de dato válido no esta correctamente creada.\n");
        return 0;
    }
	
}

void Send_Data(fsm_t *this){

    fsm_sensores_t *fp = (fsm_sensores_t*) this;

    sensors_data_t txDataSensor[NUM_SENSORS];

    int i;
    for (i=0;i<NUM_SENSORS;i++){
        txDataSensor[i] = devices[i].data;
    }
    xQueueSend(fp->datosSensoresQueue, txDataSensor, ( TickType_t )0);
    
    printf("Datos de los sensores enviados.\n");
    datos_validos = true;
}


void Apagar_Sensores(fsm_t *this){
    
    datos_validos = false;
    printf("Apagado de los sensores.\n");

    if(sleep_data(&devices[0]) != SENSORS_OK)
    {
        printf("Error en disactivación sensor 1.\n");
    }

    if(sleep_data(&devices[1]) != SENSORS_OK)
    {
        printf("Error en disactivación sensor 2.\n");
    }

    if(sleep_data(&devices[2]) != SENSORS_OK)
    {
        printf("Error en disactivación sensor 3.\n");
    }
}

int ReturnTrue(fsm_t *this){
	
	return 1;
	
}