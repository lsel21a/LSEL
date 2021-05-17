#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "drivers_fsm_deteccion_incendio.h"
#include "fsm_deteccion_incendio.h"
#include "config.h"

int ReceiveDatoValido (fsm_t* this) 
{
  bool rxDatoValido = false;
  fsm_deteccion_incendio_t *fp = (fsm_deteccion_incendio_t *) this;

  if( *(fp->datoValidoQueue) != 0 ) {
    // Receive a message on the created queue.  Block for 10 ticks if a
    // message is not immediately available.
    xQueueReceive( *(fp->datoValidoQueue), (void *) &rxDatoValido, ( TickType_t ) 0);
#ifdef DEBUG_PRINT_ENABLE
    printf("Se ha recibido %d en la cola de datoValido \n", rxDatoValido);
#endif /* DEBUG_PRINT_ENABLE */
  } else {
#ifdef DEBUG_PRINT_ENABLE
    printf("Error en abrir cola de datoValido \n");
#endif /* DEBUG_PRINT_ENABLE */
  }
  return rxDatoValido;
}

int ReceiveDatoValidoIncendio (fsm_t* this)
{
  bool rxDatoValido = false;
  fsm_deteccion_incendio_t *fp = (fsm_deteccion_incendio_t *) this;

  if( *(fp->datoValidoQueue) != 0 ) {
    // Receive a message on the created queue.  Block for 10 ticks if a
    // message is not immediately available.
    xQueueReceive( *(fp->datoValidoQueue), (void *) &rxDatoValido,( TickType_t ) 0);
#ifdef DEBUG_PRINT_ENABLE
    printf("Se ha enviado %d en la cola de datoValico \n", rxDatoValido);
#endif /* DEBUG_PRINT_ENABLE */
  } else {
#ifdef DEBUG_PRINT_ENABLE
    printf("Error en abrir cola de datoValido (incendio) \n");
#endif /* DEBUG_PRINT_ENABLE */
  }
  return rxDatoValido;
}

int NoHayPeligro (fsm_t * this) 
{
  int resultado = 0;

  fsm_deteccion_incendio_t *fp = (fsm_deteccion_incendio_t *) this;
  resultado = !(algoritmo_incendio(fp->temperatura, fp->humedad, fp->gases, CONFIG_SENSOR_NUM)); 
  
#ifdef DEBUG_PRINT_ENABLE
  printf("Algoritmo ha devuelto %d \n", !resultado);
#endif /* DEBUG_PRINT_ENABLE */
  
  return resultado;
}

int PuedeSerIncendio (fsm_t* this) 
{
  int resultado = 0;

  fsm_deteccion_incendio_t *fp = (fsm_deteccion_incendio_t *) this;
  resultado = algoritmo_incendio(fp->temperatura, fp->humedad, fp->gases, CONFIG_SENSOR_NUM); 

#ifdef DEBUG_PRINT_ENABLE
  printf("Algoritmo ha devuelto %d \n", resultado);
#endif /* DEBUG_PRINT_ENABLE */
  
  return resultado;
}

int HayIncendio (fsm_t* this) 
{
  int resultado = 0;

  fsm_deteccion_incendio_t *fp = (fsm_deteccion_incendio_t *) this;
  resultado = algoritmo_incendio(fp->temperatura, fp->humedad, fp->gases, CONFIG_SENSOR_NUM); 

#ifdef DEBUG_PRINT_ENABLE
  printf("Algoritmo ha devuelto %d \n", resultado);
#endif /* DEBUG_PRINT_ENABLE */
  
  return resultado;
}

int Incendio_to_Idle (fsm_t* this) 
{
  int resultado = 0;

  fsm_deteccion_incendio_t *fp = (fsm_deteccion_incendio_t *) this;
  resultado = !(algoritmo_incendio( fp->temperatura, fp->humedad, fp->gases, CONFIG_SENSOR_NUM)); 
  
#ifdef DEBUG_PRINT_ENABLE
  printf("Algoritmo ha devuelto %d \n", !resultado);
#endif /* DEBUG_PRINT_ENABLE */
  
  return resultado;
}

