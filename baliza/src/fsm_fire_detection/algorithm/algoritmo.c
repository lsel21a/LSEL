#include <stddef.h>

#include "algoritmo.h"
#include "config.h"

//Teniendo en cuenta el mismo número de sensores para Temperatura, Humedad y Gases
int algoritmo_incendio(float* Temperatura, float* Humedad, float* Gases, size_t n_items) {
  int posible_incendio1 = 0;
  int posible_incendio2 = 0;
  int posible_incendio3 = 0;

  for (int i = 0; i < n_items; i++) {
    if (Temperatura[i] > CONFIG_ALGO_TMAX && Humedad[i] < CONFIG_ALGO_HMIN) {
      posible_incendio1++;
    } else if (Temperatura[i] > CONFIG_ALGO_TMAX && Gases[i] > CONFIG_ALGO_GMAX) {
      posible_incendio2++;
    } else if (Humedad[i] < CONFIG_ALGO_HMIN && Gases[i] > CONFIG_ALGO_GMAX) {
      posible_incendio3++;
    } else {
      break;
    }
  }
  
  if (posible_incendio1 >= n_items || posible_incendio2 >= n_items || posible_incendio3 >= n_items) {
    return 1;
  } else { 
    return 0;
  }
}
