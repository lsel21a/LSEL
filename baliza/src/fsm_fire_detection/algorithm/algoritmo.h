#ifndef ALGORITMO_H
#define ALGORITMO_H

#include <stddef.h>

#define ALGO_TMAX 65
#define ALGO_GMAX 74 //niveles de monoxido y dioxido de carbono
#define ALGO_HMIN 20

int algoritmo_incendio(float* Temperatura, float* Humedad, float* Gases, size_t n_items);

#endif /* ALGORITMO_H */
