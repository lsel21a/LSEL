#include "drivers_fsm_sensores.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

static bool deadline = false;
// static bool valores_sensores = false;
static bool datos_validos = false;
// static bool Start = false;

sensors_config_t devices[NUM_SENSORS];

int checkStart_ON(fsm_t *this)
{
    fsm_sensores_t *fp = (fsm_sensores_t*) this;
    bool tick = false;

    if( *(fp->tickQueue) != 0 )
    {
        // Receive a message on the created queue.  
        xQueueReceive( *(fp->tickQueue), (void *) &tick, ( TickType_t ) 0);
#ifdef DEBUG_PRINT_ENABLE
        printf("Tick %d recibido en CheckStart_ON. \n", tick);
#endif /* DEBUG_PRINT_ENABLE */
    }
    else
    {
#ifdef DEBUG_PRINT_ENABLE
        printf("Error: cola de incendio no esta correctamente creada.\n");
#endif /* DEBUG_PRINT_ENABLE */
        return 0;
    };
    return tick;
};

void Activa_Sensores(fsm_t *this){

    //sensors_status_t result;
#ifdef DEBUG_PRINT_ENABLE
    printf("Activación sensores.\n");
#endif /* DEBUG_PRINT_ENABLE */

    int i;
    for (i=0; i<NUM_SENSORS; i++){
        if(sensors_init(&devices[i]) != SENSORS_OK)
        {
#ifdef DEBUG_PRINT_ENABLE
            printf("Error en activación sensor %d.\n", i);
#endif /* DEBUG_PRINT_ENABLE */
        }
    }
#ifdef DEBUG_PRINT_ENABLE
    printf("Activación de sensores OK.\n");
#endif /* DEBUG_PRINT_ENABLE */
    deadline = true;
}

int Deadline(fsm_t *this){
    
#ifdef DEBUG_PRINT_ENABLE
    printf("Lectura deadline.\n");
#endif /* DEBUG_PRINT_ENABLE */
    return deadline;

}

void Lectura_Sensores(fsm_t *this){

    deadline = false;
#ifdef DEBUG_PRINT_ENABLE
    printf("Lectura de los sensores.\n");
#endif /* DEBUG_PRINT_ENABLE */

    int i;
    for(i=0; i<NUM_SENSORS; i++){
        if(get_data(&devices[i]) != SENSORS_OK)
        {
#ifdef DEBUG_PRINT_ENABLE
            printf("Error en lectura sensor %d.\n", i);
#endif /* DEBUG_PRINT_ENABLE */
        }
#ifdef DEBUG_PRINT_ENABLE
        printf("Se ha leído %d ºC en el sensor %d.\n", (int)devices[i].data.temperature, i);
#endif /* DEBUG_PRINT_ENABLE */
    }

#ifdef DEBUG_PRINT_ENABLE
    printf("Lectura de los sensores OK.\n");
#endif /* DEBUG_PRINT_ENABLE */
}

int LecturaFinalizadaOK(fsm_t *this){

    fsm_sensores_t *fp = (fsm_sensores_t*) this;

    if( *(fp->datoValidoQueue) != 0 )
    {
        bool DatoValido = true;
        // Send a bool (datoValido) to fsm_deteccion_incendio. 
        if( xQueueGenericSend( *(fp->datoValidoQueue), ( void * ) &DatoValido, ( TickType_t ) 0, queueSEND_TO_BACK) != pdPASS  )
        {
#ifdef DEBUG_PRINT_ENABLE
            printf("Error en el envio del dato válido.\n");
#endif /* DEBUG_PRINT_ENABLE */
            // Failed to post the message.
            return 0;
        }
        else{
#ifdef DEBUG_PRINT_ENABLE
            printf("Envío dato válido por cola.\n");
#endif /* DEBUG_PRINT_ENABLE */
            return 1;
        }
    }
    else
    {
#ifdef DEBUG_PRINT_ENABLE
        printf("Cola de dato válido no esta correctamente creada.\n");
#endif /* DEBUG_PRINT_ENABLE */
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
    
#ifdef DEBUG_PRINT_ENABLE
    printf("Datos de los sensores enviados.\n");
#endif /* DEBUG_PRINT_ENABLE */
    datos_validos = true;
}


void Apagar_Sensores(fsm_t *this){
    
    datos_validos = false;
#ifdef DEBUG_PRINT_ENABLE
    printf("Apagado de los sensores.\n");
#endif /* DEBUG_PRINT_ENABLE */

    int i;
    for(i=0; i<NUM_SENSORS; i++){
        if(sleep_data(&devices[i]) != SENSORS_OK)
        {
#ifdef DEBUG_PRINT_ENABLE
            printf("Error en disactivación sensor %d.\n", i);
#endif /* DEBUG_PRINT_ENABLE */
        }
    }
}

int ReturnTrue(fsm_t *this){
	
	return 1;
	
}