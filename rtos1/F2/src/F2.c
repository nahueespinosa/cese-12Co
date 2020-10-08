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

/*=====[Definition & macros of public constants]==============================*/

/*=====[Definitions of extern global functions]==============================*/

// Prototipo de funcion de la tarea
void task_led( void* taskParmPtr );

/*=====[Definitions of public global variables]==============================*/

const gpioMap_t leds[] = { [TEC1_INDEX]=LEDB, [TEC2_INDEX]=LED1, [TEC3_INDEX]=LED2, [TEC4_INDEX]=LED3};
#define leds_count   sizeof(leds)/sizeof(leds[0])

/*=====[Main function, program entry point after power on or reset]==========*/

int main( void )
{
   uint32_t i;
   BaseType_t res;

   // ---------- CONFIGURACIONES ------------------------------
   boardConfig();  // Inicializar y configurar la plataforma

   printf( "Ejercicio F2\n" );

   for( i = 0 ; i < leds_count ; i++ ) {
      // Crear tareas en freeRTOS
      res = xTaskCreate (
         task_led,					// Funcion de la tarea a ejecutar
         ( const char * )"task_led",	// Nombre de la tarea como String amigable para el usuario
         configMINIMAL_STACK_SIZE*2,	// Cantidad de stack de la tarea
         (void*)i,							// Parametros de tarea
         tskIDLE_PRIORITY+1,			// Prioridad de la tarea
         0							// Puntero a la tarea creada en el sistema
      );

      // Gestión de errores
      configASSERT( res == pdPASS );
   }

   /* inicializo driver de teclas */
   keys_Init();

   // Iniciar scheduler
   vTaskStartScheduler();					// Enciende tick | Crea idle y pone en ready | Evalua las tareas creadas | Prioridad mas alta pasa a running

   /* realizar un assert con "false" es equivalente al while(1) */
   configASSERT( 0 );
   return 0;
}

void task_led( void* taskParmPtr )
{
    uint32_t index = (uint32_t) taskParmPtr;
    TickType_t dif =   pdMS_TO_TICKS( 500 );
    TickType_t xPeriodicity = pdMS_TO_TICKS( 1000 ); // Tarea periodica cada 1000 ms

    TickType_t xLastWakeTime = xTaskGetTickCount();

    while( 1 )
    {
        if( key_pressed( index ) )
        {
            dif = get_diff( index );
        }

        gpioWrite( leds[index], ON );
        vTaskDelay( dif );
        gpioWrite( leds[index], OFF );

        // Envia la tarea al estado bloqueado durante xPeriodicity (delay periodico)
        vTaskDelayUntil( &xLastWakeTime, 2*dif );
    }
}

/* hook que se ejecuta si al necesitar un objeto dinamico, no hay memoria disponible */
void vApplicationMallocFailedHook()
{
    printf( "Malloc Failed Hook!\n" );
    configASSERT( 0 );
}
