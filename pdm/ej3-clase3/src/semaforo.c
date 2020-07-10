/*=============================================================================
 * Author: Nahuel Espinosa <nahue.espinosa@gmail.com>
 * Date: 2020/07/09
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "led.h"
#include "sapi.h"

/*=====[Definition macros of private constants]==============================*/

#define TIEMPO_ROJO           1000
#define TIEMPO_ROJOAMARILLO   700
#define TIEMPO_VERDE          800
#define TIEMPO_AMARILLO       900
#define TIEMPO_BLINK          100

typedef enum {
   ESTADO_ROJO,
   ESTADO_ROJOAMARILLO,
   ESTADO_VERDE,
   ESTADO_AMARILLO,
   ESTADO_OFFLINE
} estado_t;

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

static delay_t semaforoDelay;
static estado_t estado = ESTADO_ROJO;

/*=====[Definitions of private functions]====================================*/

static void semaforoMEF( void )
{
   switch( estado ) {
      case ESTADO_ROJO:
         if( delayRead(&semaforoDelay) ) {
            estado = ESTADO_ROJOAMARILLO;
            ledApagarTodos();
            ledEncenderRojo();
            ledEncenderAmarillo();
            delayConfig(&semaforoDelay, TIEMPO_ROJOAMARILLO);
         }
         break;
      case ESTADO_ROJOAMARILLO:
         if( delayRead(&semaforoDelay) ) {
            estado = ESTADO_VERDE;
            ledApagarTodos();
            ledEncenderVerde();
            delayConfig(&semaforoDelay, TIEMPO_VERDE);
         }
         break;
      case ESTADO_VERDE:
         if( delayRead(&semaforoDelay) ) {
            estado = ESTADO_AMARILLO;
            ledApagarTodos();
            ledEncenderAmarillo();
            delayConfig(&semaforoDelay, TIEMPO_AMARILLO);
         }
         break;
      case ESTADO_AMARILLO:
         if( delayRead(&semaforoDelay) ) {
            estado = ESTADO_ROJO;
            ledApagarTodos();
            ledEncenderRojo();
            delayConfig(&semaforoDelay, TIEMPO_ROJO);
         }
         break;
      case ESTADO_OFFLINE:
         if( delayRead(&semaforoDelay) ) {
            ledTogglearAmarillo();
            delayConfig(&semaforoDelay, TIEMPO_BLINK);
         }
         break;
   }
}

/*=====[Definitions of public functions]=====================================*/

void semaforoInit( void )
{
   ledApagarTodos();
   ledEncenderRojo();
   delayConfig(&semaforoDelay, TIEMPO_ROJO);
}

void semaforoOnline( void )
{
   if( ESTADO_OFFLINE == estado ) {
      estado = ESTADO_ROJO;
      ledApagarTodos();
      ledEncenderRojo();
      delayConfig(&semaforoDelay, TIEMPO_ROJO);
   }

   semaforoMEF();
}

void semaforoOffline( void )
{
   if( ESTADO_OFFLINE != estado ) {
      ledApagarTodos();
      estado = ESTADO_OFFLINE;
      delayConfig(&semaforoDelay, TIEMPO_BLINK);
   }

   semaforoMEF();
}



