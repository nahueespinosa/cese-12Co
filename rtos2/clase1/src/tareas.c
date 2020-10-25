/*=============================================================================
 * Author: Nahuel Espinosa <nahue.espinosa@gmail.com>
 * Date: 2020/10/25
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "tareas.h"
#include <string.h>

#include "sapi.h"

#include "debounce.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/*=====[Definition macros of private constants]==============================*/

#define tecs_count   sizeof(tecs)/sizeof(tecs[0])

/*=====[Prototypes (declarations) of private functions]======================*/

static void tec_pressed_callback( void * );
static void tec_released_callback( void * );

/*=====[Definitions of private global variables]=============================*/

static QueueHandle_t cola_1;

static debounceButton_t tecs[] = {
   { TEC1, TEC_PERIODICITY_MS, tec_pressed_callback, tec_released_callback,
         DEBOUNCE_BUTTON_UP, 0, 0 },
   { TEC2, TEC_PERIODICITY_MS, tec_pressed_callback, tec_released_callback,
         DEBOUNCE_BUTTON_UP, 0, 0 },
};

static TickType_t tec_start[tecs_count];

/*=====[Implementation of public functions]==================================*/

/**
 * @brief   Inicialización de tareas
 */
void tareasInit( void ) {
   BaseType_t res;

   cola_1 = xQueueCreate( QUEUE_SIZE, sizeof(TickType_t) );

   configASSERT( cola_1 != NULL );

   res = xTaskCreate( tarea_A, (const char *) "Tarea A",
         configMINIMAL_STACK_SIZE * 2, 0, tskIDLE_PRIORITY + 1, 0 );

   configASSERT( res == pdPASS );

   res = xTaskCreate( tarea_B, (const char *) "Tarea B",
         configMINIMAL_STACK_SIZE * 2, 0, tskIDLE_PRIORITY + 1, 0 );

   configASSERT( res == pdPASS );

   res = xTaskCreate( tarea_C, (const char *) "Tarea C",
         configMINIMAL_STACK_SIZE * 2, 0, tskIDLE_PRIORITY + 1, 0 );

   configASSERT( res == pdPASS );
}

/**
 * @brief   Tarea A
 *
 * Enciende periódicamente un LED. Cada vez que el LED enciende, se
 * envía un mensaje "LED ON" a la "cola_1".
 */
void tarea_A( void* pvParameters ) {
   char *mensaje = NULL;

   TickType_t xLastWakeTime = xTaskGetTickCount();

   while ( TRUE ) {
      gpioToggle(LEDB);

      if( gpioRead(LEDB) == ON ) {
         mensaje = pvPortMalloc( sizeof(LED_MSG) );

         if( mensaje != NULL ) {
            strcpy( mensaje, LED_MSG );
            xQueueSend( cola_1, &mensaje, portMAX_DELAY );
         }
      }

      vTaskDelayUntil( &xLastWakeTime, LED_PERIODICITY / 2 );

   }
}

/**
 * @brief   Tarea B
 *
 * Lee con antirrebote el estado de i teclas. Mide el tiempo
 * de pulsación a través de las funciones "tec_pressed_callback"
 * y "tec_released_callback".
 *
 * Al medirse el tiempo de pulsación, envía el mensaje "TECx Tyyyy"
 * a la "cola_1" donde x es el índice e yyyy la cantidad de ms
 * que fue pulsada.
 */
void tarea_B( void* pvParameters ) {
   TickType_t xLastWakeTime = xTaskGetTickCount();

   while ( TRUE ) {

      // Actualizo el estado de cada tecla por polling
      for( uint32_t i = 0 ; i < tecs_count ; i++ ) {
         debounceButtonUpdate( &tecs[i] );
      }

      vTaskDelayUntil( &xLastWakeTime, TEC_PERIODICITY );
   }
}

/**
 * @brief   Tarea C
 *
 * Obtiene de "cola_1" mensajes de texto y los envía por la
 * UART_USB.
 */
void tarea_C( void* pvParameters ) {
   char *mensaje = NULL;

   uartConfig( UART_USB, UART_BAUDRATE );

   while ( TRUE ) {

      xQueueReceive( cola_1, &mensaje, portMAX_DELAY );

      if( mensaje != NULL ) {
         uartWriteString( UART_USB, mensaje );
         vPortFree( mensaje );
         mensaje = NULL;
      }
   }
}

/*=====[Implementation of private functions]=================================*/

/**
 * @brief   Callback para una tecla presionada
 * @param   tec   Puntero a la tecla presionada
 */
static void tec_pressed_callback( void *tec ) {

   // Busco el índice de la tecla que se presionó
   for( uint32_t i = 0 ; i < tecs_count ; i++ ) {
      if( &tecs[i] == (debounceButton_t*) tec ) {
         tec_start[i] = xTaskGetTickCount();
         break;
      }
   }
}

/**
 * @brief   Callback para una tecla liberada
 * @param   tec   Puntero a la tecla liberada
 */
static void tec_released_callback( void *tec ) {
   char *mensaje = NULL;
   TickType_t measurement;

   // Busco el índice de la tecla que se liberó
   for( uint32_t i = 0 ; i < tecs_count ; i++ ) {
      if( &tecs[i] == (debounceButton_t*) tec ) {
         measurement = xTaskGetTickCount() - tec_start[i];

         // Si la medición supera 4 dígitos se satura a 9999
         if( measurement >= 10000 ) measurement = 9999;

         mensaje = pvPortMalloc( sizeof(TEC_MSG) );

         if( mensaje != NULL ) {
            sprintf( mensaje, TEC_MSG_FORMAT, i+1, measurement );
            xQueueSend( cola_1, &mensaje, portMAX_DELAY );
         }

         break;
      }
   }
}
