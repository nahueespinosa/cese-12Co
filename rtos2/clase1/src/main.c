/*=============================================================================
 * Author: Nahuel Espinosa <nahue.espinosa@gmail.com>
 * Date: 2020/10/25
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "sapi.h"
#include "tareas.h"

#include "FreeRTOS.h"
#include "task.h"

/*=====[Definition macros of private constants]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Prototypes (declarations) of private functions]======================*/

/*=====[Implementation of public functions]==================================*/

int main( void )
{
   boardConfig();

   tareasInit();

   vTaskStartScheduler();

   // ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE )
   {
      // Si cae en este while 1 significa que no pudo iniciar el scheduler
   }

   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
   // directamente sobre un microcontrolador y no es llamado por ningún
   // Sistema Operativo, como en el caso de un programa para PC.
   return TRUE;
}
