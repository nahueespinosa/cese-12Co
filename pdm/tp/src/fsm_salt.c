/*=============================================================================
 * Author: Nahuel Espinosa <nahue.espinosa@gmail.com>
 * Date: 2020/08/11
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "sapi.h"
#include "safety.h"
#include "fsm_salt.h"

/*=====[Definition macros of private constants]==============================*/

//! Definici�n de estados del m�dulo SAL/T
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

   static bool_t fsmDisabledFlag = false;
   static bool_t fsmEnabledFlag = false;
   static bool_t fsmErrorFlag = false;

   switch(state) {
   case DISABLED:
      // Punto de entrada
      if( fsmDisabledFlag == false ) {
         safetyIsolatedModeDisable();
         gpioWrite(LEDG, OFF);
         gpioWrite(LEDR, OFF);
         fsmDisabledFlag = true;
      }

      // Revisi�n de condiciones de transici�n
      if( switchRead() == ON ) {
         state = ENABLED;
      }

      // Punto de salida
      if( state != DISABLED ) {
         fsmDisabledFlag = false;
      }
      break;

   case ENABLED:
      // Punto de entrada
      if( fsmEnabledFlag == false ) {
         safetyIsolatedModeEnable();
         gpioWrite(LEDG, ON);
         fsmEnabledFlag = true;
      }

      // Revisi�n de condiciones de transici�n
      if( relayCheckAll() == FALSE ) {
         state = ERROR_STATE;
      }

      /* La transici�n al estado deshabilitado tiene m�s prioridad que la
       * detecci�n de un error en los rel�s porque los mismos se desactivan
       * por hardware y en ese punto no se pueden detectar inconsistencias. */
      if( switchRead() == OFF ) {
         state = DISABLED;
      }

      // Punto de salida
      if( state != ENABLED ) {
         fsmEnabledFlag = false;
      }
      break;

   case ERROR_STATE:
      // Punto de entrada
      if( fsmErrorFlag == false ) {
         safetyIsolatedModeDisable();
         gpioWrite(LEDR, ON);
         fsmErrorFlag = true;
      }

      // Revisi�n de condiciones de transici�n
      if( switchRead() == OFF ) {
         state = DISABLED;
      }

      // Punto de salida
      if( state != ERROR_STATE ) {
         fsmErrorFlag = false;
      }
      break;

   default:
      state = DISABLED;
      break;
   }
}

/*=====[Implementation of private functions]=================================*/
