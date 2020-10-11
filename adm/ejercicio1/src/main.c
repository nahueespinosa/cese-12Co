/* Copyright 2020, Nahuel Espinosa
 * All rights reserved.
 *
 * This file is part of arquitecturaDeMicroprocesadores.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*==================[inclusions]=============================================*/

#include "board.h"
#include <stdint.h>

/*==================[macros and definitions]=================================*/

#define MAX_SIZE 1000

/*==================[internal data declaration]==============================*/

volatile uint32_t * DWT_CTRL   = (uint32_t *)0xE0001000;
volatile uint32_t * DWT_CYCCNT = (uint32_t *)0xE0001004;

/*==================[external functions definition]==========================*/

extern void zerosInC(uint32_t *vector, uint32_t longitud);
extern void zerosInAsm(uint32_t *vector, uint32_t longitud);

/*==================[internal functions definition]==========================*/

static void initHardware(void)
{
   Board_Init();
   SystemCoreClockUpdate();
   //SysTick_Config(SystemCoreClock / 1000);
}

int main(void)
{
	volatile uint32_t cyclesC=0,cyclesAsm=0,cyclesSIMD=0;
	uint32_t resultC=0,resultAsm=0,resultSIMD=0, i;
	uint32_t vectorEnC[MAX_SIZE];
	uint32_t vectorEnAsm[MAX_SIZE];

	//Inicializamos los vectores
	//Hacemos vec[0] = 0, vec[1] = 1, etc
	for (i=0; i < MAX_SIZE; i++)
	{
		vectorEnC[i] = i;
		vectorEnAsm[i] = i;
	}

	initHardware();

   // Activamos el conteo de ciclos
	*DWT_CTRL |= 1;

	//Inicializamos en 0 el registro de cuentas de ciclos
	*DWT_CYCCNT = 0;
	//Invocamos a la funcion en C
	zerosInC(vectorEnC,MAX_SIZE);
	//Nos guardamos en una variable el valor actual del contador de ciclos
	cyclesC=*DWT_CYCCNT;

	//Inicializamos en 0 el registro de cuentas de ciclos
	*DWT_CYCCNT = 0;
	//Invocamos a la función en ASM
	zerosInAsm(vectorEnAsm,MAX_SIZE);
	//Nos guardamos en una variable el valor actual del contador de ciclos
	cyclesAsm=*DWT_CYCCNT;

	while (1)
	{
		__WFI(); //wfi
	}
}

/** @} doxygen end group definition */

/*==================[end of file]============================================*/
