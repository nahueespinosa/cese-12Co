/*=============================================================================
 * Author: Nahuel Espinosa <nahue.espinosa@gmail.com>
 * Date: 2020/08/17
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "sapi.h"
#include "switch.h"

/*=====[Definition macros of private constants]==============================*/

//! GPIO de entrada de la llave rotativa
#define SWITCH_GPIO           GPIO8

//! Umbral de detección para el cambio de estado
#define SWITCH_STATE_THR      20

/*=====[Definitions of private global variables]=============================*/

//! Variable interna de estado de la llave rotativa
static bool_t switchState = OFF;

/*=====[Implementation of public functions]==================================*/

void switchInit(void) {
   gpioConfig(SWITCH_GPIO, GPIO_INPUT_PULLUP);
}

bool_t switchReadCurrent(void) {
   return !gpioRead(SWITCH_GPIO);
}

bool_t switchRead(void) {
   return switchState;
}

void switchUpdate(void) {
   static uint32_t stateCount = 0;

   if( switchState == OFF ) {
      if( switchReadCurrent() ) {
         stateCount++;

         if( stateCount >= SWITCH_STATE_THR ) {
            stateCount = 0;
            switchState = ON;
         }
      } else {
         stateCount = 0;
      }
   } else {
      if( !switchReadCurrent() ) {
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
