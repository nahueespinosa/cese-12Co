/*=============================================================================
 * Author: Nahuel Espinosa <nahue.espinosa@gmail.com>
 * Date: 2020/04/08
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
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "test.h"
#include "sapi.h"

/*=====[Definition macros of private constants]==============================*/
#define TIEMPO_OPCION_1	150
#define TIEMPO_OPCION_2	750

typedef enum {SECUENCIA_DIRECTA, SECUENCIA_INVERSA} status_t;

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Main function, program entry point after power on or reset]==========*/

int main( void )
{
    uint8_t paso = 0;
    status_t estado = SECUENCIA_DIRECTA;
    gpioMap_t secuencia[]  = {LEDB, LED1, LED2, LED3};
    delay_t tiempo_encendido;

	// ----- Setup -----------------------------------
	boardInit();
	delayInit( &tiempo_encendido, TIEMPO_OPCION_1 );

	// ----- Repeat for ever -------------------------
	while( true ) {
	// ----- State Machine -----------------------
		switch(estado) {
			case SECUENCIA_DIRECTA:
				if( delayRead( &tiempo_encendido ) ) {
				gpioWrite(secuencia[paso], OFF);
				paso++;
				if( paso == sizeof(secuencia) ) paso = 0;
				gpioWrite(secuencia[paso], ON);
				}
			break;
			case SECUENCIA_INVERSA:
				if( delayRead( &tiempo_encendido ) ) {
				gpioWrite(secuencia[paso], OFF);
				if( paso == 0 ) paso = sizeof(secuencia);
				paso--;
				gpioWrite(secuencia[paso], ON);
				}
			break;
		}

		// ----- Inputs ------------------------------
		if( !gpioRead(TEC1) ) {
			estado = SECUENCIA_DIRECTA;
		}
		if( !gpioRead(TEC4) ) {
			estado = SECUENCIA_INVERSA;
		}
		if( !gpioRead(TEC2) ) {
			delayWrite( &tiempo_encendido, TIEMPO_OPCION_1 );
		}
		if( !gpioRead(TEC3) ) {
			delayWrite( &tiempo_encendido, TIEMPO_OPCION_2 );
		}
	}

	// YOU NEVER REACH HERE, because this program runs directly or on a
	// microcontroller and is not called by any Operating System, as in the
	// case of a PC program.
	return 0;
}
