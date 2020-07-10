/*=============================================================================
 * Author: Nahuel Espinosa <nahue.espinosa@gmail.com>
 * Date: 2020/07/09
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "sapi.h"

/*=====[Definition macros of private constants]==============================*/

#define LED_ROJO        LED2
#define LED_AMARILLO    LED1
#define LED_VERDE       LED3

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Definitions of public functions]=====================================*/

void ledEncenderTodos( void )
{
   gpioWrite(LED_ROJO, ON);
   gpioWrite(LED_AMARILLO, ON);
   gpioWrite(LED_VERDE, ON);
}

void ledApagarTodos( void )
{
   gpioWrite(LED_ROJO, OFF);
   gpioWrite(LED_AMARILLO, OFF);
   gpioWrite(LED_VERDE, OFF);
}

void ledEncenderRojo( void )
{
   gpioWrite(LED_ROJO, ON);
}

void ledEncenderAmarillo( void )
{
   gpioWrite(LED_AMARILLO, ON);
}

void ledEncenderVerde( void )
{
   gpioWrite(LED_VERDE, ON);
}

void ledTogglearAmarillo( void )
{
   gpioToggle(LED_AMARILLO);
}
