/*=============================================================================
 * Author: Nahuel Espinosa <nahue.espinosa@gmail.com>
 * Date: 2020/06/25
 *
 * Resumen: Prender secuencialmente los leds de la placa LED1, LED2, LED3, LEDB
 *
 * Condiciones de funcionamiento:
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
#define IS_NOT_LAST(ptr, arr)   (void *)*(ptr+1) != NULL
#define IS_NOT_FIRST(ptr, arr)  arr != ptr

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Main function, program entry point after power on or reset]==========*/

int main( void )
{
   uint8_t paso = 0;

   gpioMap_t secuencia1[] = {LEDB, LED1, LED2, LED3};
   gpioMap_t secuencia2[] = {LED3, LED2, LED1, LEDB};
   gpioMap_t secuencia3[] = {LEDB, LED2, LED1, LED3};
   gpioMap_t secuencia4[] = {LEDB, LED1, LED3, LED2};

   gpioMap_t * secuencias[]  = {
      secuencia1, secuencia2, secuencia3, secuencia4, NULL
   };
   gpioMap_t ** ptr_secuencia = secuencias;

   uint32_t tiempoEncendido[] = {150, 300, 750, (uint32_t) NULL};
   uint32_t * ptr_tiempo = tiempoEncendido;

   delay_t delayEncendido;

   // ----- Setup -----------------------------------
   boardInit();
   delayInit(&delayEncendido, *ptr_tiempo);

   // ----- Repeat for ever -------------------------
   while( true ) {

       if( delayRead(&delayEncendido) ) {
          gpioWrite((*ptr_secuencia)[paso], OFF);
          paso++;
          paso %= sizeof(secuencia1);
          gpioWrite((*ptr_secuencia)[paso], ON);
       }

       // ----- Inputs ------------------------------
       if( !gpioRead(TEC1) ) {
          delay(40);

          if( IS_NOT_LAST(ptr_secuencia, secuencias) ) {
             ptr_secuencia++;
          }

          while( !gpioRead(TEC1) );
          delay(40);
       }

       if( !gpioRead(TEC4) ) {
          delay(40);

          if( IS_NOT_FIRST(ptr_secuencia, secuencias) ) {
             ptr_secuencia--;
          }

          while( !gpioRead(TEC4) );
          delay(40);
       }

       if( !gpioRead(TEC2) ) {
          delay(40);

          if( IS_NOT_LAST(ptr_tiempo, tiempoEncendido) ) {
             ptr_tiempo++;
             delayWrite(&delayEncendido, *ptr_tiempo);
          }

          while( !gpioRead(TEC2) );
          delay(40);
       }

       if( !gpioRead(TEC3) ) {
          delay(40);

          if( IS_NOT_FIRST(ptr_tiempo, tiempoEncendido) ) {
             ptr_tiempo--;
             delayWrite(&delayEncendido, *ptr_tiempo);
          }

          while( !gpioRead(TEC3) );
          delay(40);
       }
   }

   // YOU NEVER REACH HERE, because this program runs directly or on a
   // microcontroller and is not called by any Operating System, as in the 
   // case of a PC program.
   return 0;
}
