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

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Main function, program entry point after power on or reset]==========*/

int main( void )
{
   // ----- Setup -----------------------------------
   boardInit();

   // ----- Repeat for ever -------------------------
   while( true ) {
      gpioToggle(LED);
      delay(500);
   }

   // YOU NEVER REACH HERE, because this program runs directly or on a
   // microcontroller and is not called by any Operating System, as in the 
   // case of a PC program.
   return 0;
}
