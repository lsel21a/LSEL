#include "fsm_deteccion_incendio.h"
#include "drivers_fsm_deteccion_incendio.h"

// Explicit FSM description
static fsm_trans_t DeteccionIncendio[] = {
    {IDLE_INCENDIO, ReceiveDatoValido, DATA_SENSOR, GetDataFromFsmSensor},
    {DATA_SENSOR, NoHayPeligro, IDLE_INCENDIO, BackToIdle},
    {DATA_SENSOR, PuedeSerIncendio, POSIBLE_INCENDIO, GetMuestreoRapido},
    {POSIBLE_INCENDIO, ReceiveDatoValido, DATA_SENSOR_POSIBLE_INCENDIO, GetDataFromFsmSensor},
    {DATA_SENSOR_POSIBLE_INCENDIO, NoHayPeligro, IDLE_INCENDIO, BackToIdle},
    {DATA_SENSOR_POSIBLE_INCENDIO, HayIncendio, INCENDIO, SendDatoIncendio},
    {INCENDIO, ReceiveDatoValidoIncendio, INCENDIO, GetDataFromFsmSensor},
    {INCENDIO, Incendio_to_Idle, IDLE_INCENDIO, BackToIdle},
    {-1, NULL, -1, NULL}
};


void  fsm_deteccion_incendio_init ( fsm_deteccion_incendio_t *this, QueueHandle_t *datoValidoQueue, QueueHandle_t *datosSensoresQueue, QueueHandle_t *incendioQueue, QueueHandle_t *muestreoRapidoQueue)
{
  fsm_init((fsm_t*)this , DeteccionIncendio);
  this->datoValidoQueue = datoValidoQueue;
  this->datosSensoresQueue = datosSensoresQueue;
  this->incendioQueue = incendioQueue;
  this->muestreoRapidoQueue = muestreoRapidoQueue;
}
