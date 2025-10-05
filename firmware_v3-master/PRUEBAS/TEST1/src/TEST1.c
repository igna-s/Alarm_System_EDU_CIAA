/*=============================================================================
 * Program: TEST1
 * Date: 2025/10/04
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "TEST1.h"
#include "sapi.h"


#define PIR1_GPIO   GPIO1   // reemplazar por el pin real
#define PIR2_GPIO   GPIO3
#define PIR3_GPIO   GPIO5
#define BUZZER_GPIO GPIO7

#define ALARM_TIMEOUT_MS   10000   // la sirena se apaga despues de 10 s
#define BUZZER_PERIOD_MS     200   // 100 ms ON / 100 ms OFF

int main(void) {
   boardInit();

   gpioInit(PIR1_GPIO, GPIO_INPUT);
   gpioInit(PIR2_GPIO, GPIO_INPUT);
   gpioInit(PIR3_GPIO, GPIO_INPUT);
   gpioInit(BUZZER_GPIO, GPIO_OUTPUT);

   delay_t scanDelay;
   delay_t buzzerDelay;
   delay_t alarmTimeout;

   delayInit(&scanDelay, 50);
   delayInit(&buzzerDelay, BUZZER_PERIOD_MS);
   delayInit(&alarmTimeout, ALARM_TIMEOUT_MS);

   bool alarmActive = false;
   bool buzzerState = OFF;

   while (true) {
      if (delayRead(&scanDelay)) {
         bool pir1 = gpioRead(PIR1_GPIO);
         bool pir2 = gpioRead(PIR2_GPIO);
         bool pir3 = gpioRead(PIR3_GPIO);
         if (pir1 || pir2 || pir3) {
            alarmActive = true;
            delayWrite(&alarmTimeout, ALARM_TIMEOUT_MS);
         } else if (alarmActive && delayRead(&alarmTimeout)) {
            alarmActive = false;
            buzzerState = OFF;
            gpioWrite(BUZZER_GPIO, OFF);
         }
      }

      if (alarmActive) {
         if (delayRead(&buzzerDelay)) {
            buzzerState = !buzzerState;
            gpioWrite(BUZZER_GPIO, buzzerState);
         }
      } else {
         gpioWrite(BUZZER_GPIO, OFF);
      }
   }

   return 0;
}