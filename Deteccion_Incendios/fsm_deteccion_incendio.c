#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include "fsm.h"
#include "algoritmo.h"

enum DeteccionIncendio_State {

  IDLE,
  POSIBLE_INCENDIO,
  INCENDIO,

};

//Pensar en si introducir una valor de error 
static int incendio = 0;
static int muestreo_rapido = 0;


static int Idle_to_PosibleIncendio (fsm_t* this) {return algoritmo_incendio(temperatura, humedad, gases)};
static int PosibleIncendio_to_Idle (fsm_t* this) {return ! algoritmo_incendio(temperatura, humedad, gases)};
static int PosibleIncendio_to_Incendio (fsm_t* this) {return algoritmo_incendio(temperatura, humedad, gases)};
static int Incendio_to_Idle (fsm_t* this) {return ! algoritmo_incendio(temperatura, humedad, gases)};


static int timer = 0;
static void timer_isr (union sigval arg) { timer = 1; }
static void timer_start (int ms)
{
  timer_t timerid;
  struct itimerspec spec;
  struct sigevent se;
  se.sigev_notify = SIGEV_THREAD;
  se.sigev_value.sival_ptr = &timerid;
  se.sigev_notify_function = timer_isr;
  se.sigev_notify_attributes = NULL;
  spec.it_value.tv_sec = ms / 1000;
  spec.it_value.tv_nsec = (ms % 1000) * 1000000;
  spec.it_interval.tv_sec = 0;
  spec.it_interval.tv_nsec = 0;
  timer_create (CLOCK_REALTIME, &se, &timerid);
  timer_settime (timerid, 0, &spec, NULL);
}
static int timer_finished (fsm_t* this) { return timer; }
  
static void salida1 (fsm_t* this)

{
  muestreo_rapido = 1;
  incendio = 0;

}

static void salida2 (fsm_t* this)

{
  muestreo_rapido = 0;
  incendio = 0;

}



static void salida3 (fsm_t* this)

{
  incendio = 1;
  muestreo_rapido = 0;
}



static void salida4 (fsm_t* this)

{
  incendio = 0;
  muestreo_rapido = 0;
}


// Explicit FSM description

static fsm_trans_t DeteccionIncendio[] = {

  { IDLE, Idle_to_PosibleIncendio, POSIBLE_INCENDIO, salida1 },
  { POSIBLE_INCENDIO, PosibleIncendio_to_Idle, IDLE, salida2 },
  { POSIBLE_INCENDIO, PosibleIncendio_to_Incendio, INCENDIO, salida3 },
  { INCENDIO, Incendio_to_Idle, IDLE, salida4 },
  {-1, NULL, -1, NULL },

};

// res = a - b

void

timeval_sub (struct timeval *res, struct timeval *a, struct timeval *b)

{

  res->tv_sec = a->tv_sec - b->tv_sec;
  res->tv_usec = a->tv_usec - b->tv_usec;
  if (res->tv_usec < 0) {
    --res->tv_sec;
    res->tv_usec += 1000000;
  }
}


// res = a + b

void

timeval_add (struct timeval *res, struct timeval *a, struct timeval *b)

{

  res->tv_sec = a->tv_sec + b->tv_sec
    + a->tv_usec / 1000000 + b->tv_usec / 1000000; 
  res->tv_usec = a->tv_usec % 1000000 + b->tv_usec % 1000000;

}



// wait until next_activation (absolute time)

void delay_until (struct timeval* next_activation)

{

  struct timeval now, timeout;
  gettimeofday (&now, NULL);
  timeval_sub (&timeout, next_activation, &now);
  select (0, NULL, NULL, NULL, &timeout);

}

void  fsm_valores_init ( fsm_valores_t * f, int *temperatura, int *humedad, int *gases);
{
  fsm_init((fsm_t*)f,DeteccionIncendio);
  f -> temperatura = temperatura;
  f -> humedad = humedad;
  f -> gases = gases;
}
