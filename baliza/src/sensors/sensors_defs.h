/**
 * \file sensors_def.h
 * \brief 
 * \version 0.1
 * \date 2021-03-03
 */


#ifndef SENSORS_DEFS_H
#define SENSORS_DEFS_H

#ifdef TEST
#include <stdint.h>
#include <stddef.h>
#include <malloc.h>
#else
// Add uC standard types header
// Add uC stddef header (or at least, define NULL)
// Define uC malloc(...) and free(...) 
#endif /* TEST */

#include "bme68x/bme68x.h"

#define SENSORS_BME68X_I2C_ADDR BME68X_I2C_ADDR_LOW     /* BME68x I2C address */
// #define SENSORS_BME68X_I2C_ADDR BME68X_I2C_ADDR_HIGH
#define SENSORS_AMBIENT_TEMP    25                      /* Temperatura ambiente de los sensores */

typedef enum {
    SENSORS_OK,                                 /* Everything Ok 👌 */
    SENSORS_ERR,                                /* Something went wrong */
    SENSORS_IN_USE_ERR,                         /* Already initialized */
    SENSORS_MEM_ERR,                            /* Dynamic memory error */
    SENSORS_HAL_ERR                             /* Sensors HAL error */
} sensors_status_t;

typedef struct {
    struct bme68x_dev* p_bme_dev;               /* BME68x device */
} sensors_config_t;

#endif /* SENSORS_DEFS_H */