void GetDataFromFsmSensor (fsm_t* this)
{
  fsm_deteccion_incendio_t *fp = (fsm_deteccion_incendio_t *) this;
  sensors_data_t rxDataSensor[CONFIG_SENSOR_NUM];

   if( *(fp->datosSensoresQueue) != 0 )
   {
      if( xQueueReceive(*(fp->datosSensoresQueue), (void *) rxDataSensor, ( TickType_t ) 0))       
      {
        // Se envían los datos a la FSM Emergencia
        xQueueReset(*(fp->datosMQTTQueue));
        xQueueSend(*(fp->datosMQTTQueue), (void *) rxDataSensor, ( TickType_t ) 0);

        // Se almacenan los datos
        for(int i=0; i<CONFIG_SENSOR_NUM; i++)
        {
          fp->temperatura[i] = rxDataSensor[i].temperature;
          fp->humedad[i] = rxDataSensor[i].humidity;
          fp->gases[i] = rxDataSensor[i].gas_resistance;
#ifdef DEBUG_PRINT_ENABLE
          printf("Datos sensor %d recibido \n", i);
          printf("Se ha recibido %f ºC.\n", fp->temperatura[i]);
          printf("Se ha recibido %f %%H20.\n", fp->humedad[i]);
          printf("Se ha recibido %f Ohms de gas.\n", fp->gases[i]);
#endif /* DEBUG_PRINT_ENABLE */
        }
      }
      else
      {
#ifdef DEBUG_PRINT_ENABLE
        printf("Error en recibir desde cola de datos sensores \n");
#endif /* DEBUG_PRINT_ENABLE */
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
#ifdef DEBUG_PRINT_ENABLE
        printf("Error en enviar senal de incendio \n");
#endif /* DEBUG_PRINT_ENABLE */
      }
      else
      {
#ifdef DEBUG_PRINT_ENABLE
        printf("Enviado senal de incendio = false \n");
#endif /* DEBUG_PRINT_ENABLE */
      }
  }
  else
  {
#ifdef DEBUG_PRINT_ENABLE
      printf("Error: queue of incendio is not correctly open\n");
#endif /* DEBUG_PRINT_ENABLE */
  }

  if( *(fp->muestreoRapidoQueue) != 0 )
  {
    bool muestreoRapido = false;
    // Send a bool (muestreoRapido) to fsm_sensor.
    if( xQueueGenericSend( *(fp->muestreoRapidoQueue), (void *) &muestreoRapido, ( TickType_t ) 0, queueSEND_TO_BACK) != pdPASS  )
    {
      // Failed to post the message.
#ifdef DEBUG_PRINT_ENABLE
      printf("Error en enviar senal de muestreoRapido \n");
#endif /* DEBUG_PRINT_ENABLE */
    }
    else
    {
#ifdef DEBUG_PRINT_ENABLE
      printf("Enviado senal de muestreoRapido = false \n");
#endif /* DEBUG_PRINT_ENABLE */
    }  
  }
  else
  {
#ifdef DEBUG_PRINT_ENABLE
  printf("Error: queue of muestreoRapido is not correctly open\n");
#endif /* DEBUG_PRINT_ENABLE */
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
#ifdef DEBUG_PRINT_ENABLE
      printf("Error en enviar senal de muestreoRapido.\n");
#endif /* DEBUG_PRINT_ENABLE */
    }
    else
    {
#ifdef DEBUG_PRINT_ENABLE
      printf("Enviado senal de muestreoRapido = true.\n");
#endif /* DEBUG_PRINT_ENABLE */
    }  
  }
  else
  {
#ifdef DEBUG_PRINT_ENABLE
    printf("Error: queue of muestreoRapido is not correctly open.\n");
#endif /* DEBUG_PRINT_ENABLE */
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
#ifdef DEBUG_PRINT_ENABLE
      printf("Error en enviar senal de incendio.\n");
#endif /* DEBUG_PRINT_ENABLE */
    }
    else
    {
#ifdef DEBUG_PRINT_ENABLE
      printf("Enviado senal de incendio = true.\n");
#endif /* DEBUG_PRINT_ENABLE */
    }
  }
  else
  {
#ifdef DEBUG_PRINT_ENABLE
    printf("Error: queue of incendio is not correctly open.\n");
#endif /* DEBUG_PRINT_ENABLE */
  }

  if( *(fp->muestreoRapidoQueue) != 0 )
  {
    bool muestreoRapido = false;
    // Send a bool (muestreoRapido) to fsm_sensor.
    if( xQueueGenericSend( *(fp->muestreoRapidoQueue), (void *) &muestreoRapido, ( TickType_t ) 0, queueSEND_TO_BACK) != pdPASS  )
    {
      // Failed to post the message.
#ifdef DEBUG_PRINT_ENABLE
      printf("Error en enviar senal de muestreoRapido.\n");
#endif /* DEBUG_PRINT_ENABLE */
    }
    else
    {
#ifdef DEBUG_PRINT_ENABLE
      printf("Enviado senal de muestreoRapido = false.\n");
#endif /* DEBUG_PRINT_ENABLE */
    }
  }
  else
  {
#ifdef DEBUG_PRINT_ENABLE
    printf("Error: queue of muestreoRapido is not correctly open.\n");
#endif /* DEBUG_PRINT_ENABLE */
  }
  return;
}