/*
 * sensors.c
 *
 * Created on: 08/09/2025
 * Author: Equipo de Proyecto
 *
 * Implementación del módulo de gestión de sensores.
 */
#include "sensors.h"
#include <string.h>

#ifdef ENABLE_ZONAL_CONTROL
// Array para mantener el estado de habilitación de cada sensor
// Índices: 0 -> PIR1, 1 -> PIR2, 2 -> DOOR
static bool_t sensor_enabled[3] = {TRUE, TRUE, TRUE};
#endif

void sensors_init(void) {
    gpioConfig(PIR1_PIN, GPIO_INPUT);
    gpioConfig(PIR2_PIN, GPIO_INPUT);
    // Usar pull-up interno para el sensor magnético.
    // Un interruptor N.C. (Normalmente Cerrado) leerá LOW cuando la puerta esté cerrada
    // y HIGH cuando se abra. Un N.O. (Normalmente Abierto) leerá a la inversa.
    // El código asume N.C. donde LOW es seguro y HIGH es disparo.
    gpioConfig(MAG_SENSOR_PIN, GPIO_INPUT_PULLUP);
}

SensorTriggered_t sensors_is_any_triggered(void) {
    #ifdef ENABLE_ZONAL_CONTROL
    if (sensor_enabled[0] && gpioRead(PIR1_PIN) == HIGH) {
        return SENSOR_PIR1;
    }
    if (sensor_enabled[1] && gpioRead(PIR2_PIN) == HIGH) {
        return SENSOR_PIR2;
    }
    // Asumiendo sensor magnético N.C. con pull-up, un HIGH significa circuito abierto (puerta abierta)
    if (sensor_enabled[2] && gpioRead(MAG_SENSOR_PIN) == HIGH) {
        return SENSOR_DOOR;
    }
    #else
    // Si el control zonal está deshabilitado, leer todos los sensores.
    if (gpioRead(PIR1_PIN) == HIGH) {
        return SENSOR_PIR1;
    }
    if (gpioRead(PIR2_PIN) == HIGH) {
        return SENSOR_PIR2;
    }
    if (gpioRead(MAG_SENSOR_PIN) == HIGH) {
        return SENSOR_DOOR;
    }
    #endif

    return SENSOR_NONE;
}

#ifdef ENABLE_ZONAL_CONTROL
void sensors_set_enabled(const char* sensor_name, bool_t state) {
    if (strcmp(sensor_name, "PIR1") == 0) {
        sensor_enabled[0] = state;
    } else if (strcmp(sensor_name, "PIR2") == 0) {
        sensor_enabled[1] = state;
    } else if (strcmp(sensor_name, "DOOR") == 0) {
        sensor_enabled[2] = state;
    }
    // Opcional: Enviar un ACK de confirmación
    // communication_send_string("ACK:SENSOR_STATE_UPDATED\n");
}
#endif
