#include "fsm_deteccion_incendio.h"


float temperatura[3], humedad[3], gases[3];

static int ReceiveDatoValido (fsm_t* this) 
{
   bool * rxDatoValido;

   if( datoValidoQueue != 0 )
   {
       // Receive a message on the created queue.  Block for 10 ticks if a
       // message is not immediately available.
       if( xQueueReceive( datoValidoQueue, &( rxDatoValido ), portMAX_DELAY))       //( TickType_t ) 10 ) )
       {
           // rxDatoValido now points to the bool variable posted by LecturaFinalizadaOK from fsm_sensores (drivers.c file).
           if( * rxDatoValido == true )
           {
             return 1;
           }
           else
           {
             return 0;
           }
       }
   }
  
   return 0;

}

static int ReceiveDatoValidoIncendio (fsm_t* this)
{
   bool * rxDatoValido;

   if( datoValidoQueue != 0 )
   {
       // Receive a message on the created queue.  Block for 10 ticks if a
       // message is not immediately available.
       if( xQueueReceive( datoValidoQueue, &( rxDatoValido ),( TickType_t ) 10000 / portTICK_PERIOD_MS))       //( TickType_t ) 10 ) )
       {
           // rxDatoValido now points to the bool variable posted by LecturaFinalizadaOK from fsm_sensores (drivers.c file).
           if( * rxDatoValido == true )
           {
             return 1;
           }
           else
           {
             return 0;
           }
       }
   }
  
   return 0;
}

static int NoHayPeligro (fsm_t * this) 
{
  return !(algoritmo_incendio(temperatura, humedad, gases));
}

static int PuedeSerIncendio (fsm_t* this) 
{
  return algoritmo_incendio(temperatura, humedad, gases);
}


static int HayIncendio (fsm_t* this) 
{
  return algoritmo_incendio(temperatura, humedad, gases);
}



static int Incendio_to_Idle (fsm_t* this) 
{
  return !(algoritmo_incendio(temperatura, humedad, gases));
}


static void GetDataFromFsmSensor (fsm_t* this)
{

  struct bme68x_data * rxDataSensor;

   if( datosSensoresQueue != 0 )
   {

      for(int i=0; i<3; i++)   //3 is the size of the devices array
      {
        // Receive a message on the created queue.  Blocks if a
        // message is not immediately available.
        if( xQueueReceive( datosSensoresQueue, &( rxDataSensor ), portMAX_DELAY))       
        {
            // rxDataSensor now points to the struct variable posted by Send_Data from fsm_sensores (drivers.c file).
            temperatura[i] = (rxDataSensor->temperature)/100;
            humedad[i] = (rxDataSensor->humidity)/1000;
            gases[i] = rxDataSensor->gas_resistance;
        }
      }
   }  

}


static void GetDataIncendio (fsm_t* this)
{
 // muestreo_rapido = 0;
 // incendio = 0;

  struct bme68x_data * rxDataSensor;

   if( datosSensoresQueue != 0 )
   {

      for(int i=0; i<3; i++)   //3 is the size of the devices array
      {
        // Receive a message on the created queue.  Blocks if a
        // message is not immediately available.
        if( xQueueReceive( datosSensoresQueue, &( rxDataSensor ), portMAX_DELAY))       
        {
            // rxDataSensor now points to the struct variable posted by Send_Data from fsm_sensores (drivers.c file).
            temperatura[i] = (rxDataSensor->temperature)/100;
            humedad[i] = (rxDataSensor->humidity)/1000;
            gases[i] = rxDataSensor->gas_resistance;
        }
      }
   }  

}

static void BackToIdle (fsm_t* this)
{
    // Create a queue capable of containing 1 bool value.
    incendioQueue = xQueueCreate( 1 , sizeof( bool ) );


    if( incendioQueue != 0 )
    {
        bool Incendio = false;
        // Send a bool (datoValido) to fsm_deteccion_incendio. 
        if( xQueueGenericSend( incendioQueue, ( void * ) &Incendio, ( TickType_t ) 0, queueSEND_TO_BACK) != pdPASS  )
        {
            // Failed to post the message.
            return;
        }
    }
}

static void SetMuestreoRapido (fsm_t* this)
{
  
  // Create a queue capable of containing 1 bool value.
  muestreoRapidoQueue = xQueueCreate( 1 , sizeof( bool ) );


  if( muestreoRapidoQueue != 0 )
  {
      bool muestreoRapido = true;
      // Send a bool (muestreoRapido) to fsm_sensor. 
      if( xQueueGenericSend( muestreoRapidoQueue, ( void * ) &muestreoRapido, ( TickType_t ) 0, queueSEND_TO_BACK) != pdPASS  )
      {
          // Failed to post the message.
          return;
      }
      else{
          return;
      }
  }
  else
  {
      //printf("Error: queue of dato valido is not correctly open\n");
  }
}



static void SendDatoIncendio (fsm_t* this)

{
    // Create a queue capable of containing 1 bool value.
    incendioQueue = xQueueCreate( 1 , sizeof( bool ) );


    if( incendioQueue != 0 )
    {
        bool Incendio = true;
        // Send a bool (datoValido) to fsm_deteccion_incendio. 
        if( xQueueGenericSend( incendioQueue, ( void * ) &Incendio, ( TickType_t ) 0, queueSEND_TO_BACK) != pdPASS  )
        {
            // Failed to post the message.
            return;
        }
    }
}


// Explicit FSM description

static fsm_trans_t DeteccionIncendio[] = {
    {IDLE_INCENDIO, ReceiveDatoValido, DATA_SENSOR, GetDataFromFsmSensor},
    {DATA_SENSOR, NoHayPeligro, IDLE_INCENDIO, BackToIdle },
    {DATA_SENSOR, PuedeSerIncendio, POSIBLE_INCENDIO, SetMuestreoRapido },
    {POSIBLE_INCENDIO, ReceiveDatoValido, DATA_SENSOR_POSIBLE_INCENDIO, GetDataFromFsmSensor},
    {DATA_SENSOR_POSIBLE_INCENDIO, NoHayPeligro, IDLE_INCENDIO, BackToIdle},
    {DATA_SENSOR_POSIBLE_INCENDIO, HayIncendio, INCENDIO, SendDatoIncendio},
    {INCENDIO, ReceiveDatoValidoIncendio, INCENDIO, GetDataIncendio},
    {INCENDIO, Incendio_to_Idle, IDLE_INCENDIO, BackToIdle},
    {-1, NULL, -1, NULL},

};



void  fsm_deteccion_incendio_init ( fsm_deteccion_incendio_t * f, int *temperatura, int *humedad, int *gases)
{
  fsm_init((fsm_t*)f,DeteccionIncendio);
  f -> temperatura = temperatura;
  f -> humedad = humedad;
  f -> gases = gases;
}
