/*
 * siren.c
 *
 * Created on: 08/09/2025
 * Author: Equipo de Proyecto
 *
 * Implementación del módulo de control de la sirena.
 */
#include "siren.h"

void siren_init(void) {
    gpioConfig(SIREN_PIN, GPIO_OUTPUT);
    gpioWrite(SIREN_PIN, LOW); // Asegurarse de que esté apagada al inicio
}

void siren_on(void) {
    gpioWrite(SIREN_PIN, HIGH);
}

void siren_off(void) {
    gpioWrite(SIREN_PIN, LOW);
}
