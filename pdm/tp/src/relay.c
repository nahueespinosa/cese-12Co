/*=============================================================================
 * Author: Nahuel Espinosa <nahue.espinosa@gmail.com>
 * Date: 2020/08/11
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "relay.h"
#include "switch.h"

/*=====[Definition macros of private constants]==============================*/

typedef struct {
   gpioMap_t controlPin;
   gpioMap_t statePin;
   uint32_t  errorCount;
} relayConfig_t;

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

static relayConfig_t relayConfig[RELAY_NUM] = {
// {controlPin, statusPin, errorCount}
   {GPIO1     , GPIO7    , 0},    // RELAY_CT_1
   {LCD1      , GPIO5    , 0},    // RELAY_CT_2
   {LCD2      , GPIO3    , 0},    // RELAY_CT_3
   {ENET_TXD1 , LCD3     , 0},    // RELAY_FE_1
   {ENET_TXD0 , LCDRS    , 0},    // RELAY_FE_2
   {ENET_MDIO , LCD4     , 0},    // RELAY_FE_3
   {T_COL1    , DI7      , 0},    // RELAY_REG_1
   {T_FIL3    , T_FIL0   , 0},    // RELAY_REG_2
   {T_COL0    , T_FIL2   , 0},    // RELAY_REG_3
   {T_COL2    , T_FIL1   , 0},    // RELAY_REG_4
};

/*=====[Prototypes (declarations) of private functions]======================*/

static bool_t relayControlRead(relay_t relay);

/*=====[Implementation of public functions]==================================*/

void relayInit() {
   int i;

   for( i = 0 ; i < RELAY_NUM ; i++ ) {
      gpioWrite(relayConfig[i].controlPin, OFF);
      gpioConfig(relayConfig[i].controlPin, GPIO_OUTPUT);
      gpioConfig(relayConfig[i].statePin, GPIO_INPUT);
      relayConfig[i].errorCount = 0;
   }
}

void relayWrite(relay_t relay, bool_t value) {
   gpioWrite(relayConfig[relay].controlPin, value);
}

bool_t relayRead(relay_t relay) {
   return !gpioRead(relayConfig[relay].statePin);
}

bool_t relayCheck(relay_t relay) {
   bool_t retValue = TRUE;

   if( relayConfig[relay].errorCount >= RELAY_ERROR_THR ) {
      retValue = FALSE;
   }

   return retValue;
}

bool_t relayCheckAll() {
   bool_t retValue = TRUE;
   int i;

   for( i = 0 ; i < RELAY_NUM ; i++ ) {
      if( relayCheck(i) == FALSE ) {
         retValue = FALSE;
      }
   }

   return retValue;
}

void relayUpdate() {
   int i;

   for( i = 0 ; i < RELAY_NUM ; i++ ) {
      if( relayControlRead(i) != relayRead(i) ) {
         switch(i) {
         case RELAY_CT_1:
         case RELAY_CT_2:
         case RELAY_FE_1:
         case RELAY_FE_2:
            /* No se pueden detectar inconsistencias si la llave está
             * deshabilitada porque estos relés se desactivan por hardware. */
            if( relayCheck(i) == TRUE && switchRead() == ON ) {
               relayConfig[i].errorCount++;
            }
            break;
         default:
            if( relayCheck(i) == TRUE ) {
               relayConfig[i].errorCount++;
            }
            break;
         }
      } else {
         relayConfig[i].errorCount = 0;
      }
   }
}

/*=====[Implementation of private functions]=================================*/

static bool_t relayControlRead(relay_t relay) {
   return gpioRead(relayConfig[relay].controlPin);
}
