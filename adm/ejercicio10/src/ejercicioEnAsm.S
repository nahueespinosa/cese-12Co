/* Copyright 2020, Nahuel Espinosa
 * All rights reserved.
 *
 * This file is part of arquitecturaDeMicroprocesadores.
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
 *
 */

    /**
     * Directiva al ensamblador que permite indicar que se encarga de buscar
     * la instruccion mas apropiada entre ARM y thumb2
     */
    .syntax unified

    /**
     * .text permite indicar una seccion de codigo.
     */
    .text

    /**
     * .global permite definir un simbolo exportable,
     * es decir que podemos verlo desde otros modulos (equivalente a extern).
     * Definimos la rutina como global para que sea visible desde otros modulos.
     */
    .global addEchoInAsm
    .global addEchoInSIMD


#define vectorIn    r0
#define vectorOut   r1
#define longitud    r2
#define indice      r3
#define indiceEcho  r4
#define valorActual r5
#define valorEcho   r6
#define echoLong    r7
#define divisor     r8
#define cero        r8

    /**
     * Indicamos que la siguiente subrutina debe ser ensamblada en modo thumb,
     * entonces en las direcciones en el ultimo bit tendran el 1 para que se reconozcan como en modo thumb.
     * Siempre hay que ponerla antes de la primer instruccion.
     */
    .thumb_func


addEchoInAsm:
    push    {r4-r8,lr}  /* guardamos la direccion de retorno en la pila */

    mov     indice, #0
    mov     indiceEcho, #0
    mov     echoLong, #882
    mov     divisor, #2

addEchoInAsm_loop_1:
    ldrsh   valorActual,[vectorIn, indice, lsl 1]
    strh    valorActual,[vectorOut, indice, lsl 1]

    add     indice, 1
    cmp     indice, echoLong
    bne     addEchoInAsm_loop_1

addEchoInAsm_loop_2:
    ldrsh   valorActual,[vectorIn, indice, lsl 1]
    ldrsh   valorEcho,[vectorIn, indiceEcho, lsl 1]
    sdiv    valorEcho, divisor
    add     valorActual, valorEcho
    ssat    valorActual, 16, valorActual
    strh    valorActual,[vectorOut, indice, lsl 1]

    add     indice, 1
    add     indiceEcho, 1

    cmp     longitud, indice
    bne     addEchoInAsm_loop_2

    pop     {r4-r8,pc}   /* retorno */


addEchoInSIMD:
    push    {r4-r8,lr}  /* guardamos la direccion de retorno en la pila */

    mov     indice, #0
    mov     indiceEcho, #0
    mov     echoLong, #882
    mov     cero, #0

    lsr     echoLong, 1

addEchoInSIMD_loop_1:
    ldr     valorActual,[vectorIn, indice, lsl 2]
    str     valorActual,[vectorOut, indice, lsl 2]

    add     indice, 1
    cmp     indice, echoLong
    bne     addEchoInSIMD_loop_1

    lsr     longitud, 1

addEchoInSIMD_loop_2:
    ldr     valorActual, [vectorIn, indice, lsl 2]
    ldr     valorEcho, [vectorIn, indiceEcho, lsl 2]
    shadd16 valorEcho, valorEcho, cero
    qadd16  valorActual, valorActual, valorEcho
    str     valorActual, [vectorOut, indice, lsl 2]

    add     indice, 1
    add     indiceEcho, 1

    cmp     longitud, indice
    bne     addEchoInSIMD_loop_2

    pop     {r4-r8,pc}   /* retorno */
