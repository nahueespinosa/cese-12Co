/*=============================================================================
 * Author: Nahuel Espinosa <nahue.espinosa@gmail.com>
 * Date: 2020/08/11
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "sapi.h"
#include "safety.h"
#include "fsm_salt.h"

/*=====[Definition macros of private constants]==============================*/

//! Definición de estados del módulo SAL/T
typedef enum {DISABLED, ENABLED, ERROR_STATE} saltState_t;

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Prototypes (declarations) of private functions]======================*/

/*=====[Implementation of public functions]==================================*/

void saltInit() {
   relayInit();
   switchInit();
}

void saltUpdate() {
   static saltState_t state = DISABLED;

   static bool_t fsmDisabledFlag = OFF;
   static bool_t fsmEnabledFlag = OFF;
   static bool_t fsmErrorFlag = OFF;

   switch(state) {
   case DISABLED:
      // Punto de entrada
      if( fsmDisabledFlag == OFF ) {
         safetyIsolatedModeDisable();
         gpioWrite(LEDG, OFF);
         gpioWrite(LEDR, OFF);
         fsmDisabledFlag = ON;
      }

      // Revisión de condiciones de transición
      if( switchRead() == ON ) {
         state = ENABLED;
      }

      // Punto de salida
      if( state != DISABLED ) {
         fsmDisabledFlag = OFF;
      }
      break;

   case ENABLED:
      // Punto de entrada
      if( fsmEnabledFlag == OFF ) {
         safetyIsolatedModeEnable();
         gpioWrite(LEDG, ON);
         fsmEnabledFlag = ON;
      }

      // Revisión de condiciones de transición
      relayUpdate();

      if( relayCheckAll() == FALSE ) {
         state = ERROR_STATE;
      }

      switchUpdate();

      if( switchRead() == OFF ) {
         state = DISABLED;
      }

      // Punto de salida
      if( state != ENABLED ) {
         fsmEnabledFlag = OFF;
      }
      break;

   case ERROR_STATE:
      // Punto de entrada
      if( fsmErrorFlag == OFF ) {
         safetyIsolatedModeDisable();
         gpioWrite(LEDR, ON);
         fsmErrorFlag = ON;
      }

      // Revisión de condiciones de transición
      if( switchRead() == OFF ) {
         state = DISABLED;
      }

      // Punto de salida
      if( state != ERROR_STATE ) {
         fsmErrorFlag = OFF;
      }
      break;
   default:
      state = DISABLED;
      break;
   }
}

/*=====[Implementation of private functions]=================================*/
