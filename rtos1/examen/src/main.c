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
#include "main.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "sapi.h"

#include "semphr.h"
#include "protocol.h"
#include "keys.h"
#include "leds.h"

void wait_frame(void* pvParameters);

int main(void) {

   BaseType_t res;

   boardConfig();

   procotol_x_init(UART_USB, BAUDRATE);

   // Crea una tarea para procesar los comandos recibidos y enviarlos a la tarea de led correspondiente
   res = xTaskCreate(wait_frame,          // Funcion de la tarea a ejecutar
         (const char *) "wait_frame",     // Nombre de la tarea como String amigable para el usuario
         configMINIMAL_STACK_SIZE * 2,    // Cantidad de stack de la tarea
         0,                               // Parametros de tarea
         tskIDLE_PRIORITY + 1,            // Prioridad de la tarea
         0                                // Puntero a la tarea creada en el sistema
         );

   configASSERT(res == pdPASS);

   leds_Init();      // Crea las tareas que controlan los leds
   keys_Init();      // Crea las tareas que gestionan las teclas

   vTaskStartScheduler();

   return 0;
}

// Esta tarea procesa los comandos recibidos por la UART y los envía al módulo de control de leds
void wait_frame(void* pvParameters) {
   char* data;
   uint16_t size;

   uint16_t frame_counter = 0;

   uint32_t index;
   TickType_t new_time;

   while ( TRUE) {
      protocol_wait_frame();

      protocol_get_frame_ref(&data, &size);

      // Una trama válida tiene cuatro caracteres "[XY]"
      if( size == 4 ) {

         // "X" es data[1], indica el índice del led a controlar
         index = data[1] - '0';

         // "Y" es data[2], indica el brillo
         if (data[2] <= '9' && data[2] >= '0') {

            // Del '0' al '9' varia entre el 0% y el 90% del periodo
            new_time = (data[2] - '0') * pdMS_TO_TICKS(LED_PERIODICITY) / 10;
            leds_sendTime( index, new_time );

         } else if (data[2] == 'A') {

            // La 'A' indica el 100% del periodo
            new_time = pdMS_TO_TICKS(LED_PERIODICITY);
            leds_sendTime( index, new_time );

         } else {

            // Si no es es ninguno de los anteriores, es una trama incorrecta y se descarta
         }

      }

      protocol_discard_frame();
      frame_counter++;
   }
}
