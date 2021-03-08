/**
 * Copyright (C) 2020 Bosch Sensortec GmbH. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "bme68x.h"
#include "common.h"

/******************************************************************************/
/*!                 Macro definitions                                         */
/*! BME68X shuttle board ID */
#define BME68X_SHUTTLE_ID  0x93

/******************************************************************************/
/*!                Static variable definition                                 */
static uint8_t dev_addr;


void bme68x_check_rslt(const char api_name[], int8_t rslt)
{
    switch (rslt)
    {
        case BME68X_OK:

            /* Do nothing */
            break;
        case BME68X_E_NULL_PTR:
            printf("Error [%d] : Null pointer\r\n", rslt);
            break;
        case BME68X_E_COM_FAIL:
            printf("Error [%d] : Communication failure\r\n", rslt);
            break;
        case BME68X_E_INVALID_LENGTH:
            printf("Error [%d] : Incorrect length parameter\r\n", rslt);
            break;
        case BME68X_E_DEV_NOT_FOUND:
            printf("Error [%d] : Device not found\r\n", rslt);
            break;
        default:
            printf("Error [%d] : Unknown error code\r\n", rslt);
            break;
    }
}

