/*
 * config.h
 *
 * Created on: 08/09/2025
 * Author: Equipo de Proyecto
 *
 * Archivo de configuración central para el Sistema de Alarma Inteligente.
 * Aquí se definen los pines, constantes y se activan/desactivan módulos.
 */
#ifndef CONFIG_H_
#define CONFIG_H_

// ============================================================================
// HABILITACIÓN DE FUNCIONALIDADES SECUNDARIAS
// ============================================================================
// Comentar o descomentar estas líneas para incluir/excluir funcionalidades
// del build final. Esto permite un desarrollo y prueba incremental.
#define ENABLE_ZONAL_CONTROL    // Habilita el control individual de sensores
// #define ENABLE_EVENT_LOGGER     // Habilita el registro de eventos en flash (Deshabilitado por defecto)
#define ENABLE_CAMERA_CONTROL   // Habilita el control del módulo de cámara

// ============================================================================
// CONFIGURACIÓN DE PINES (según la nomenclatura de sAPI)
// ============================================================================

// --- Sensores ---
#define PIR1_PIN                GPIO0
#define PIR2_PIN                GPIO1
#define MAG_SENSOR_PIN          GPIO2

// --- Actuadores ---
#define SIREN_PIN               GPIO3

// --- Comunicación ---
// UART para el módulo inalámbrico (Bluetooth/Wi-Fi)
#define WIRELESS_UART           UART_232
#define WIRELESS_BAUD_RATE      9600

#ifdef ENABLE_CAMERA_CONTROL
// UART para el control del módulo de cámara ESP32-CAM
#define CAMERA_UART             UART_USB
#define CAMERA_BAUD_RATE        115200
#endif

// ============================================================================
// CONSTANTES DEL SISTEMA
// ============================================================================

// Período de gracia en milisegundos después de que un sensor se activa
#define GRACE_PERIOD_MS         10000

// Tamaño máximo del buffer para leer comandos desde la app
#define COMMAND_BUFFER_SIZE     32

// Códigos de evento para el logger
#ifdef ENABLE_EVENT_LOGGER
typedef enum {
    EVENT_SYSTEM_ARMED = 1,
    EVENT_SYSTEM_DISARMED,
    EVENT_PIR1_TRIGGERED,
    EVENT_PIR2_TRIGGERED,
    EVENT_DOOR_TRIGGERED,
    EVENT_ALARM_SOUNDED
} EventCode_t;
#endif

#endif /* CONFIG_H_ */
