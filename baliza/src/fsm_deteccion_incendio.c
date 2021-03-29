#include "fsm_deteccion_incendio.h"


//Pensar en si introducir una valor de error 
//static int incendio = 0;
static int muestreo_rapido = 0;
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
  muestreo_rapido = 0;
  incendio = 0;

  struct bme68x_data * rxDataSensor;

   if( datosSensoresQueue != 0 )
   {

      for(int i=0; i<3; i++)   //3 is the size of the devices array
      {
        // Receive a message on the created queue.  Block forevers if a
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
  muestreo_rapido = 0;
  incendio = 0;

}



static void SetMuestreoRapido (fsm_t* this)

{
  incendio = 0;
  muestreo_rapido = 1;
}



static void SendDatoIncendio (fsm_t* this)

{
  incendio = 1;
  muestreo_rapido = 0;
}

int incendio;

// Explicit FSM description

static fsm_trans_t DeteccionIncendio[] = {
    {IDLE, ReceiveDatoValido, DATA_SENSOR, GetDataFromFsmSensor},
    {DATA_SENSOR, NoHayPeligro, IDLE, BackToIdle },
    {DATA_SENSOR, PuedeSerIncendio, POSIBLE_INCENDIO, SetMuestreoRapido },
    {POSIBLE_INCENDIO, ReceiveDatoValido, DATA_SENSOR_POSIBLE_INCENDIO, GetDataFromFsmSensor},
    {DATA_SENSOR_POSIBLE_INCENDIO, NoHayPeligro, IDLE, BackToIdle},
    {DATA_SENSOR_POSIBLE_INCENDIO, HayIncendio, INCENDIO, SendDatoIncendio},
    {INCENDIO, Incendio_to_Idle, IDLE, BackToIdle},
    {-1, NULL, -1, NULL},

};



void  fsm_valores_init ( fsm_valores_t * f, int *temperatura, int *humedad, int *gases)
{
  fsm_init((fsm_t*)f,DeteccionIncendio);
  f -> temperatura = temperatura;
  f -> humedad = humedad;
  f -> gases = gases;
}
