/*=============================================================================
 * Author: Nahuel Espinosa <nahue.espinosa@gmail.com>
 * Date: 2020/08/17
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "relay.h"
#include "switch.h"

/*=====[Definition macros of private constants]==============================*/

//! Umbral de detección para inconsistencias en los relés
#define RELAY_ERROR_THR    10

/*=====[Definition macros of private constants]==============================*/

//! Estructura para almacenar los GPIO correspondientes a cada pin y su estado
typedef struct {
   gpioMap_t controlPin;
   gpioMap_t statePin;
   uint32_t  errorCount;
} relayConfig_t;

/*=====[Definitions of private global variables]=============================*/

//! Vector que almacena configuración y el estado de cada relé
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

/**
 * @brief Leer el valor del pin de control del relé
 *
 * @param[in]   relay    Identificador del relé
 * @return      ON       El pin está en un estado lógico alto
 * @return      OFF      El pin está en un estado lógico bajo
 */
static bool_t relayControlRead(relay_t relay);

/*=====[Implementation of public functions]==================================*/

void relayInit(void) {
   int i;

   for( i = 0 ; i < RELAY_NUM ; i++ ) {
      gpioWrite(relayConfig[i].controlPin, OFF);
      gpioConfig(relayConfig[i].controlPin, GPIO_OUTPUT);
      gpioConfig(relayConfig[i].statePin, GPIO_INPUT_PULLUP);
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

bool_t relayCheckAll(void) {
   bool_t retValue = TRUE;
   int i;

   for( i = 0 ; i < RELAY_NUM ; i++ ) {
      if( relayCheck(i) == FALSE ) {
         retValue = FALSE;
      }
   }

   return retValue;
}

void relayUpdate(void) {
   int i;

   for( i = 0 ; i < RELAY_NUM ; i++ ) {
      switch(i) {
      case RELAY_CT_1:
      case RELAY_CT_2:
      case RELAY_FE_1:
      case RELAY_FE_2:
         /* Solo se pueden detectar inconsistencias si la llave está habilitada,
          * porque de lo contrario estos relés se desactivan por hardware. */
         if( relayControlRead(i) != relayRead(i) && switchReadCurrent() == ON ) {
            relayConfig[i].errorCount++;
         } else {
            relayConfig[i].errorCount = 0;
         }
         break;
      default:
         if( relayControlRead(i) != relayRead(i) ) {
            relayConfig[i].errorCount++;
         } else {
            relayConfig[i].errorCount = 0;
         }
         break;
      }

      /* Si se supera el RELAY_ERROR_THR se deja al límite para que no
       * desborde la variable. */
      if( relayConfig[i].errorCount > RELAY_ERROR_THR ) {
         relayConfig[i].errorCount = RELAY_ERROR_THR;
      }
   }
}

/*=====[Implementation of private functions]=================================*/

static bool_t relayControlRead(relay_t relay) {
   return gpioRead(relayConfig[relay].controlPin);
}
