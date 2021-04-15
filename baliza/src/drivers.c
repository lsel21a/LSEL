#include "drivers.h"

static bool deadline = false;
static bool valores_sensores = false;
static bool datos_validos = false;
static bool Start = false;


int checkStart_ON(fsm_t *this)
{

    fsm_sensores_t *fp = (fsm_sensores_t*) this;

   if( *(fp->tickQueue) != 0 )
   {
        bool * rxMuestreoRapido;
        bool muestreo_rapido = false;
        TickType_t xDelay;

       // Receive a message on the created queue.  If a
       // message is not immediately available we use the default sampling period.
       if( xQueueReceive( *(fp->tickQueue), &( rxMuestreoRapido ), ( TickType_t ) 0))       //( TickType_t ) 10 ) )
       {
           printf("Tick recibido en la cola.\n");
           // rxDatoValido now points to the bool variable posted by LecturaFinalizadaOK from fsm_sensores (drivers.c file).
           if( * rxMuestreoRapido == true )
            {
                muestreo_rapido = true;
            }
            else
            {
                muestreo_rapido = false;
            }
       }
   }
   else
   {
        printf("Tick no recibido en la cola.\n");
        return 0;
   }

}

void Activa_Sensores(fsm_t *this){

    //sensors_status_t result;
    printf("Activación sensores.\n");

    if(sensors_init(&devices[0]) != SENSORS_OK)
    {
        //printf(result)?;
        printf("Error en activación sensor 1.\n");
    }
    
    if(sensors_init(&devices[1]) != SENSORS_OK)
    {
        //printf(result)?;
        printf("Error en activación sensor 2.\n");
    }
    
    if(sensors_init(&devices[2]) != SENSORS_OK)
    {
        //printf(result)?;
        printf("Error en activación sensor 3.\n");
    }

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
        //printf(result)?
    }

    if(get_data(&devices[1]) != SENSORS_OK)
    {
        //printf(result)?
    }

    if(get_data(&devices[2]) != SENSORS_OK)
    {
        //printf(result)?
    }
    //valores_sensores = true;
}

int LecturaFinalizadaOK(fsm_t *this){

    fsm_sensores_t *fp = (fsm_sensores_t*) this;

    if( *fp->datoValidoQueue != 0 )
    {
        bool DatoValido = true;
        // Send a bool (datoValido) to fsm_deteccion_incendio. 
        if( xQueueGenericSend( *(fp->datoValidoQueue), ( void * ) &DatoValido, ( TickType_t ) 0, queueSEND_TO_BACK) != pdPASS  )
        {
            printf("Error en el envñio del datos válido.\n");
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

    struct bme68x_data *txDataSensor;
    int i = 0;
    //valores_sensores = false;
    
    for(i=0; i<3; i++)   //3 is the size of the devices array
    {
        txDataSensor = & (devices[i].data);
        // Send a pointer to each data struct of each sensor to fsm_deteccion_incendio. 
        xQueueSend( *(fp->datosSensoresQueue), ( void * ) &txDataSensor, ( TickType_t ) 0 );     
    }

    printf("Datos de los sensores enviados.\n");
    datos_validos = true;
}


void Apagar_Sensores(fsm_t *this){
    
    datos_validos = false;
    printf("Apagado de los sensores.\n");

    if(sleep_data(&devices[0]) != SENSORS_OK)
    {
        //printf(result)?
    }

    if(sleep_data(&devices[1]) != SENSORS_OK)
    {
        //printf(result)?
    }

    if(sleep_data(&devices[2]) != SENSORS_OK)
    {
        //printf(result)?
    }
}

int ReturnTrue(fsm_t *this){
	
	return 1;
	
}