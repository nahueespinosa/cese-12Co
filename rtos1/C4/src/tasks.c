/*=============================================================================
 * Copyright (c) 2020, Martin N. Menendez <menendezmartin81@gmail.com>
 * All rights reserved.
 * License: Free
 * Date: 2020/09/03
 * Version: v1.1
 *===========================================================================*/

/*==================[inclusiones]============================================*/

#include "tasks.h"

/*==================[definiciones y macros]==================================*/

/*==================[definiciones de datos internos]=========================*/

/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/

/*==================[tareas]=================================================*/

void tarea_tecla( void* taskParmPtr )
{
   tLedTecla* config = (tLedTecla*) taskParmPtr;

   fsmButtonInit( config );

   while( TRUE )
   {
      fsmButtonUpdate( config );
      vTaskDelay( BUTTON_RATE );
   }
}

void tarea_led( void* taskParmPtr )
{
   // ---------- CONFIGURACIONES ------------------------------
   tLedTecla* config = (tLedTecla*) taskParmPtr;
   TickType_t xLastWakeTime = xTaskGetTickCount();

   // ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE )
   {
      if( xSemaphoreTake( config->sem_tec_pulsada, 0 ) == pdTRUE )
      {
         gpioWrite( LEDG , ON );
         vTaskDelay( LED_ON_TIME );
         gpioWrite( LEDG , OFF );
      }
      else
      {
         gpioWrite( LEDR , ON );
         vTaskDelay( LED_ON_TIME );
         gpioWrite( LEDR , OFF );
      }

      vTaskDelayUntil( &xLastWakeTime, SEM_WAIT_RATE );
   }
}
