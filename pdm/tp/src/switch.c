/*=============================================================================
 * Author: Nahuel Espinosa <nahue.espinosa@gmail.com>
 * Date: 2020/08/11
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "sapi.h"
#include "switch.h"

/*=====[Definition macros of private constants]==============================*/

#define SWITCH_GPIO  GPIO8

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

static bool_t switchState = OFF;

/*=====[Prototypes (declarations) of private functions]======================*/

/*=====[Implementation of public functions]==================================*/

void switchInit() {
   gpioConfig(SWITCH_GPIO, GPIO_INPUT_PULLUP);
}

bool_t switchRead() {
   return switchState;
}

void switchUpdate() {
   static uint32_t stateCount = 0;

   if( switchState == OFF ) {
      if( !gpioRead(SWITCH_GPIO) ) {
         stateCount++;

         if( stateCount >= SWITCH_STATE_THR ) {
            stateCount = 0;
            switchState = ON;
         }
      } else {
         stateCount = 0;
      }
   } else {
      if( gpioRead(SWITCH_GPIO) ) {
         stateCount++;

         if( stateCount >= SWITCH_STATE_THR ) {
            stateCount = 0;
            switchState = OFF;
         }
      } else {
         stateCount = 0;
      }
   }
}

/*=====[Implementation of private functions]=================================*/
