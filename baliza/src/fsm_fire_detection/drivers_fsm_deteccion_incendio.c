#include "drivers_fsm_deteccion_incendio.h"

int ReceiveDatoValido (fsm_t* this) 
{
  bool * rxDatoValido;
  fsm_deteccion_incendio_t *fp = (fsm_deteccion_incendio_t *) this;

  if( *(fp->datoValidoQueue) != 0 )
  {
       // Receive a message on the created queue.  Block for 10 ticks if a
       // message is not immediately available.
       if( xQueueReceive( *(fp->datoValidoQueue), &( rxDatoValido ), portMAX_DELAY))       //( TickType_t ) 10 ) )
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

int ReceiveDatoValidoIncendio (fsm_t* this)
{
  
   bool * rxDatoValido;
   fsm_deteccion_incendio_t *fp = (fsm_deteccion_incendio_t *) this;

   if( *(fp->datoValidoQueue) != 0 )
   {
       // Receive a message on the created queue.  Block for 10 ticks if a
       // message is not immediately available.
       if( xQueueReceive( *(fp->datoValidoQueue), &( rxDatoValido ),( TickType_t ) 10000 / portTICK_PERIOD_MS))       //( TickType_t ) 10 ) )
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

int NoHayPeligro (fsm_t * this) 
{
  fsm_deteccion_incendio_t *fp = (fsm_deteccion_incendio_t *) this;
  return !(algoritmo_incendio(fp->temperatura, fp->humedad, fp->gases));
}

int PuedeSerIncendio (fsm_t* this) 
{
  fsm_deteccion_incendio_t *fp = (fsm_deteccion_incendio_t *) this;
  return algoritmo_incendio(fp->temperatura, fp->humedad, fp->gases);
}

int HayIncendio (fsm_t* this) 
{
  fsm_deteccion_incendio_t *fp = (fsm_deteccion_incendio_t *) this;
  return algoritmo_incendio(fp->temperatura, fp->humedad, fp->gases);
}

int Incendio_to_Idle (fsm_t* this) 
{
  fsm_deteccion_incendio_t *fp = (fsm_deteccion_incendio_t *) this;
  return !(algoritmo_incendio(fp->temperatura, fp->humedad, fp->gases));
}

void GetDataFromFsmSensor (fsm_t* this)
{
  fsm_deteccion_incendio_t *fp = (fsm_deteccion_incendio_t *) this;
  sensors_data_t rxDataSensor[NUM_SENSORS];

   if( *(fp->datosSensoresQueue) != 0 )
   {
     if( xQueueReceive(*(fp->datosSensoresQueue), rxDataSensor, portMAX_DELAY))       
      {
          for(int i=0; i<NUM_SENSORS; i++)
          {
            fp->temperatura[i] = rxDataSensor[i].temperature;
            fp->humedad[i] = rxDataSensor[i].humidity;
            fp->gases[i] = rxDataSensor[i].gas_resistance;
          }
      }
   }  
}

void BackToIdle (fsm_t* this)
{
  fsm_deteccion_incendio_t *fp = (fsm_deteccion_incendio_t *) this;
  
  if( *(fp->incendioQueue) != 0 )
  {
      bool Incendio = false;
      // Send a bool (datoValido) to fsm_deteccion_incendio. 
      if( xQueueGenericSend( *(fp->incendioQueue) , ( void * ) &Incendio, ( TickType_t ) 0, queueSEND_TO_BACK) != pdPASS  )
      {
          // Failed to post the message.
          return;
      }
  }
}

void GetMuestreoRapido (fsm_t* this)
{
  fsm_deteccion_incendio_t *fp = (fsm_deteccion_incendio_t *) this;

  if( *(fp->muestreoRapidoQueue) != 0 )
  {
      bool muestreoRapido = true;
      // Send a bool (muestreoRapido) to fsm_sensor. 
      if( xQueueGenericSend( *(fp->muestreoRapidoQueue), ( void * ) &muestreoRapido, ( TickType_t ) 0, queueSEND_TO_BACK) != pdPASS  )
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

void SendDatoIncendio (fsm_t* this)
{
  fsm_deteccion_incendio_t *fp = (fsm_deteccion_incendio_t *) this;

  if( *(fp->incendioQueue) != 0 )
  {
      bool Incendio = true;
      // Send a bool (datoValido) to fsm_deteccion_incendio. 
      if( xQueueGenericSend( *(fp->incendioQueue), ( void * ) &Incendio, ( TickType_t ) 0, queueSEND_TO_BACK) != pdPASS  )
      {
          // Failed to post the message.
          return;
      }
  }
}