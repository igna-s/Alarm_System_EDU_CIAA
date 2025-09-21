/*
 * camera_control.c
 *
 * Created on: 08/09/2025
 * Author: Equipo de Proyecto
 */
#include "camera_control.h"

#ifdef ENABLE_CAMERA_CONTROL

void camera_control_init(void) {
    uartConfig(CAMERA_UART, CAMERA_BAUD_RATE);
}

void camera_control_start_stream(void) {
    uartWriteString(CAMERA_UART, "STREAM_ON\n");
}

void camera_control_stop_stream(void) {
    uartWriteString(CAMERA_UART, "STREAM_OFF\n");
}

#endif /* ENABLE_CAMERA_CONTROL */
