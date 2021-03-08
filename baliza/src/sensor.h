
#ifndef SENSOR_H
#define SENSOR_H
#include <stdio.h>
#include <stdint.h>
#include "bme68x_defs.h"
#include "bme68x.h"

float * get_temp_data(struct bme68x_dev *);
float * get_gas_data(struct bme68x_dev *);
float * get_humidity_data(struct bme68x_dev *);

#endif // SENSOR_H
