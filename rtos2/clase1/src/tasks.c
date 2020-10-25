/*=============================================================================
 * Author: Nahuel Espinosa <nahue.espinosa@gmail.com>
 * Date: 2020/10/25
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "tasks.h"
#include <string.h>

#include "sapi.h"

#include "debounce.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/*=====[Definitions of private global variables]=============================*/

static QueueHandle_t cola_1;
static TickType_t tec1_start, tec2_start;

/*=====[Prototypes (declarations) of private functions]======================*/

static void tec_pressed_callback( void *param );
static void tec_released_callback( void *param );

/*=====[Implementation of public functions]==================================*/

/**
 * @brief Inicialización de tareas
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
 * @brief Tarea A
 *
 * Enciende periódicamente un LED. Cada vez que el LED enciende, se
 * envía un mensaje "LED ON" a la "cola_1".
 */
void tarea_A( void* pvParameters ) {
   char *mensaje = NULL;

   TickType_t xLastWakeTime = xTaskGetTickCount();

   while ( TRUE) {
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
 * @brief Tarea B
 *
 * Lee con antirrebote el estado de 2 teclas. Mide el tiempo
 * de pulsación a través de las funciones "tec_pressed_callback"
 * y "tec_released_callback".
 *
 * Al medirse el tiempo de pulsación, envía el mensaje "TECx Tyyyy"
 * a la "cola_1" donde x es el índice e yyyy la cantidad de ms
 * que fue pulsada.
 */
void tarea_B( void* pvParameters ) {
   debounceButton_t tec_1;
   debounceButton_t tec_2;

   char *mensaje = NULL;

   TickType_t xLastWakeTime = xTaskGetTickCount();

   debounceButtonInit( &tec_1, TEC1, TEC_PERIODICITY_MS,
      tec_pressed_callback, tec_released_callback );

   debounceButtonInit( &tec_2, TEC2, TEC_PERIODICITY_MS,
      tec_pressed_callback, tec_released_callback );

   while ( TRUE) {
      debounceButtonUpdate( &tec_1 );
      debounceButtonUpdate( &tec_2 );

      vTaskDelayUntil( &xLastWakeTime, TEC_PERIODICITY );
   }
}

/**
 * @brief Tarea C
 *
 * Obtiene de "cola_1" mensajes de texto y los envía por la
 * UART_USB.
 */
void tarea_C( void* pvParameters ) {
   char *mensaje = NULL;

   uartConfig( UART_USB, UART_BAUDRATE );

   while ( TRUE) {

      xQueueReceive( cola_1, &mensaje, portMAX_DELAY );

      if( mensaje != NULL ) {
         uartWriteString( UART_USB, mensaje );
         vPortFree( mensaje );
         mensaje = NULL;
      }

   }
}

/*=====[Implementation of private functions]=================================*/

static void tec_pressed_callback( void *param ) {
   debounceButton_t *tec = (debounceButton_t*) param;

   switch( tec->gpio ) {
      case TEC1:
         tec1_start = xTaskGetTickCount();
         break;
      case TEC2:
         tec2_start = xTaskGetTickCount();
         break;
   }
}

static void tec_released_callback( void *param ) {
   debounceButton_t *tec = (debounceButton_t*) param;
   TickType_t measurement;
   uint32_t index;

   char *mensaje = NULL;

   switch( tec->gpio ) {
      case TEC1:
         measurement = xTaskGetTickCount() - tec1_start;
         index = 1;
         break;
      case TEC2:
         measurement = xTaskGetTickCount() - tec2_start;
         index = 2;
         break;
   }

   // Si la medición supera 4 dígitos se satura a 9999
   if( measurement >= 10000 ) measurement = 9999;

   mensaje = pvPortMalloc( sizeof(TEC_MSG) );

   if( mensaje != NULL ) {
      sprintf( mensaje, TEC_MSG_FORMAT, index, measurement );
      xQueueSend( cola_1, &mensaje, portMAX_DELAY );
   }
}
