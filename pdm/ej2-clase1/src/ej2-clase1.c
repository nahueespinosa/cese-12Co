/*=============================================================================
 * Author: Nahuel Espinosa <nahue.espinosa@gmail.com>
 * Date: 2020/06/25
 *
 * Resumen: Prender secuencialmente los leds de la placa LED1, LED2, LED3, LEDB
 *
 * Condiciones de funcionamiento
 * - Una vez que se pasa al siguiente led los demás deberán apagarse.
 * - Utilizar solamente LED azul de los leds RGB
 * - Controlar el sentido de la secuencia con los botones TEC1 y TEC4:
 *     - Inicialmente la secuencia es LEDB->LED1->LED2->LED3->LEDA...
 *     - Al presionar TEC4 la secuencia se recorre en sentido inverso, esto es:
 *     LED3->LED2->LED1->LEDA->LED3...
 *     - Al presionar TEC1 la secuencia vuelve al orden inicial.
 * - Al presionar TEC2 cada led queda encendido 150 ms.
 * - Al presionar TEC3 cada led queda encendido 750 ms.
 *
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "ej2-clase1.h"
#include "sapi.h"

/*=====[Definition macros of private constants]==============================*/

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Main function, program entry point after power on or reset]==========*/

int main( void )
{
   // ----- Setup -----------------------------------
   boardInit();

   // ----- Repeat for ever -------------------------
   while( true ) {
      gpioToggle(LED);
      delay(500);
   }

   // YOU NEVER REACH HERE, because this program runs directly or on a
   // microcontroller and is not called by any Operating System, as in the 
   // case of a PC program.
   return 0;
}
