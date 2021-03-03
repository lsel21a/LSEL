/**
 * \file sensors.h
 * \brief 
 * \version 0.1
 * \date 2021-03-03
 */


#ifndef SENSORS_H
#define SENSORS_H

#include "sensors_defs.h"

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

/* TODO
 * 
 * Deinit
 *   Parameters: sensors_config_t*
 *   Put sensor(s) in low power mode
 *   Clean sensors_config_t allocs
 *   Unset init flag
 */


/* End of CPP guard */
#ifdef __cplusplus
}
#endif

#endif /* SENSORS_H */
