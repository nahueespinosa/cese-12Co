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

typedef struct {
   gpioMap_t   led;        // Led associated
   gpioMap_t   button;     // Button associated
   fsmStatus_t status;     // Current status
   uint32_t    ticks;      // Current tick counter
} ledButton_t;

void ledButtonFSM( ledButton_t *ledButton );

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Main function, program entry point after power on or reset]==========*/

int main( void )
{
   uint8_t i;

   // led, button, status, ticks
   ledButton_t ledButtons[] = {
      {LED1, TEC1, IDLE, 0},
      {LED2, TEC2, IDLE, 0},
      {LED3, TEC3, IDLE, 0}
   };

   delay_t tickDelay;

   // ----- Setup -----------------------------------
   boardInit();
   delayInit(&tickDelay, 1);   // Tick delay 1 ms

   // ----- Repeat for ever -------------------------
   while( true ) {

      if( delayRead(&tickDelay) ) {
         for( i = 0 ; i < sizeof(ledButtons) ; i++ ) {
            ledButtonFSM(&ledButtons[i]);
         }
      }

   }

   // YOU NEVER REACH HERE, because this program runs directly or on a
   // microcontroller and is not called by any Operating System, as in the 
   // case of a PC program.
   return 0;
}

void ledButtonFSM( ledButton_t *ledButton ) {

   switch( ledButton->status ) {
      case IDLE:
         if( !gpioRead(ledButton->button) ) {
            ledButton->status = TEC_PRESSED;
            ledButton->ticks = 0;
            break;
         }

      case TEC_PRESSED:
         if( !gpioRead(ledButton->button) ) {
            ledButton->ticks++;
         } else {
            ledButton->status = LED_ON;
         }
         break;

      case LED_ON:
         if( ledButton->ticks > 0 ) {
            ledButton->ticks--;
            gpioWrite(ledButton->led, ON);
         } else {
            gpioWrite(ledButton->led, OFF);
            ledButton->status = IDLE;
         }
         break;
   }

}
