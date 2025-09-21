/*
 * sensors.h
 *
 * Created on: 08/09/2025
 * Author: Equipo de Proyecto
 *
 * Interfaz para el módulo de gestión de sensores.
 */
#ifndef SENSORS_H_
#define SENSORS_H_

#include "sapi.h"
#include "config.h"

// Enum para identificar qué sensor se disparó
typedef enum {
    SENSOR_NONE,
    SENSOR_PIR1,
    SENSOR_PIR2,
    SENSOR_DOOR
} SensorTriggered_t;

// Inicializa los pines GPIO para los sensores
void sensors_init(void);

// Verifica si algún sensor habilitado ha sido disparado.
// Devuelve el tipo de sensor que se activó o SENSOR_NONE si no hay activación.
SensorTriggered_t sensors_is_any_triggered(void);

#ifdef ENABLE_ZONAL_CONTROL
// Habilita o deshabilita un sensor específico por su nombre.
void sensors_set_enabled(const char* sensor_name, bool_t state);
#endif

#endif /* SENSORS_H_ */
