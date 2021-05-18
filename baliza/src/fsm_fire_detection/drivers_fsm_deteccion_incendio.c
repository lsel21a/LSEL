#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "drivers_fsm_deteccion_incendio.h"
#include "fsm_deteccion_incendio.h"
#include "config.h"

static const char* TAG = "drivers_fsm_deteccion_incendio";


int ReceiveDatoValido (fsm_t* this) 
{
  bool rxDatoValido = false;
  fsm_deteccion_incendio_t *fp = (fsm_deteccion_incendio_t *) this;

  if( *(fp->datoValidoQueue) != 0 ) {
    // Receive a message on the created queue.  Block for 10 ticks if a
    // message is not immediately available.
    if (xQueueReceive( *(fp->datoValidoQueue), (void *) &rxDatoValido, ( TickType_t ) 0) == pdTRUE) {
      ESP_LOGD(TAG, "[ReceiveDatoValido] Se ha recibido %d en la cola de datoValido.", rxDatoValido);
    } else {
      ESP_LOGD(TAG, "[ReceiveDatoValido] Cola de datoValido vacia.");
    }
  } else {
    ESP_LOGE(TAG, "[ReceiveDatoValido] Error al abrir cola de datoValido.");
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
    if (xQueueReceive( *(fp->datoValidoQueue), (void *) &rxDatoValido,( TickType_t ) 0) == pdTRUE) {
      ESP_LOGD(TAG, "[ReceiveDatoValidoIncendio] Se ha recibido %d en la cola de datoValido.", rxDatoValido);
    } else {
      ESP_LOGD(TAG, "[ReceiveDatoValidoIncendio] Cola de datoValido vacia.");
    }
  } else {
    ESP_LOGE(TAG, "[ReceiveDatoValidoIncendio] Error al abrir cola de datoValido.");
  }

  return rxDatoValido;
}

int NoHayPeligro (fsm_t * this) 
{
  int resultado = 0;

  fsm_deteccion_incendio_t *fp = (fsm_deteccion_incendio_t *) this;
  resultado = !(algoritmo_incendio(fp->temperatura, fp->humedad, fp->gases, CONFIG_SENSOR_NUM)); 
  
  ESP_LOGD(TAG, "[NoHayPeligro] Algoritmo ha devuelto %d.", !resultado);
  
  return resultado;
}

int PuedeSerIncendio (fsm_t* this) 
{
  int resultado = 0;

  fsm_deteccion_incendio_t *fp = (fsm_deteccion_incendio_t *) this;
  resultado = algoritmo_incendio(fp->temperatura, fp->humedad, fp->gases, CONFIG_SENSOR_NUM); 

  ESP_LOGD(TAG, "[Incendio_to_Idle] Algoritmo ha devuelto %d.", resultado);
  
  return resultado;
}

int HayIncendio (fsm_t* this) 
{
  int resultado = 0;

  fsm_deteccion_incendio_t *fp = (fsm_deteccion_incendio_t *) this;
  resultado = algoritmo_incendio(fp->temperatura, fp->humedad, fp->gases, CONFIG_SENSOR_NUM); 

  ESP_LOGD(TAG, "[Incendio_to_Idle] Algoritmo ha devuelto %d.", resultado);
  
  return resultado;
}

