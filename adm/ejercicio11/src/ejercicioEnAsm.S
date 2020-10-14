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
    .global corrInAsm


#define vectorX     r0
#define vectorY     r1
#define vectorCorr  r2
#define longitud    r3
#define l           r4
#define i           r5
#define j           r6
#define acumulador  r7
#define valorX      r8
#define valorY      r9
#define aux         r10

    /**
     * Indicamos que la siguiente subrutina debe ser ensamblada en modo thumb,
     * entonces en las direcciones en el ultimo bit tendran el 1 para que se reconozcan como en modo thumb.
     * Siempre hay que ponerla antes de la primer instruccion.
     */
    .thumb_func


corrInAsm:
    push    {r4-r10,lr}

    mov     l, #0

loop_l:
    mov     i, #0
    mov     acumulador, #0
    subs    j, longitud, l

loop_n:
    udiv    aux, j, longitud
    mls     j, aux, longitud, j

    ldrsh   valorX, [vectorX, i, LSL 1]
    ldrsh   valorY, [vectorY, j, LSL 1]

    mla     acumulador, valorX, valorY, acumulador

    add     j, 1
    add     i, 1
    cmp     i, longitud
    bne     loop_n

    strh    acumulador, [vectorCorr, l, LSL 1]

    add     l, 1
    cmp     l, longitud
    bne     loop_l

    pop     {r4-r10,pc}   /* retorno */
