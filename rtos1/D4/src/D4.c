/* Copyright 2020, Franco Bucafusco
 * All rights reserved.
 *
 * This file is part of sAPI Library.
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
 */

/*=====[Inclusions of function dependencies]=================================*/

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

#include "sapi.h"
#include "keys.h"

#define  LED_2_PERIODICITY_MS    2000
#define  LED_2_PERIODICITY       pdMS_TO_TICKS(LED_2_PERIODICITY_MS)

void task_led_1( void* taskParmPtr );
void task_led_2( void* taskParmPtr );

/*=====[Main function, program entry point after power on or reset]==========*/

int main( void )
{
	BaseType_t res;

	// ---------- CONFIGURACIONES ------------------------------
	boardConfig();  // Inicializar y configurar la plataforma

	printf( "Ejercicio D4\n" );

	// Crear tareas en freeRTOS
	res = xTaskCreate (
      task_led_1,					// Funcion de la tarea a ejecutar
      ( const char * )"task_led_1",	// Nombre de la tarea como String amigable para el usuario
      configMINIMAL_STACK_SIZE*2,	// Cantidad de stack de la tarea
      0,							// Parametros de tarea
      tskIDLE_PRIORITY+1,			// Prioridad de la tarea
      0							// Puntero a la tarea creada en el sistema
   );

	// Gestión de errores
	configASSERT( res == pdPASS );

	// Crear tareas en freeRTOS
   res = xTaskCreate (
      task_led_2,             // Funcion de la tarea a ejecutar
      ( const char * )"task_led_2", // Nombre de la tarea como String amigable para el usuario
      configMINIMAL_STACK_SIZE*2, // Cantidad de stack de la tarea
      0,                    // Parametros de tarea
      tskIDLE_PRIORITY+1,         // Prioridad de la tarea
      0                     // Puntero a la tarea creada en el sistema
   );

   // Gestión de errores
   configASSERT( res == pdPASS );

	/* inicializo driver de teclas */
	keys_Init();

	// Iniciar scheduler
	vTaskStartScheduler();					// Enciende tick | Crea idle y pone en ready | Evalua las tareas creadas | Prioridad mas alta pasa a running

	/* realizar un assert con "false" es equivalente al while(1) */
	configASSERT( 0 );
	return 0;
}

void task_led_1( void* taskParmPtr )
{
   TickType_t xTime;

	while( 1 )
	{
	   xSemaphoreTake( c1_mutex, portMAX_DELAY );
      xTime = c1;
      xSemaphoreGive( c1_mutex );

      gpioToggle( LED1 );
      vTaskDelay( xTime );
	}
}

void task_led_2( void* taskParmPtr )
{
   TickType_t xTime;
   TickType_t xLastWakeTime = xTaskGetTickCount();

   while( 1 )
   {
      xSemaphoreTake( c1_mutex, portMAX_DELAY );
      xTime = c1 * 2;
      xSemaphoreGive( c1_mutex );

      gpioWrite( LED2, ON );
      vTaskDelay( xTime );
      gpioWrite( LED2, OFF );
      vTaskDelayUntil( &xLastWakeTime, LED_2_PERIODICITY );
   }
}

/* hook que se ejecuta si al necesitar un objeto dinamico, no hay memoria disponible */
void vApplicationMallocFailedHook()
{
	printf( "Malloc Failed Hook!\n" );
	configASSERT( 0 );
}
