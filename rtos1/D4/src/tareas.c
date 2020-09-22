/*=============================================================================
 * Copyright (c) 2020, Nahuel Espinosa
 * All rights reserved.
 * License: Free
 * Date: 2020/09/03
 * Version: v1.1
 *===========================================================================*/

/*==================[inclusiones]============================================*/

#include "tareas.h"
#include "sapi.h"
#include "keys.h"

#include "task.h"
#include "semphr.h"

/*==================[definiciones y macros]==================================*/

#define DEBOUNCE_TIME         40

/*==================[definiciones de datos internos]=========================*/

TickType_t c1;
SemaphoreHandle_t c1_mutex;

/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/

void tasks_init( void ) {
   BaseType_t res;

   /* inicializo driver de teclas */
   keys_Init();

   c1_mutex = xSemaphoreCreateMutex();
   c1 = C1_INITIAL_VALUE;

   configASSERT( c1_mutex != NULL );

   xSemaphoreGive( c1_mutex );

   // Crear tareas en freeRTOS
   res = xTaskCreate (
      task_teclas,               // Funcion de la tarea a ejecutar
      ( const char * )"task_teclas",   // Nombre de la tarea como String amigable para el usuario
      configMINIMAL_STACK_SIZE*2,   // Cantidad de stack de la tarea
      0,                   // Parametros de tarea
      tskIDLE_PRIORITY+1,        // Prioridad de la tarea
      0                    // Puntero a la tarea creada en el sistema
   );

   // Gestión de errores
   configASSERT( res == pdPASS );

   // Crear tareas en freeRTOS
   res = xTaskCreate (
      task_led_1,             // Funcion de la tarea a ejecutar
      ( const char * )"task_led_1", // Nombre de la tarea como String amigable para el usuario
      configMINIMAL_STACK_SIZE*2,   // Cantidad de stack de la tarea
      0,                   // Parametros de tarea
      tskIDLE_PRIORITY+1,        // Prioridad de la tarea
      0                    // Puntero a la tarea creada en el sistema
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
}

/*==================[tareas]=================================================*/

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

