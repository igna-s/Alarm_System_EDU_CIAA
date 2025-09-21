/*
 * communication.h
 *
 * Created on: 08/09/2025
 * Author: Equipo de Proyecto
 *
 * Interfaz para el módulo de comunicación UART.
 */
#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_

#include "sapi.h"
#include "config.h"

// Inicializa la UART seleccionada
void communication_init(void);

// Envía una cadena de caracteres a través de la UART
void communication_send_string(const char* str);

// Lee caracteres de la UART de forma no bloqueante hasta encontrar un '\n'.
// Devuelve TRUE si se ha recibido una línea completa, FALSE en caso contrario.
// La línea recibida se almacena en el 'buffer'.
bool_t communication_read_line(char* buffer, uint32_t bufferSize);

#endif /* COMMUNICATION_H_ */
