/*=============================================================================
 * Copyright (c) 2020, Martin N. Menendez <menendezmartin81@gmail.com>
 * All rights reserved.
 * License: Free
 * Date: 2020/09/03
 * Version: v1.1
 *===========================================================================*/

#ifndef _TASKS_H_
#define _TASKS_H_

/*==================[inclusiones]============================================*/

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "sapi.h"
#include "semphr.h"
#include "auxs.h"
#include "fsm_debounce.h"

/*==================[definiciones y macros]==================================*/

#define BUTTON_RATE_MS     1
#define SEM_WAIT_RATE_MS   1000
#define LED_ON_TIME_MS     250

#define BUTTON_RATE        pdMS_TO_TICKS(BUTTON_RATE_MS)
#define SEM_WAIT_RATE      pdMS_TO_TICKS(SEM_WAIT_RATE_MS)
#define LED_ON_TIME        pdMS_TO_TICKS(LED_ON_TIME_MS)

/*==================[definiciones de datos]=========================*/

extern tLedTecla tecla_led_config[];

/*==================[prototipos de tareas]====================*/

void tarea_led( void* taskParmPtr );
void tarea_tecla( void* taskParmPtr );

#endif /* _Tasks_H_ */
