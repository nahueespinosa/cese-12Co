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

#ifndef _LEDS_H_
#define _LEDS_H_

#include "FreeRTOS.h"
#include "semphr.h"
#include "sapi.h"

/* public macros ================================================================= */
#define  LED_PERIODICITY_MS   20
#define  LED_PERIODICITY      pdMS_TO_TICKS(LED_PERIODICITY_MS)

#define  LED_STARTING_DUTY_MS 1
#define  LED_STARTING_DUTY    pdMS_TO_TICKS(LED_STARTING_DUTY_MS)

// Cantidad de leds utilizados
#define leds_count   sizeof(leds)/sizeof(leds[0])

/* types ================================================================= */
typedef struct {
   gpioMap_t led;                   // led a controlar
   uint32_t downKeyIndex;           // indice de la tecla que decrementa el brillo
   uint32_t upKeyIndex;             // indice de la tecla que incrementa el brillo
   QueueHandle_t new_time_queue;    // cola de eventos para recibir nuevos tiempos por puerto serie
} ledControl_t;

/* methods ================================================================= */
void leds_Init( void );
void leds_sendTime( uint32_t ledIndex, TickType_t time );

#endif /* PDM_ANTIRREBOTE_MEF_INC_DEBOUNCE_H_ */
