#ifndef SENSORS_H
#define SENSORS_H

#include "sensor_defs.h"
#include "bme68x.h"

/* CPP guard */
#ifdef __cplusplus
extern "C" {
#endif


/**
 * \brief           Init all the sensor(s)
 * \param[in]       in: Sensors config struct
 * \return          \ref SENSORS_OK on success, member of \ref sensors_status_t otherwise
 */
sensors_status_t sensors_init(sensors_config_t* p_config);

/**
 * \brief           Deinit all the sensor(s)
 * \param[in]       in: Sensors config struct
 * \return          \ref SENSORS_OK on success, member of \ref sensors_status_t otherwise
 */
sensors_status_t sensors_deinit(sensors_config_t* p_config);

/**
 * \brief           Function to read the data from the sensor
 * \param[in]       in: struct of the sensor
 * \return         \ref SENSORS_OK on success, member of \ref sensors_status_t otherwise 
 */
sensors_status_t get_data(sensors_config_t* p_config);

/**
 * \brief           Function to put the sensor in sleep mode
 * \param[in]       in: struct of the sensor
 * \return         \ref SENSORS_OK on success, member of \ref sensors_status_t otherwise 
 */
sensors_status_t sleep_data(sensors_config_t* p_config);

/* End of CPP guard */
#ifdef __cplusplus
}
#endif

#endif /* SENSORS_H */
