/*
 * communication.c
 *
 * Created on: 08/09/2025
 * Author: Equipo de Proyecto
 *
 * Implementación del módulo de comunicación UART no bloqueante.
 */
#include "communication.h"
#include <string.h>

void communication_init(void) {
    uartConfig(WIRELESS_UART, WIRELESS_BAUD_RATE);
}

void communication_send_string(const char* str) {
    uartWriteString(WIRELESS_UART, str);
}

bool_t communication_read_line(char* buffer, uint32_t bufferSize) {
    static uint32_t char_count = 0;
    uint8_t received_byte;

    if (uartReadByte(WIRELESS_UART, &received_byte)) {
        // Ignorar el retorno de carro
        if (received_byte == '\r') {
            return FALSE;
        }

        if (received_byte == '\n') {
            buffer[char_count] = '\0'; // Terminar la cadena
            char_count = 0;
            return TRUE; // Línea completa recibida
        } else if (char_count < (bufferSize - 1)) {
            buffer[char_count] = received_byte;
            char_count++;
        } else {
            // Error de desbordamiento, reiniciar el buffer
            char_count = 0;
        }
    }
    return FALSE; // Línea incompleta
}
