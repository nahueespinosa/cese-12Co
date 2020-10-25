# RTOS 2 - Clase 1 - Ejercitaci�n introductoria

## Parte 1:

Implemente un sistema que tenga las siguientes tareas y use los siguientes recursos:

- Recurso 1: Una cola `cola_1` cuyos elementos sean referencias de memoria.
- Tarea A: Encender peri�dicamente un LED. Cada vez que el LED encienda, se deber� enviar un mensaje "LED_ON" a la `cola_1`.
- Tarea B: Lectura con antirrebote de 2 teclas y medici�n de tiempo de pulsaci�n (utilizar lectura de teclas por polling). Al medirse el tiempo de pulsaci�n, enviar el mensaje "TECx Tyyyy" a la `cola_1` donde *x* es el �ndice de tecla e *yyyy* la cantidad de ms que fue pulsada.
- Tarea C: Deber� obtener de `cola_1` mensajes de texto, y enviarlos por la uart.

Se debe utilizar memoria din�mica para alocar los espacios de memoria.

## Parte 2:

Esta parte del ejercicio involucra la modularizaci�n del mecanismo implementado en la parte uno de forma tal que las tareas (el "usuario") no conozcan c�mo est� implementada sino que conozca una API que la utilice.

De esta modularizaci�n se obtendr� una librer�a que permitir� intercambiar mensajes de tama�o variable entre varias tareas productoras y una tarea consumidora de datos. El contenido de cada mensaje puede ser cualquier tipo de dato.

Para probar la librer�a, implemente el mismo sistema de 3 tareas de la parte 1 modificando lo siguiente:

- El recurso utilizado no ser� m�s `cola_1` sino `obj_1` y `obj_2` que ser�n instancias del objeto gestionado por la librer�a.
- `obj 1` se utilizar� tal cual la parte 1 (en reemplazo de `cola_1`).
- Cada vez que la tarea C, env�e el texto por puerto serie deber� adem�s, enviar dicho texto a la tarea D a trav�s del `obj_2`.
- Defina una tarea D que consuma las cadenas de texto (que simplemente la descarte).

