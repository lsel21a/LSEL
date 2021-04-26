#include "drivers_fsm_deteccion_incendio.h"

int ReceiveDatoValido (fsm_t* this) 
{
  bool rxDatoValido = false;
  fsm_deteccion_incendio_t *fp = (fsm_deteccion_incendio_t *) this;

  if( *(fp->datoValidoQueue) != 0 )
  {
        // Receive a message on the created queue.  Block for 10 ticks if a
        // message is not immediately available.
        xQueueReceive( *(fp->datoValidoQueue), (void *) &rxDatoValido, ( TickType_t ) 0);
        printf("Se ha recibido %d en la cola de datoValido \n", rxDatoValido);
  }   
  else
   {
       printf("Error en abrir cola de datoValido \n");
   }
  return rxDatoValido;
}

int ReceiveDatoValidoIncendio (fsm_t* this)
{
  
   bool rxDatoValido = false;
   fsm_deteccion_incendio_t *fp = (fsm_deteccion_incendio_t *) this;

   if( *(fp->datoValidoQueue) != 0 )
   {
       // Receive a message on the created queue.  Block for 10 ticks if a
       // message is not immediately available.
       xQueueReceive( *(fp->datoValidoQueue), (void *) &rxDatoValido,( TickType_t ) 0);
       printf("Se ha enviado %d en la cola de datoValico \n", rxDatoValido);
   }   
   else
   {
       printf("Error en abrir cola de datoValido (incendio) \n");
   }
   return rxDatoValido;
}

int NoHayPeligro (fsm_t * this) 
{
  int resultado = 0;

  fsm_deteccion_incendio_t *fp = (fsm_deteccion_incendio_t *) this;
  resultado = !(algoritmo_incendio(fp->temperatura, fp->humedad, fp->gases)); 
  
  printf("Algoritmo ha devuelto %d \n", resultado);
  
  return resultado;
}

int PuedeSerIncendio (fsm_t* this) 
{
  int resultado = 0;

  fsm_deteccion_incendio_t *fp = (fsm_deteccion_incendio_t *) this;
  resultado = algoritmo_incendio(fp->temperatura, fp->humedad, fp->gases); 
  
  printf("Algoritmo ha devuelto %d \n", resultado);
  
  return resultado;
}

int HayIncendio (fsm_t* this) 
{
  int resultado = 0;

  fsm_deteccion_incendio_t *fp = (fsm_deteccion_incendio_t *) this;
  resultado = algoritmo_incendio(fp->temperatura, fp->humedad, fp->gases); 
  
  printf("Algoritmo ha devuelto %d \n", resultado);
  
  return resultado;
}

int Incendio_to_Idle (fsm_t* this) 
{
  int resultado = 0;

  fsm_deteccion_incendio_t *fp = (fsm_deteccion_incendio_t *) this;
  resultado = !(algoritmo_incendio(fp->temperatura, fp->humedad, fp->gases)); 
  
  printf("Algoritmo ha devuelto %d \n", resultado);
  
  return resultado;
}

void GetDataFromFsmSensor (fsm_t* this)
{
  fsm_deteccion_incendio_t *fp = (fsm_deteccion_incendio_t *) this;
  sensors_data_t rxDataSensor[NUM_SENSORS];

   if( *(fp->datosSensoresQueue) != 0 )
   {
      if( xQueueReceive(*(fp->datosSensoresQueue), (void *) rxDataSensor, ( TickType_t ) 0))       
      {
          for(int i=0; i<NUM_SENSORS; i++)
          {
            fp->temperatura[i] = rxDataSensor[i].temperature;
            fp->humedad[i] = rxDataSensor[i].humidity;
            fp->gases[i] = rxDataSensor[i].gas_resistance;
            printf("Datos sensor %d recibido \n", i);
          }
      }
      else
      {
        printf("Error en recibir desde cola de datos sensores \n");
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
          printf("Error en enviar senal de incendio \n");
      }
      else
      {
          printf("Enviado senal de incendio = false \n");
      }
  }
  else
  {
      printf("Error: queue of incendio is not correctly open\n");
  }

  if( *(fp->muestreoRapidoQueue) != 0 )
  {
      bool muestreoRapido = false;
      // Send a bool (muestreoRapido) to fsm_sensor.
      if( xQueueGenericSend( *(fp->muestreoRapidoQueue), (void *) &muestreoRapido, ( TickType_t ) 0, queueSEND_TO_BACK) != pdPASS  )
      {
          // Failed to post the message.
          printf("Error en enviar senal de muestreoRapido \n");
      }
      else
      {
          printf("Enviado senal de muestreoRapido = false \n");
      }  
  }
  else
  {
      printf("Error: queue of muestreoRapido is not correctly open\n");
  }
  return;
}

void GetMuestreoRapido (fsm_t* this)
{
  fsm_deteccion_incendio_t *fp = (fsm_deteccion_incendio_t *) this;

  if( *(fp->muestreoRapidoQueue) != 0 )
  {
      bool muestreoRapido = true;
      // Send a bool (muestreoRapido) to fsm_sensor.
      if( xQueueGenericSend( *(fp->muestreoRapidoQueue), (void *) &muestreoRapido, ( TickType_t ) 0, queueSEND_TO_BACK) != pdPASS  )
      {
          // Failed to post the message.
          printf("Error en enviar senal de muestreoRapido.\n");
      }
      else
      {
          printf("Enviado senal de muestreoRapido = true.\n");
      }  
  }
  else
  {
      printf("Error: queue of muestreoRapido is not correctly open.\n");
  }
}

void SendDatoIncendio (fsm_t* this)
{
  fsm_deteccion_incendio_t *fp = (fsm_deteccion_incendio_t *) this;

  if( *(fp->incendioQueue) != 0 )
  {
      bool Incendio = true;
      // Send a bool (datoValido) to fsm_deteccion_incendio. 
      if( xQueueGenericSend( *(fp->incendioQueue), (void *) &Incendio, ( TickType_t ) 0, queueSEND_TO_BACK) != pdPASS  )
      {
          // Failed to post the message.
          printf("Error en enviar senal de incendio.\n");
      }
      else
      {
          printf("Enviado senal de incendio = true.\n");
      }
  }
  else
  {
      printf("Error: queue of incendio is not correctly open.\n");
  }

  if( *(fp->muestreoRapidoQueue) != 0 )
  {
      bool muestreoRapido = false;
      // Send a bool (muestreoRapido) to fsm_sensor.
      if( xQueueGenericSend( *(fp->muestreoRapidoQueue), (void *) &muestreoRapido, ( TickType_t ) 0, queueSEND_TO_BACK) != pdPASS  )
      {
          // Failed to post the message.
          printf("Error en enviar senal de muestreoRapido.\n");
      }
      else
      {
          printf("Enviado senal de muestreoRapido = false.\n");
      }
  }
  else
  {
      printf("Error: queue of muestreoRapido is not correctly open.\n");
  }
  return;
}