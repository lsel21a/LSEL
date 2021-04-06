#include "drivers.h"

static bool deadline = false;
static bool valores_sensores = false;
static bool datos_validos = false;
static bool Start = false;


int checkStart_ON(fsm_t *this)
{
    
   if( muestreoRapidoQueue != 0 )
   {
        bool * rxMuestreoRapido;
        bool muestreo_rapido = false;
        TickType_t xDelay;

       // Receive a message on the created queue.  If a
       // message is not immediately available we use the default sampling period.
       if( xQueueReceive( muestreoRapidoQueue, &( rxMuestreoRapido ), ( TickType_t ) 0))       //( TickType_t ) 10 ) )
       {
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

       if(muestreo_rapido == true)
       {
           xDelay= 2000 / portTICK_PERIOD_MS;
           vTaskDelay( xDelay );
           return 1;
       }
       else
       {
           xDelay= 15000 / portTICK_PERIOD_MS;
           vTaskDelay( xDelay );
           return 1;
       }
   }
   else
   {
       return 0;
   }

}

void Activa_Sensores(fsm_t *this){

    //sensors_status_t result;

    if(sensors_init(&devices[0]) != SENSORS_OK)
    {
        //printf(result)?;
    }
    
    if(sensors_init(&devices[1]) != SENSORS_OK)
    {
        //printf(result)?;
    }
    
    if(sensors_init(&devices[2]) != SENSORS_OK)
    {
        //printf(result)?;
    }

    deadline = true;
}

int Deadline(fsm_t *this){
    
    return deadline;

}

void Lectura_Sensores(fsm_t *this){

    deadline = false;

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

    // Create a queue capable of containing 1 bool value.
    datoValidoQueue = xQueueCreate( 1 , sizeof( bool ) );


    if( datoValidoQueue != 0 )
    {
        bool DatoValido = true;
        // Send a bool (datoValido) to fsm_deteccion_incendio. 
        if( xQueueGenericSend( datoValidoQueue, ( void * ) &DatoValido, ( TickType_t ) 0, queueSEND_TO_BACK) != pdPASS  )
        {
            // Failed to post the message.
            return 0;
        }
        else{
            return 1;
        }
    }
    else
    {
        //printf("Error: queue of dato valido is not correctly open\n");
        return 0;
    }
	
}

void Send_Data(fsm_t *this){

    // Create a queue capable of containing 10 pointers to data structures.
    // These should be passed by pointer as they contain a lot of data.
    datosSensoresQueue = xQueueCreate( 10, sizeof( struct bme68x_data *) );


    struct bme68x_data *txDataSensor;
    int i = 0;
    //valores_sensores = false;
    
    for(i=0; i<3; i++)   //3 is the size of the devices array
    {
        txDataSensor = & (devices[i].data);
        // Send a pointer to each data struct of each sensor to fsm_deteccion_incendio. 
        xQueueSend( datosSensoresQueue, ( void * ) &txDataSensor, ( TickType_t ) 0 );     
    }

    datos_validos = true;
}


void Apagar_Sensores(fsm_t *this){
    
    datos_validos = false;

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