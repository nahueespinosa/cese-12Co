/*=============================================================================
 * Author: Nahuel Espinosa <nahue.espinosa@gmail.com>
 * Date: 2020/07/09
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "main.h"

#include "semaforo.h"
#include "sapi.h"

/*=====[Definition macros of private constants]==============================*/

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Main function, program entry point after power on or reset]==========*/

int main( void )
{
   bool_t modo = TRUE;
   // ----- Setup -----------------------------------
   boardInit();

   // ----- Repeat for ever -------------------------
   while( true ) {
      if( !gpioRead(TEC1) ) {
         modo = !modo;
         delay(200);
      }

      if( TRUE == modo ) {
         semaforoOnline();
      } else {
         semaforoOffline();
      }
   }

   // YOU NEVER REACH HERE, because this program runs directly or on a
   // microcontroller and is not called by any Operating System, as in the 
   // case of a PC program.
   return 0;
}
