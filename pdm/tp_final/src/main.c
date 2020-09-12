/*=============================================================================
 * Author: Nahuel Espinosa <nahue.espinosa@gmail.com>
 * Date: 2020/08/11
 *===========================================================================*/

// The maximum number of tasks required at any one time during the execution
// of the program. MUST BE ADJUSTED FOR EACH NEW PROJECT
#define SCHEDULER_MAX_TASKS   (3)

/*=====[Inclusions of function dependencies]=================================*/

#include "../../tp_final/inc/main.h"

#include "sapi.h"
#include "../../tp_final/inc/fsm_salt.h"

#include "seos_pont_2014_isr.h"       // <= dispatcher and task management header
#include "seos_pont_2014_scheduler.h" // <= scheduler and system initialization header

/*=====[Main function, program entry point after power on or reset]==========*/

int main(void)
{
   // ----- Setup -----------------------------------
   boardInit();
   saltInit();

   // Función que inicializa el planificador de tareas
   schedulerInit();

   // Se agregan las tareas al planificador
   schedulerAddTask( (callBackFuncPtr_t)saltUpdate  , NULL, 0, SALT_UPDATE_TIME   );
   schedulerAddTask( (callBackFuncPtr_t)relayUpdate , NULL, 1, RELAY_UPDATE_TIME  );
   schedulerAddTask( (callBackFuncPtr_t)switchUpdate, NULL, 2, SWITCH_UPDATE_TIME );

   // FUNCION que inicializa la interrupcion que ejecuta el planificador de
   // tareas con tick cada 1ms.
   schedulerStart( 1 );

   // ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE )
   {
      // Se despachan (ejecutan) las tareas marcadas para su ejecucion.
      // Luego se pone el sistema en bajo consumo hasta que ocurra la
      // proxima interrupcion, en este caso la de Tick.
      // Al ocurrir la interrupcion de Tick se ejecutara el planificador
      // que revisa cuales son las tareas a marcar para su ejecucion.
      schedulerDispatchTasks();
   }

   return 0;
}




