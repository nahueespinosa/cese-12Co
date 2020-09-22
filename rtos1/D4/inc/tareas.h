/*=============================================================================
 * Copyright (c) 2020, Nahuel Espinosa
 * All rights reserved.
 * License: Free
 * Date: 2020/09/03
 * Version: v1.1
 *===========================================================================*/

#ifndef _TAREAS_H_
#define _TAREAS_H_

/*==================[inclusiones]============================================*/

#include "FreeRTOS.h"

/*==================[definiciones y macros]==================================*/

#define LED_2_PERIODICITY_MS  2000
#define LED_2_PERIODICITY     pdMS_TO_TICKS(LED_2_PERIODICITY_MS)

#define C1_INITIAL_VALUE_MS   500
#define C1_MIN_VALUE_MS       100
#define C1_MAX_VALUE_MS       1900

#define C1_INITIAL_VALUE      pdMS_TO_TICKS(C1_INITIAL_VALUE_MS)
#define C1_MIN_VALUE          pdMS_TO_TICKS(C1_MIN_VALUE_MS)
#define C1_MAX_VALUE          pdMS_TO_TICKS(C1_MAX_VALUE_MS)

/*==================[definiciones de datos]=========================*/

/*==================[prototipos de tareas]====================*/

void tasks_init( void );
void task_teclas( void* taskParmPtr );
void task_led_1( void* taskParmPtr );
void task_led_2( void* taskParmPtr );

#endif /* _TAREAS_H_ */
