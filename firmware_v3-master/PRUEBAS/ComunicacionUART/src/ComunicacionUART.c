/*=============================================================================
 * Program: ComunicacionUART
 * Date: 2025/10/04
 * Version: 1.0
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "ComunicacionUART.h"
#include "sapi.h"
#include <string.h>

#define UART_LINK    UART_232   // enlace con ESP32
#define UART_MONITOR UART_USB   // consola de depuración
#define WAIT_MESSAGE "UART en espera...\r\n"

static void resetLeds(void) {
   gpioWrite(LED1, OFF);
   gpioWrite(LED2, OFF);
   gpioWrite(LED3, OFF);
   gpioWrite(LEDB, OFF);
   gpioWrite(LEDR, OFF);
   gpioWrite(LEDG, ON);
}

static void mostrarEvento(char codigo) {
   switch (codigo) {
      case 'X':
         gpioWrite(LED1, ON);
         break;
      case 'C':
         gpioWrite(LED2, ON);
         break;
      case 'O':
         gpioWrite(LED3, ON);
         break;
      case 'T':
         gpioWrite(LEDB, ON);
         break;
      default:
         gpioWrite(LED1, ON);
         gpioWrite(LED2, ON);
         gpioWrite(LEDR, ON);
         break;
   }
}

int main(void) {
   boardConfig();

   uartConfig(UART_LINK,    115200);
   uartConfig(UART_MONITOR,  9600);

   gpioConfig(LED1, GPIO_OUTPUT);
   gpioConfig(LED2, GPIO_OUTPUT);
   gpioConfig(LED3, GPIO_OUTPUT);
   gpioConfig(LEDB, GPIO_OUTPUT);
   gpioConfig(LEDR, GPIO_OUTPUT);
   gpioConfig(LEDG, GPIO_OUTPUT);

   resetLeds();

   delay_t waitDelay;
   delayInit(&waitDelay, 250);

   while (true) {
      uint8_t rxByte;

      if (delayRead(&waitDelay)) {
         uartWriteString(UART_MONITOR, WAIT_MESSAGE);
      }

      if (uartReadByte(UART_LINK, &rxByte)) {
         gpioWrite(LEDG, OFF);
         uartWriteString(UART_MONITOR, "Dato recibido: ");
         uartWriteByte(UART_MONITOR, rxByte);
         uartWriteString(UART_MONITOR, "\r\n");

         mostrarEvento((char)rxByte);

         delay(200);
         resetLeds();
      }
   }

   return 0;
}

