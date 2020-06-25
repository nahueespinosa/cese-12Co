/*=============================================================================
 * Author: Nahuel Espinosa <nahue.espinosa@gmail.com>
 * Date: 2020/06/25
 *
 * Description:
 *    El programa espera a que un botón sea pulsado. Mientras el botón está
 *    pulsado, se mide el tiempo utilizando un contador hasta que el botón es
 *    liberado. En ese momento, se enciende el led asociado al botón durante
 *    el tiempo calculado.
 *
 *          TEC1 -> LED1
 *          TEC2 -> LED2
 *          TEC3 -> LED3
 *
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "ej1-clase1.h"
#include "sapi.h"

/*=====[Definition macros of private constants]==============================*/
typedef enum {
   IDLE,
   TEC_PRESSED,
   LED_ON
} fsmStatus_t;

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Main function, program entry point after power on or reset]==========*/

int main( void )
{
   uint8_t  current_idx;                        // Current index of pressed button
   uint32_t tick_counter;                       // Time ticks counter (1 ms)

   gpioMap_t   buttons[] = {TEC1, TEC2, TEC3};  // Buttons to test
   gpioMap_t   leds[]    = {LED1, LED2, LED3};  // Leds to turn on for each button
   fsmStatus_t status    = IDLE;

   // ----- Setup -----------------------------------
   boardInit();

   // ----- Repeat for ever -------------------------
   while( true ) {

      // ----- Finite State Machine --------------------
      switch(status) {
         case IDLE:
            for( current_idx = 0 ; current_idx < sizeof(buttons) ; current_idx++ ) {
               if( !gpioRead(buttons[current_idx]) && current_idx < sizeof(leds)) {
                  status = TEC_PRESSED;
                  tick_counter = 0;
                  break;
               }
            }
            break;

         case TEC_PRESSED:
            if( !gpioRead(buttons[current_idx]) ) {
               tick_counter++;
            } else {
               status = LED_ON;
            }
            break;

         case LED_ON:
            if( tick_counter > 0 ) {
               tick_counter--;
               gpioWrite(leds[current_idx], ON);
            } else {
               gpioWrite(leds[current_idx], OFF);
               status = IDLE;
            }
            break;
      }

      delay(1);   // Time ticks delay (1 ms)
   }

   // YOU NEVER REACH HERE, because this program runs directly or on a
   // microcontroller and is not called by any Operating System, as in the 
   // case of a PC program.
   return 0;
}
