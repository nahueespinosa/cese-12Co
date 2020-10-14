/*==================[inclusions]=============================================*/

#include "main.h"

#include "ajustarBrillo.h"
#include "board.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/** @brief hardware initialization function
 *	@return none
 */
static void pausems(uint32_t t);

/*==================[internal data definition]===============================*/

/** @brief used for delay counter */
static uint32_t pausems_count;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static void initHardware(void)
{
	Board_Init();
	SystemCoreClockUpdate();
	//SysTick_Config(SystemCoreClock / 1000);
}

static void pausems(uint32_t t)
{
	pausems_count = t;
	while (pausems_count != 0) {
		__WFI();
	}
}

/*==================[external functions definition]==========================*/

/*void SysTick_Handler(void)
{
	if(pausems_count > 0) pausems_count--;
}*/

volatile uint32_t * DWT_CTRL   = (uint32_t *)0xE0001000;
volatile uint32_t * DWT_CYCCNT = (uint32_t *)0xE0001004;

#define ANCHO_IMAGEN 64
#define ALTO_IMAGEN  48
#define MAX_BRILLO 	 255

int main(void)
{
	/**
	 *  La imagen se representa como una matriz donde cada elemento es un pixel.
	 *  Cada pixel se representa por 1 byte, es decir, 256 colores posibles por pixel
	 */
	uint8_t imagen_c[ANCHO_IMAGEN][ALTO_IMAGEN], imagen_asm[ANCHO_IMAGEN][ALTO_IMAGEN];

	/**
	 * Cantidad de brillo que deseamos aplicar a la imagen.
	 * OJO, puede ser negativo también.
	 */
	int8_t brilloDeseado = 110;

	/**
	 * Auxiliar para inicializar la imagen
	 */
	int8_t valorBase = 100;

	/**
	 * Variables auxiliares
	 */
	volatile uint32_t ciclos_c=0,
			 ciclos_asm=0,
			 ciclos_simd=0,
			 i,
			 j,
			 right_asm=1;


	initHardware();

	/**
	 * Inicializamos las imagenes: fila 0 con el valorBase + 0 (100),
	 * 							fila 1 con el valorBase + 1 (101),
	 * 							fila 2 con el valorBase + 2 (102), ...
	 */
	for (i=0; i < ANCHO_IMAGEN; i++)
	{
		for(j=0; j < ALTO_IMAGEN; j++){
			imagen_c[i][j] = (valorBase + i) % MAX_BRILLO;
			imagen_asm[i][j] = (valorBase + i) % MAX_BRILLO;
		}
	}

	*DWT_CTRL  |= 1;
	//Corro la rutina en C y anoto cuanto tarda en 'ciclos_c'
	*DWT_CYCCNT = 0;
	ajustarBrillo_c ((uint8_t *)imagen_c, ANCHO_IMAGEN, ALTO_IMAGEN, brilloDeseado);
	ciclos_c=*DWT_CYCCNT;

	//Corro la rutina en asm
	*DWT_CYCCNT = 0;
	//ajustarBrillo_asm((uint8_t *)imagen_asm, ANCHO_IMAGEN, ALTO_IMAGEN, brilloDeseado);
	ciclos_asm=*DWT_CYCCNT;

	//Corro la rutina en SIMD
	*DWT_CYCCNT = 0;
	ajustarBrillo_simd((uint8_t *)imagen_asm, ANCHO_IMAGEN, ALTO_IMAGEN, brilloDeseado);
	ciclos_simd=*DWT_CYCCNT;


	//Si todo fue bien... la funcion en C da lo mismo que en asm
	//-> right_asm=1;
	for (i=0; i < ANCHO_IMAGEN; i++)
	{
		for(j=0; j < ALTO_IMAGEN; j++){

			if ( imagen_c[i][j] != imagen_asm[i][j] ){
				right_asm=0;
				break;
			}
		}
	}

	//Poner un breakpoint acá y ver como fue todo....
	while (1)
	{
		Board_LED_Toggle(LED);
		pausems(DELAY_MS);
	}
}

