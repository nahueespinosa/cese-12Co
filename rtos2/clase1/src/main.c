/*=============================================================================
 * Author: Nahuel Espinosa <nahue.espinosa@gmail.com>
 * Date: 2020/09/19
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "sapi.h"
#include "FreeRTOS.h"
#include "task.h"

/*=====[Definition macros of private constants]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Prototypes (declarations) of private functions]======================*/
void tarea_A(void* pvParameters);

/*=====[Implementation of public functions]==================================*/

int main( void )
{
   BaseType_t res;

   boardConfig();

   res = xTaskCreate(tarea_A,          // Funcion de la tarea a ejecutar
         (const char *) "Tarea A",     // Nombre de la tarea como String amigable para el usuario
         configMINIMAL_STACK_SIZE * 2,    // Cantidad de stack de la tarea
         0,                               // Parametros de tarea
         tskIDLE_PRIORITY + 1,            // Prioridad de la tarea
         0                                // Puntero a la tarea creada en el sistema
         );

   configASSERT(res == pdPASS);

   vTaskStartScheduler();

   // ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE )
   {
      // Si cae en este while 1 significa que no pudo iniciar el scheduler
   }

   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
   // directamenteno sobre un microcontroladore y no es llamado por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return TRUE;
}

/*=====[Implementation of private functions]=================================*/
void tarea_A(void* pvParameters) {


   while ( TRUE) {
      gpioToggle(LEDB);

   }
}


