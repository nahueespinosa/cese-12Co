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

/*==================[ Inclusions ]============================================*/
#include "FreeRTOS.h"
#include "task.h"
#include "sapi.h"
#include "keys.h"

/*=====[ Definitions of private data types ]===================================*/


/*=====[Definition macros of private constants]==============================*/
#define DEBOUNCE_TIME         40

#define C1_INITIAL_VALUE_MS   500
#define C1_MIN_VALUE_MS       100
#define C1_MAX_VALUE_MS       1900

#define C1_INITIAL_VALUE      pdMS_TO_TICKS(C1_INITIAL_VALUE_MS)
#define C1_MIN_VALUE          pdMS_TO_TICKS(C1_MIN_VALUE_MS)
#define C1_MAX_VALUE          pdMS_TO_TICKS(C1_MAX_VALUE_MS)

/*=====[Prototypes (declarations) of private functions]======================*/

static void keys_ButtonError( keyMap_t index );
static void buttonPressed( keyMap_t index );
static void buttonReleased( keyMap_t index );

/*=====[Definitions of private global variables]=============================*/

TickType_t c1;
SemaphoreHandle_t c1_mutex;

/*=====[Definitions of public global variables]==============================*/

static const t_key_config  keys_config[] = { TEC1, TEC2 } ;

#define key_count   sizeof(keys_config)/sizeof(keys_config[0])

t_key_data keys_data[key_count];

/*=====[prototype of private functions]=================================*/
void task_teclas( void* taskParmPtr );

/*=====[Implementations of public functions]=================================*/
TickType_t get_diff( keyMap_t index )
{
	TickType_t tiempo;

	taskENTER_CRITICAL();
	tiempo = keys_data[index].time_diff;
	taskEXIT_CRITICAL();

	return tiempo;
}

void clear_diff( keyMap_t index )
{
	taskENTER_CRITICAL();
	keys_data[index].time_diff = KEYS_INVALID_TIME;
	taskEXIT_CRITICAL();
}

void keys_Init( void )
{
	BaseType_t res;

	keys_data[KEY1].state          = BUTTON_UP;  // Set initial state
	keys_data[KEY1].time_down      = KEYS_INVALID_TIME;
	keys_data[KEY1].time_up        = KEYS_INVALID_TIME;
	keys_data[KEY1].time_diff      = KEYS_INVALID_TIME;

	keys_data[KEY2].state          = BUTTON_UP;  // Set initial state
   keys_data[KEY2].time_down      = KEYS_INVALID_TIME;
   keys_data[KEY2].time_up        = KEYS_INVALID_TIME;
   keys_data[KEY2].time_diff      = KEYS_INVALID_TIME;

	// Crear tareas en freeRTOS
	res = xTaskCreate (
      task_teclas,					// Funcion de la tarea a ejecutar
      ( const char * )"task_teclas",	// Nombre de la tarea como String amigable para el usuario
      configMINIMAL_STACK_SIZE*2,	// Cantidad de stack de la tarea
      0,							// Parametros de tarea
      tskIDLE_PRIORITY+1,			// Prioridad de la tarea
      0							// Puntero a la tarea creada en el sistema
   );

	c1_mutex = xSemaphoreCreateMutex();
	c1 = C1_INITIAL_VALUE;
	xSemaphoreGive( c1_mutex );

	// Gestión de errores
	configASSERT( res == pdPASS );
}

// keys_ Update State Function
void keys_Update( keyMap_t index )
{
	switch( keys_data[index].state )
	{
		case STATE_BUTTON_UP:
			/* CHECK TRANSITION CONDITIONS */
			if( !gpioRead( keys_config[index].tecla ) )
			{
				keys_data[index].state = STATE_BUTTON_FALLING;
			}
			break;

		case STATE_BUTTON_FALLING:
			/* ENTRY */

			/* CHECK TRANSITION CONDITIONS */
			if( !gpioRead( keys_config[index].tecla ) )
			{
				keys_data[index].state = STATE_BUTTON_DOWN;

				/* ACCION DEL EVENTO !*/
				buttonPressed( index );
			}
			else
			{
				keys_data[index].state = STATE_BUTTON_UP;
			}

			/* LEAVE */
			break;

		case STATE_BUTTON_DOWN:
			/* CHECK TRANSITION CONDITIONS */
			if( gpioRead( keys_config[index].tecla ) )
			{
				keys_data[index].state = STATE_BUTTON_RISING;
			}
			break;

		case STATE_BUTTON_RISING:
			/* ENTRY */

			/* CHECK TRANSITION CONDITIONS */

			if( gpioRead( keys_config[index].tecla ) )
			{
				keys_data[index].state = STATE_BUTTON_UP;

				/* ACCION DEL EVENTO ! */
				buttonReleased( index );
			}
			else
			{
				keys_data[index].state = STATE_BUTTON_DOWN;
			}

			/* LEAVE */
			break;

		default:
			keys_ButtonError( index );
			break;
	}
}

/*=====[Implementations of private functions]================================*/

/* accion de el evento de tecla pulsada */
static void buttonPressed( keyMap_t index )
{
	TickType_t current_tick_count = xTaskGetTickCount();

	taskENTER_CRITICAL();
	keys_data[index].time_down = current_tick_count;
	taskEXIT_CRITICAL();
}

/* accion de el evento de tecla liberada */
static void buttonReleased( keyMap_t index )
{
	TickType_t current_tick_count = xTaskGetTickCount();

	taskENTER_CRITICAL();
	keys_data[index].time_up    = current_tick_count;
	keys_data[index].time_diff  = keys_data[index].time_up - keys_data[index].time_down;
	taskEXIT_CRITICAL();
}

static void keys_ButtonError( keyMap_t index )
{
	taskENTER_CRITICAL();
	keys_data[index].state = BUTTON_UP;
	taskEXIT_CRITICAL();
}

/*=====[Implementations of private functions]=================================*/
void task_teclas( void* taskParmPtr )
{
   TickType_t dif;

	while( 1 )
	{
	   // Actualiza estado de la tecla 1
		keys_Update( KEY1 );

		dif = get_diff( KEY1 );

      if( dif != KEYS_INVALID_TIME ) {
         xSemaphoreTake(c1_mutex, portMAX_DELAY);

         if( c1 + dif > C1_MAX_VALUE ) {
            c1 = C1_MAX_VALUE;
         } else {
            c1 += dif;
         }

         xSemaphoreGive(c1_mutex);

         clear_diff( KEY1 );
      }

      // Actualiza estado de la tecla 2
      keys_Update( KEY2 );

      dif = get_diff( KEY2 );

      if( dif != KEYS_INVALID_TIME ) {
         xSemaphoreTake(c1_mutex, portMAX_DELAY);

         if( c1 < C1_MIN_VALUE + dif ) {
            c1 = C1_MIN_VALUE;
         } else {
            c1 -= dif;
         }

         xSemaphoreGive(c1_mutex);

         clear_diff( KEY2 );
      }

		vTaskDelay( DEBOUNCE_TIME / portTICK_RATE_MS );
	}
}
