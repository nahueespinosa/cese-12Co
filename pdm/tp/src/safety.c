/*=============================================================================
 * Author: Nahuel Espinosa <nahue.espinosa@gmail.com>
 * Date: 2020/08/17
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "sapi.h"

#include "safety.h"
#include "relay.h"

/*=====[Implementation of public functions]==================================*/

void safetyIsolatedModeEnable(void) {
   relayWrite(RELAY_CT_1, ON);
   relayWrite(RELAY_CT_2, OFF);
   relayWrite(RELAY_CT_3, ON);

   relayWrite(RELAY_FE_1, ON);
   relayWrite(RELAY_FE_2, OFF);
   relayWrite(RELAY_FE_3, ON);

   relayWrite(RELAY_REG_4, ON);
}

void safetyIsolatedModeDisable(void) {
   relayWrite(RELAY_CT_1, OFF);
   relayWrite(RELAY_CT_2, OFF);
   relayWrite(RELAY_CT_3, OFF);

   relayWrite(RELAY_FE_1, OFF);
   relayWrite(RELAY_FE_2, OFF);
   relayWrite(RELAY_FE_3, OFF);

   relayWrite(RELAY_REG_4, OFF);
}

void safetySignalActivateCT(void) {
   if( relayRead(RELAY_CT_1) && relayRead(RELAY_CT_3) ) {
      relayWrite(RELAY_CT_2, ON);
      relayWrite(RELAY_REG_2, ON);
   }
}

void safetySignalDeactivateCT(void) {
   if( relayRead(RELAY_CT_1) && relayRead(RELAY_CT_3) ) {
      relayWrite(RELAY_CT_2, OFF);
      relayWrite(RELAY_REG_2, OFF);
   }
}

void safetySignalActivateFE(void) {
   if( relayRead(RELAY_FE_1) && relayRead(RELAY_FE_3) ) {
      relayWrite(RELAY_FE_2, ON);
      relayWrite(RELAY_REG_3, ON);
   }
}

void safetySignalDeactivateFE(void) {
   if( relayRead(RELAY_FE_1) && relayRead(RELAY_FE_3) ) {
      relayWrite(RELAY_FE_2, OFF);
      relayWrite(RELAY_REG_3, OFF);
   }
}
