/*=============================================================================
 * Author: Nahuel Espinosa <nahue.espinosa@gmail.com>
 * Date: 2020/08/11
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "sapi.h"

#include "safety.h"
#include "relay.h"

/*=====[Definition macros of private constants]==============================*/

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Prototypes (declarations) of private functions]======================*/

/*=====[Implementation of public functions]==================================*/

void safetyIsolatedModeEnable() {
   relayWrite(RELAY_CT_1, ON);
   relayWrite(RELAY_CT_2, OFF);
   relayWrite(RELAY_CT_3, ON);

   relayWrite(RELAY_FE_1, ON);
   relayWrite(RELAY_FE_2, OFF);
   relayWrite(RELAY_FE_3, ON);
}

void safetyIsolatedModeDisable() {
   relayWrite(RELAY_CT_1, OFF);
   relayWrite(RELAY_CT_2, OFF);
   relayWrite(RELAY_CT_3, OFF);

   relayWrite(RELAY_FE_1, OFF);
   relayWrite(RELAY_FE_2, OFF);
   relayWrite(RELAY_FE_3, OFF);
}

void safetySignalActivateCT() {
   if( relayRead(RELAY_CT_1) && relayRead(RELAY_CT_3) ) {
      relayWrite(RELAY_CT_2, ON);
   }
}

void safetySignalDeactivateCT() {
   if( relayRead(RELAY_CT_1) && relayRead(RELAY_CT_3) ) {
      relayWrite(RELAY_CT_2, OFF);
   }
}

void safetySignalActivateFE() {
   if( relayRead(RELAY_FE_1) && relayRead(RELAY_FE_3) ) {
      relayWrite(RELAY_FE_2, ON);
   }
}

void safetySignalDeactivateFE() {
   if( relayRead(RELAY_FE_1) && relayRead(RELAY_FE_3) ) {
      relayWrite(RELAY_FE_2, OFF);
   }
}

/*=====[Implementation of private functions]=================================*/
