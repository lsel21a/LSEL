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
#else
// Add uC standard types header
// Add uC stddef header (or at least, define NULL)
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
    struct bme68x_dev bme_dev;                  /* BME68x device */

    uint8_t _bme_dev_addr;                      /* [Internal] BME68x I2C address */
} sensors_config_t;

#endif /* SENSORS_DEFS_H */
