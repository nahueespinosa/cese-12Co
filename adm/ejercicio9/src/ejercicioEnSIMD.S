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
    .global invertirInSIMD


#define vector      r0
#define longitud    r1
#define i           r2
#define j           r3
#define valor1      r4
#define valor2      r5

    /**
     * Indicamos que la siguiente subrutina debe ser ensamblada en modo thumb,
     * entonces en las direcciones en el ultimo bit tendran el 1 para que se reconozcan como en modo thumb.
     * Siempre hay que ponerla antes de la primer instruccion.
     */
    .thumb_func


invertirInSIMD:
    push    {r2-r5,lr}

    lsr     longitud, #1

    mov     i, #0
    sub     j, longitud, #1

loop:
    ldr     valor1,[vector, i, lsl 2]
    ldr     valor2,[vector, j, lsl 2]

    rev     valor1, valor1
    rev16   valor1, valor1
    rev     valor2, valor2
    rev16   valor2, valor2

    str     valor1,[vector, j, lsl 2]
    str     valor2,[vector, i, lsl 2]

    sub     j, 1
    add     i, 1

    cmp     i, j
    blt     loop

    pop     {r2-r5,pc}   /* retorno */
