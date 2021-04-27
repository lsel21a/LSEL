#include "drivers_fsm_sensores.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

static bool deadline = false;
// static bool valores_sensores = false;
static bool datos_validos = false;
// static bool Start = false;


int checkStart_ON(fsm_t *this)
{
    fsm_sensores_t *fp = (fsm_sensores_t*) this;
    bool tick = false;

    if( *(fp->tickQueue) != 0 )
    {
       // Receive a message on the created queue.  
       xQueueReceive( *(fp->tickQueue), (void *) &tick, ( TickType_t ) 0);
       printf("Tick %d recibido en CheckStart_ON. \n", tick);
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

    int i;
    for (i=0; i<NUM_SENSORS; i++){
        if(sensors_init(&devices[i]) != SENSORS_OK)
        {
            printf("Error en activación sensor %d.\n", i);
        }
    }
    printf("Activación de sensores OK.\n");
    deadline = true;
}

int Deadline(fsm_t *this){
    
    printf("Lectura deadline.\n");
    return deadline;

}

void Lectura_Sensores(fsm_t *this){

    deadline = false;
    printf("Lectura de los sensores.\n");

    int i;
    for(i=0; i<NUM_SENSORS; i++){
        if(get_data(&devices[i]) != SENSORS_OK)
        {
            printf("Error en lectura sensor %d.\n", i);
        }
        printf("Se ha leído %d ºC en el sensor %d.\n", (int)devices[i].data.temperature, i);
    }

    printf("Lectura de los sensores OK.\n");
}

int LecturaFinalizadaOK(fsm_t *this){

    fsm_sensores_t *fp = (fsm_sensores_t*) this;

    if( *(fp->datoValidoQueue) != 0 )
    {
        bool DatoValido = true;
        // Send a bool (datoValido) to fsm_deteccion_incendio. 
        if( xQueueGenericSend( *(fp->datoValidoQueue), ( void * ) &DatoValido, ( TickType_t ) 0, queueSEND_TO_BACK) != pdPASS  )
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
	return 0;
}

void Send_Data(fsm_t *this){

    fsm_sensores_t *fp = (fsm_sensores_t*) this;

    sensors_data_t txDataSensor[NUM_SENSORS];

    int i;
    for (i=0;i<NUM_SENSORS;i++){
        txDataSensor[i] = devices[i].data;
    }
    xQueueSend( *(fp->datosSensoresQueue), ( void * ) txDataSensor, ( TickType_t ) 0);
    
    printf("Datos de los sensores enviados.\n");
    datos_validos = true;
}


void Apagar_Sensores(fsm_t *this){
    
    datos_validos = false;
    printf("Apagado de los sensores.\n");

    int i;
    for(i=0; i<NUM_SENSORS; i++){
        if(sleep_data(&devices[i]) != SENSORS_OK)
        {
            printf("Error en disactivación sensor %d.\n", i);
        }
    }
}

int ReturnTrue(fsm_t *this){
	
	return 1;
	
}