/*=============================================================================
 * Copyright (c) 2020, Martin N. Menendez <menendezmartin81@gmail.com>
 * All rights reserved.
 * License: Free
 * Date: 2020/09/03
 * Version: v1.1
 *===========================================================================*/

/*==================[inclusiones]============================================*/

#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"

#include "sapi.h"
#include "tipos.h"

/*==================[definiciones y macros]==================================*/

#define RATE 40
#define LED_RATE pdMS_TO_TICKS(RATE)
#define BUTTON_RATE 1

/*==================[definiciones de datos internos]=========================*/
tLedTecla config;

/*==================[definiciones de datos externos]=========================*/

DEBUG_PRINT_ENABLE;

/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/

// Prototipo de funcion de la tarea
void tarea_led( void* taskParmPtr );
void tarea_tecla( void* taskParmPtr );

/*==================[funcion principal]======================================*/

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void )
{
    // ---------- CONFIGURACIONES ------------------------------
	boardConfig();									// Inicializar y configurar la plataforma

	debugPrintConfigUart( UART_USB, 115200 );		// UART for debug messages
	printf( "Ejercicio B_7.\r\n" );

	//gpioWrite( LED3, ON );							// Led para dar se�al de vida

    // Crear tarea en freeRTOS
    BaseType_t res_A =
    xTaskCreate(
        tarea_led,                     // Funcion de la tarea a ejecutar
        ( const char * )"tarea_led",   // Nombre de la tarea como String amigable para el usuario
        configMINIMAL_STACK_SIZE*2, // Cantidad de stack de la tarea
        0,                          // Parametros de tarea
        tskIDLE_PRIORITY+1,         // Prioridad de la tarea
        0                           // Puntero a la tarea creada en el sistema
    );

    BaseType_t res_B =
    xTaskCreate(
    	tarea_tecla,                     // Funcion de la tarea a ejecutar
        ( const char * )"tarea_tecla",   // Nombre de la tarea como String amigable para el usuario
        configMINIMAL_STACK_SIZE*2, 	// Cantidad de stack de la tarea
        0,                          	// Parametros de tarea
        tskIDLE_PRIORITY+1,         	// Prioridad de la tarea
        0                           	// Puntero a la tarea creada en el sistema
    );

    // Gestion de errores
	if((res_A || res_B) == pdFAIL)
	{
		gpioWrite( LEDR, ON );
		printf( "Error al crear las tareas.\r\n" );
		while(TRUE);						// VER ESTE LINK: https://pbs.twimg.com/media/BafQje7CcAAN5en.jpg
	}

    // Iniciar scheduler
    vTaskStartScheduler();					// Enciende tick | Crea idle y pone en ready | Evalua las tareas creadas | Prioridad mas alta pasa a running

    // ---------- REPETIR POR SIEMPRE --------------------------
    while( TRUE )
    {
        // Si cae en este while 1 significa que no pudo iniciar el scheduler
    }

    // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
    // directamenteno sobre un microcontroladore y no es llamado por ningun
    // Sistema Operativo, como en el caso de un programa para PC.
    return 0;
}

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/

void tarea_tecla( void* taskParmPtr )
{	
   config.led = LEDB;
   config.tecla = TEC1;
   fsmButtonInit( &config );
    
	while( TRUE )
	{
		fsmButtonUpdate( &config );
	 	vTaskDelay( BUTTON_RATE / portTICK_RATE_MS );
	 	//vTaskDelay ( pdMS_TO_TICKS(BUTTON_RATE) );
	}
}

// Implementacion de funcion de la tarea
void tarea_led( void* taskParmPtr )
{
    TickType_t tiempo_inicio_ciclo;

    // ---------- CONFIGURACIONES ------------------------------
    tiempo_inicio_ciclo = xTaskGetTickCount();

    // ---------- REPETIR POR SIEMPRE --------------------------
    while( TRUE )
    {
         gpioWrite( config.led , ON );
         vTaskDelay( config.tiempo_medido );
         gpioWrite( config.led , OFF );
         vTaskDelayUntil( &tiempo_inicio_ciclo, 1000 / portTICK_RATE_MS );
    }
}

/*==================[fin del archivo]========================================*/
