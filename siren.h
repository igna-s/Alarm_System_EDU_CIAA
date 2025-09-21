/*
 * siren.h
 *
 * Created on: 08/09/2025
 * Author: Equipo de Proyecto
 *
 * Interfaz para el módulo de control de la sirena/buzzer.
 */
#ifndef SIREN_H_
#define SIREN_H_

#include "sapi.h"
#include "config.h"

// Inicializa el pin de la sirena
void siren_init(void);

// Enciende la sirena
void siren_on(void);

// Apaga la sirena
void siren_off(void);

#endif /* SIREN_H_ */
