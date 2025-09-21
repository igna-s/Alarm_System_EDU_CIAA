/*
 * main.c
 *
 * Created on: 08/09/2025
 * Author: Restucha, Tiago; Schwindt, Ignacio Andrés
 *
 * Firmware principal para el Sistema de Alarma Inteligente.
 * Implementa la lógica central como una máquina de estados finita (FSM).
 */
#include "sapi.h"
#include "config.h"
#include "sensors.h"
#include "siren.h"
#include "communication.h"
#include <string.h>

#ifdef ENABLE_EVENT_LOGGER
// #include "event_logger.h" // Descomentar si se implementa event_logger
#endif

#ifdef ENABLE_CAMERA_CONTROL
#include "camera_control.h"
#endif

// Definición de los estados de la alarma
typedef enum {
    ALARM_DISARMED,
    ALARM_ARMED,
    ALARM_TRIGGERED,
    ALARM_SOUNDING
} AlarmState_t;

// Prototipos de funciones locales
static void process_incoming_command(const char* command);
static void handle_disarmed_state(void);
static void handle_armed_state(void);
static void handle_triggered_state(void);
static void handle_sounding_state(void);
static void send_status_update(void);

// Variables globales del estado del sistema
static AlarmState_t current_state = ALARM_DISARMED;
static delay_t grace_period_timer;
static char command_buffer[COMMAND_BUFFER_SIZE];

int main(void) {
    // Inicialización de la placa y periféricos
    boardConfig();
    sensors_init();
    siren_init();
    communication_init();

    #ifdef ENABLE_CAMERA_CONTROL
    camera_control_init();
    #endif

    delay(1000); // Pequeña espera para que los módulos se estabilicen

    // Mensaje de bienvenida
    communication_send_string("ALARM_SYSTEM:READY\n");

    // Bucle principal del programa
    while (1) {
        // 1. Procesar comandos entrantes desde la app
        if (communication_read_line(command_buffer, COMMAND_BUFFER_SIZE)) {
            process_incoming_command(command_buffer);
        }

        // 2. Ejecutar la lógica del estado actual
        switch (current_state) {
            case ALARM_DISARMED:
                handle_disarmed_state();
                break;
            case ALARM_ARMED:
                handle_armed_state();
                break;
            case ALARM_TRIGGERED:
                handle_triggered_state();
                break;
            case ALARM_SOUNDING:
                handle_sounding_state();
                break;
        }
        delay(1); // Pequeño retardo para no saturar el CPU
    }
    return 0;
}

static void process_incoming_command(const char* command) {
    if (strcmp(command, "ARM") == 0) {
        if (current_state == ALARM_DISARMED) {
            current_state = ALARM_ARMED;
            send_status_update();
            #ifdef ENABLE_CAMERA_CONTROL
            camera_control_start_stream();
            #endif
        }
    } else if (strcmp(command, "DISARM") == 0) {
        siren_off(); // Siempre apagar la sirena al desarmar
        current_state = ALARM_DISARMED;
        send_status_update();
        #ifdef ENABLE_CAMERA_CONTROL
        camera_control_stop_stream();
        #endif
    } else if (strcmp(command, "STATUS?") == 0) {
        send_status_update();
    }
    #ifdef ENABLE_ZONAL_CONTROL
    // Formato esperado: "ENABLE:PIR1" o "DISABLE:DOOR"
    else if (strncmp(command, "ENABLE:", 7) == 0) {
        sensors_set_enabled(command + 7, TRUE);
    } else if (strncmp(command, "DISABLE:", 8) == 0) {
        sensors_set_enabled(command + 8, FALSE);
    }
    #endif
}

static void handle_disarmed_state() {
    // No hacer nada, esperar comando "ARM"
}

static void handle_armed_state() {
    SensorTriggered_t triggered_sensor = sensors_is_any_triggered();
    if (triggered_sensor != SENSOR_NONE) {
        current_state = ALARM_TRIGGERED;
        delayConfig(&grace_period_timer, GRACE_PERIOD_MS);
        send_status_update();

        // Notificar qué sensor se disparó
        switch(triggered_sensor) {
            case SENSOR_PIR1:
                communication_send_string("EVENT:PIR1\n");
                break;
            case SENSOR_PIR2:
                communication_send_string("EVENT:PIR2\n");
                break;
            case SENSOR_DOOR:
                communication_send_string("EVENT:DOOR\n");
                break;
            default:
                break;
        }
    }
}

static void handle_triggered_state() {
    if (delayRead(&grace_period_timer)) {
        current_state = ALARM_SOUNDING;
        siren_on();
        send_status_update();
    }
}

static void handle_sounding_state() {
    // La sirena permanece encendida hasta recibir un comando "DISARM"
}

static void send_status_update() {
    switch (current_state) {
        case ALARM_DISARMED:
            communication_send_string("STATUS:DISARMED\n");
            break;
        case ALARM_ARMED:
            communication_send_string("STATUS:ARMED\n");
            break;
        case ALARM_TRIGGERED:
            communication_send_string("STATUS:TRIGGERED\n");
            break;
        case ALARM_SOUNDING:
            communication_send_string("STATUS:SOUNDING\n");
            break;
    }
}
