/*=============================================================================
 * Author: Nahuel Espinosa <nahue.espinosa@gmail.com>
 * Date: 2020/10/25
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include <string.h>
#include "tareas.h"

#include "sapi.h"

#include "debounce.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/*=====[Definition macros of private constants]==============================*/

#define  LED                  LEDB

#define  LED_PERIODICITY_MS   1000
#define  LED_PERIODICITY      pdMS_TO_TICKS(LED_PERIODICITY_MS)

#define  LED_MSG              "LED ON\n"

#define  TEC_PERIODICITY_MS   5
#define  TEC_PERIODICITY      pdMS_TO_TICKS(TEC_PERIODICITY_MS)

#define  TEC_MSG              "TECx Tyyyy\n"
#define  TEC_MSG_FORMAT       "TEC%d T%04d\n"

#define  TEC_COUNT            sizeof(tecs)/sizeof(tecs[0])

#define  UART                 UART_USB
#define  UART_BAUDRATE        115200

#define  QUEUE_SIZE           20

/*=====[Prototypes (declarations) of private functions]======================*/

static void tarea_A( void* pvParameters );
static void tarea_B( void* pvParameters );
static void tarea_C( void* pvParameters );

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

static TickType_t tec_start[TEC_COUNT];

/*=====[Implementation of public functions]==================================*/

/**
 * @brief   Inicialización de tareas
 */
void tareasInit( void ) {
   BaseType_t res;

   cola_1 = xQueueCreate( QUEUE_SIZE, sizeof(void *) );

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

/*=====[Implementation of private functions]=================================*/

/**
 * @brief   Tarea A
 *
 * Enciende periódicamente un LED. Cada vez que el LED enciende, se
 * envía un mensaje "LED ON" a la "cola_1".
 */
static void tarea_A( void* pvParameters ) {
   char *mensaje = NULL;

   TickType_t xLastWakeTime = xTaskGetTickCount();

   while ( TRUE ) {
      gpioToggle(LED);

      if( gpioRead(LED) == ON ) {
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
 * Lee con antirrebote el estado de todas las teclas. Mide el tiempo
 * de pulsación a través de las funciones "tec_pressed_callback"
 * y "tec_released_callback".
 *
 * Al medirse el tiempo de pulsación, envía el mensaje "TECx Tyyyy"
 * a la "cola_1" donde x es el índice e yyyy la cantidad de ms
 * que fue pulsada.
 */
static void tarea_B( void* pvParameters ) {
   TickType_t xLastWakeTime = xTaskGetTickCount();

   while ( TRUE ) {

      // Actualizo el estado de cada tecla por polling
      for( uint32_t i = 0 ; i < TEC_COUNT ; i++ ) {
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
static void tarea_C( void* pvParameters ) {
   char *mensaje = NULL;

   uartConfig( UART, UART_BAUDRATE );

   while ( TRUE ) {

      xQueueReceive( cola_1, &mensaje, portMAX_DELAY );

      if( mensaje != NULL ) {
         uartWriteString( UART, mensaje );
         vPortFree( mensaje );
         mensaje = NULL;
      }
   }
}

/**
 * @brief   Callback para una tecla presionada
 * @param   tec   Puntero a la tecla presionada
 */
static void tec_pressed_callback( void *tec ) {

   // Busco el índice de la tecla que se presionó
   for( uint32_t i = 0 ; i < TEC_COUNT ; i++ ) {
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
   for( uint32_t i = 0 ; i < TEC_COUNT ; i++ ) {
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
