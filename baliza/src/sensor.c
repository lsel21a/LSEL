/**
 * Copyright (C) 2020 Bosch Sensortec GmbH
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdio.h>
#include "sensor.h"

static struct bme68x_data data;

float * get_temp_data(struct bme68x_dev *bme)
{
  int8_t rslt;
  uint8_t n_fields;

  rslt = bme68x_get_data(BME68X_FORCED_MODE, &data, &n_fields, bme);

  if (rslt != BME68X_OK)
  {
    return NULL;
  }
  else
  {
    data.temperature = (data.temperature / 100);
    return &data.temperature;
  }
}

float * get_gas_data(struct bme68x_dev *bme)
{
  int8_t rslt;
  uint8_t n_fields;

  rslt = bme68x_get_data(BME68X_FORCED_MODE, &data, &n_fields, bme);

  if (rslt != BME68X_OK)
  {
    return NULL;
  }
  else
  {
    return &data.gas_resistance;
  }
}

float * get_humidity_data(struct bme68x_dev *bme)
{
  int8_t rslt;
  uint8_t n_fields;

  rslt = bme68x_get_data(BME68X_FORCED_MODE, &data, &n_fields, bme);

  if (rslt != BME68X_OK)
  {
    return NULL;
  }
  else
  {
    data.humidity = (data.humidity / 1000);
    return &data.humidity;
  }
}
