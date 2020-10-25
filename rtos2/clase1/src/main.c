/*=============================================================================
 * Author: Nahuel Espinosa <nahue.espinosa@gmail.com>
 * Date: 2020/10/25
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include <string.h>

#include "sapi.h"

#include "debounce.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/*=====[Definition macros of private constants]==============================*/

#define  QUEUE_SIZE           20

#define  LED_PERIODICITY_MS   1000
#define  LED_PERIODICITY      pdMS_TO_TICKS(LED_PERIODICITY_MS)

#define  LED_MSG              "LED ON\n"

#define  TEC_PERIODICITY_MS   5
#define  TEC_PERIODICITY      pdMS_TO_TICKS(TEC_PERIODICITY_MS)

#define  TEC_MSG              "TECx Tyyyy\n"
#define  TEC_MSG_FORMAT       "TEC%d T%04d\n"

#define  UART_BAUDRATE        115200

/*=====[Definitions of private global variables]=============================*/

QueueHandle_t cola_1;

/*=====[Prototypes (declarations) of private functions]======================*/

void tarea_A( void* pvParameters );
void tarea_B( void* pvParameters );
void tarea_C( void* pvParameters );

/*=====[Implementation of public functions]==================================*/

int main( void )
{
   BaseType_t res;

   boardConfig();

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

   vTaskStartScheduler();

   // ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE )
   {
      // Si cae en este while 1 significa que no pudo iniciar el scheduler
   }

   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
   // directamenteno sobre un microcontroladore y no es llamado por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return TRUE;
}

/*=====[Implementation of private functions]=================================*/

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

TickType_t tec1_start, tec2_start;

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

   if( measurement >= 10000 ) measurement = 9999;

   mensaje = pvPortMalloc( sizeof(TEC_MSG) );

   if( mensaje != NULL ) {
      sprintf( mensaje, TEC_MSG_FORMAT, index, measurement );
      xQueueSend( cola_1, &mensaje, portMAX_DELAY );
   }
}

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
