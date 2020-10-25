# RTOS 2 - Clase 1 - Ejercitación introductoria

## Parte 1:

Implemente un sistema que tenga las siguientes tareas y use los siguientes recursos:

- Recurso 1: Una cola `cola_1` cuyos elementos sean referencias de memoria.
- Tarea A: Encender periódicamente un LED. Cada vez que el LED encienda, se deberá enviar un mensaje "LED_ON" a la `cola_1`.
- Tarea B: Lectura con antirrebote de 2 teclas y medición de tiempo de pulsación (utilizar lectura de teclas por polling). Al medirse el tiempo de pulsación, enviar el mensaje "TECx Tyyyy" a la `cola_1` donde *x* es el índice de tecla e *yyyy* la cantidad de ms que fue pulsada.
- Tarea C: Deberá obtener de `cola_1` mensajes de texto, y enviarlos por la uart.

Se debe utilizar memoria dinámica para alocar los espacios de memoria.

## Parte 2:

Esta parte del ejercicio involucra la modularización del mecanismo implementado en la parte uno de forma tal que las tareas (el "usuario") no conozcan cómo está implementada sino que conozca una API que la utilice.

De esta modularización se obtendrá una librería que permitirá intercambiar mensajes de tamaño variable entre varias tareas productoras y una tarea consumidora de datos. El contenido de cada mensaje puede ser cualquier tipo de dato.

Para probar la librería, implemente el mismo sistema de 3 tareas de la parte 1 modificando lo siguiente:

- El recurso utilizado no será más `cola_1` sino `obj_1` y `obj_2` que serán instancias del objeto gestionado por la librería.
- `obj 1` se utilizará tal cual la parte 1 (en reemplazo de `cola_1`).
- Cada vez que la tarea C, envíe el texto por puerto serie deberá además, enviar dicho texto a la tarea D a través del `obj_2`.
- Defina una tarea D que consuma las cadenas de texto (que simplemente la descarte).