int Incendio_to_Idle (fsm_t* this) 
{
  int resultado = 0;

  fsm_deteccion_incendio_t *fp = (fsm_deteccion_incendio_t *) this;
  resultado = !(algoritmo_incendio( fp->temperatura, fp->humedad, fp->gases, CONFIG_SENSOR_NUM)); 
  
  ESP_LOGD(TAG, "[Incendio_to_Idle] Algoritmo ha devuelto %d.", !resultado);
  
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

          ESP_LOGI(TAG, "[GetDataFromFsmSensor] Datos del sensor %d recibidos.", i);
          ESP_LOGI(TAG, "[GetDataFromFsmSensor] \tSe ha recibido %f ºC.", fp->temperatura[i]);
          ESP_LOGI(TAG, "[GetDataFromFsmSensor] \tSe ha recibido %f %%H20.", fp->humedad[i]);
          ESP_LOGI(TAG, "[GetDataFromFsmSensor] \tSe ha recibido %f Ohms de gas.", fp->gases[i]);
        }
      }
      else
      {
        ESP_LOGW(TAG, "[GetDataFromFsmSensor] Error al recibir datos desde cola datosSensores.");
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
      if( xQueueGenericSend( *(fp->incendioQueue) , ( void * ) &Incendio, ( TickType_t ) 0, queueSEND_TO_BACK) != pdTRUE  )
      {
        // Failed to post the message.
        ESP_LOGW(TAG, "[BackToIdle] Error al enviar senal de incendio.");
      }
      else
      {
        ESP_LOGD(TAG, "[BackToIdle] Enviada senal de incendio = false.");
      }
  }
  else
  {
    ESP_LOGE(TAG, "[BackToIdle] Error al abrir cola de incendio.");
  }

  if( *(fp->muestreoRapidoQueue) != 0 )
  {
    bool muestreoRapido = false;
    // Send a bool (muestreoRapido) to fsm_sensor.
    if( xQueueGenericSend( *(fp->muestreoRapidoQueue), (void *) &muestreoRapido, ( TickType_t ) 0, queueSEND_TO_BACK) != pdTRUE  )
    {
      // Failed to post the message.
      ESP_LOGW(TAG, "[BackToIdle] Error al enviar senal de muestreoRapido.");
    }
    else
    {
      ESP_LOGD(TAG, "[BackToIdle] Enviada senal de muestreoRapido = false.");
    }  
  }
  else
  {
    ESP_LOGE(TAG, "[BackToIdle] Error al abrir cola de muestreoRapido.");
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
    if( xQueueGenericSend( *(fp->muestreoRapidoQueue), (void *) &muestreoRapido, ( TickType_t ) 0, queueSEND_TO_BACK) != pdTRUE  )
    {
      // Failed to post the message.
      ESP_LOGW(TAG, "[GetMuestreoRapido] Error al enviar senal de muestreoRapido.");
    }
    else
    {
      ESP_LOGD(TAG, "[GetMuestreoRapido] Enviada senal de muestreoRapido = true.");
    }  
  }
  else
  {
    ESP_LOGE(TAG, "[GetMuestreoRapido] Error al abrir cola de muestreoRapido.");
  }
}

void SendDatoIncendio (fsm_t* this)
{
  fsm_deteccion_incendio_t *fp = (fsm_deteccion_incendio_t *) this;

  if( *(fp->incendioQueue) != 0 )
  {
    bool Incendio = true;
    // Send a bool (datoValido) to fsm_deteccion_incendio. 
    if( xQueueGenericSend( *(fp->incendioQueue), (void *) &Incendio, ( TickType_t ) 0, queueSEND_TO_BACK) != pdTRUE  )
    {
      // Failed to post the message.
      ESP_LOGW(TAG, "[SendDatoIncendio] Error al enviar senal de incendio.");
    }
    else
    {
      ESP_LOGD(TAG, "[SendDatoIncendio] Enviada senal de incendio = true.");
    }
  }
  else
  {
    ESP_LOGE(TAG, "[SendDatoIncendio] Error al abrir cola de incendio.");
  }

  if( *(fp->muestreoRapidoQueue) != 0 )
  {
    bool muestreoRapido = false;
    // Send a bool (muestreoRapido) to fsm_sensor.
    if( xQueueGenericSend( *(fp->muestreoRapidoQueue), (void *) &muestreoRapido, ( TickType_t ) 0, queueSEND_TO_BACK) != pdTRUE  )
    {
      // Failed to post the message.
      ESP_LOGW(TAG, "[SendDatoIncendio] Error al enviar senal de muestreoRapido.");
    }
    else
    {
      ESP_LOGD(TAG, "[SendDatoIncendio] Enviada senal de muestreoRapido = false.");
    }
  }
  else
  {
    ESP_LOGE(TAG, "[SendDatoIncendio] Error al abrir cola de muestreoRapido.");
  }
  return;
}