#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include "fsm.h"
#include "algoritmo.h"

#define Tmax 60
#define Gmax 40
#define Hmin 20
#define Datos_N 3

//Teniendo en cuenta el mismo número de sensores para Temperatura, Humedad y Gases
int algoritmo_incendio(float *Temperatura, float *Humedad, float *Gases)

{
int posible_incendio1=0;
int posible_incendio2=0;
int posible_incendio3=0;

  for (int i = 0; i < Datos_N; i++)
  {
    if (Temperatura[i] > Tmax && Humedad[i] < Hmin)
    {
      posible_incendio1++;
    } 
    else if (Temperatura[i] > Tmax && Gases[i] > Gmax)
    {
      posible_incendio2++;
    }
    else if (Humedad[i] < Hmin && Gases[i] > Gmax)
    {
      posible_incendio3++;
    }
    else 
    {
      break;
    }
  }
  if (posible_incendio1 >= 2 || posible_incendio2 >= 2 || posible_incendio3 >= 2)
    {
    return 1;
    }
  else
    { 
    return 0;
    }
}
