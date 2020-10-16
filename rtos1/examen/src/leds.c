/* Copyright 2020, Nahuel Espinosa
 * All rights reserved.
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
#include "leds.h"

/*=====[ Definitions of private data types ]===================================*/

#define  NEW_TIME_QUEUE_SIZE  4

/*=====[Definition macros of private constants]==============================*/

/*=====[Prototypes (declarations) of private functions]======================*/

/*=====[Definitions of private global variables]=============================*/

static ledControl_t leds[] = {
   { LED1, TEC1_INDEX, TEC2_INDEX, NULL },
   { LED3, TEC3_INDEX, TEC4_INDEX, NULL }
};

/*=====[Definitions of public global variables]==============================*/

/*=====[prototype of private functions]=================================*/

void task_led(void* taskParmPtr);

/*=====[Implementations of public functions]=================================*/

void leds_Init( void ) {

   uint32_t i;
   BaseType_t res;

   // Crea una tarea por cada led a controlar
   for (i = 0; i < leds_count; i++) {
      res = xTaskCreate(task_led,         // Funcion de la tarea a ejecutar
            (const char *) "task_led",    // Nombre de la tarea como String amigable para el usuario
            configMINIMAL_STACK_SIZE * 2, // Cantidad de stack de la tarea
            (void*) i,                    // Parametros de tarea
            tskIDLE_PRIORITY + 1,         // Prioridad de la tarea
            0                             // Puntero a la tarea creada en el sistema
            );

      configASSERT(res == pdPASS);

      leds[i].new_time_queue = xQueueCreate(NEW_TIME_QUEUE_SIZE, sizeof(TickType_t));
      configASSERT(leds[i].new_time_queue != NULL);
   }
}

void leds_sendTime( uint32_t ledIndex, TickType_t time ) {

   // Si el indice o el tiempo son incorrectos, no envía el mensaje
   if( ledIndex < leds_count && time <= LED_PERIODICITY ) {
      xQueueSend(leds[ledIndex].new_time_queue, &time, portMAX_DELAY);
   }
}

void task_led(void* taskParmPtr) {
   uint32_t index = (uint32_t) taskParmPtr;
   TickType_t time_on = LED_STARTING_DUTY;

   TickType_t xLastWakeTime = xTaskGetTickCount();

   while (1) {

      // Esta tecla decrementa el brillo del led en 1 ms
      if (key_pressed(leds[index].downKeyIndex) && time_on > 0) {
         time_on -= pdMS_TO_TICKS(1);
      }

      // Esta tecla incrementa el brillo del led en 1 ms
      if (key_pressed(leds[index].upKeyIndex) && time_on < LED_PERIODICITY) {
         time_on += pdMS_TO_TICKS(1);
      }

      // Si se recibe un nuevo tiempo a través de un comando, se actualiza el valor
      xQueueReceive(leds[index].new_time_queue, &time_on, 0);

      if (time_on > 0) {
         gpioWrite(leds[index].led, ON);
         vTaskDelay(time_on);
         gpioWrite(leds[index].led, OFF);
      }

      vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(LED_PERIODICITY));
   }
}
