/*=============================================================================
 * Author: Nahuel Espinosa <nahue.espinosa@gmail.com>
 * Date: 2020/08/11
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "main.h"

#include "sapi.h"
#include "fsm_salt.h"

/*=====[Definition macros of private constants]==============================*/

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Main function, program entry point after power on or reset]==========*/

int main(void)
{
   // ----- Setup -----------------------------------
   boardInit();
   saltInit();

   // ----- Repeat for ever -------------------------
   while( true ) {
      relayUpdate();
      switchUpdate();
      saltUpdate();
      delay(10);
   }

   return 0;
}




