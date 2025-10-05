/*=============================================================================
 * Program: TestLEDS
 * Date: 2025/10/04
 * Version: 1.0
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "TestLEDS.h"
#include "sapi.h"

static void setRgb(bool r, bool g, bool b) {
   gpioWrite(LEDR, r);
   gpioWrite(LEDG, g);
   gpioWrite(LEDB, b);
}

int main(void) {
   boardConfig();

   gpioWrite(LED1, ON);
   gpioWrite(LED2, ON);
   gpioWrite(LED3, ON);

   const delay_t colorDelay = { .startTime = 0, .duration = 500 };
   delay_t rgbDelay = colorDelay;
   delayInit(&rgbDelay, colorDelay.duration);

   uint8_t colorIndex = 0;

   while (true) {
      if (delayRead(&rgbDelay)) {
         switch (colorIndex) {
            case 0: setRgb(ON, OFF, OFF); break;
            case 1: setRgb(OFF, ON, OFF); break;
            case 2: setRgb(OFF, OFF, ON); break;
            case 3: setRgb(ON, ON, OFF); break;
            case 4: setRgb(OFF, ON, ON); break;
            case 5: setRgb(ON, OFF, ON); break;
            default:setRgb(ON, ON, ON); break;
         }
         colorIndex = (colorIndex + 1) % 7;
      }
   }

   return 0;
}
