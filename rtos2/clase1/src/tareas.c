/*=============================================================================
 * Author: Nahuel Espinosa <nahue.espinosa@gmail.com>
 * Date: 2020/10/25
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include <string.h>
#include "tareas.h"

#include "sapi.h"

#include "debounce.h"
#include "messenger.h"

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
#define  TEC_MSG_FORMAT       "TEC%c T%04d\n"

#define  TEC_COUNT            sizeof(tecs)/sizeof(tecs[0])

#define  UART                 UART_USB
#define  UART_BAUDRATE        115200

#define  UART_MSG_MAX_SIZE    15

#define  QUEUE_SIZE           20

/*=====[Prototypes (declarations) of private functions]======================*/

static void tarea_A( void* pvParameters );
static void tarea_B( void* pvParameters );
static void tarea_C( void* pvParameters );
static void tarea_D( void* pvParameters );

static void tec_pressed_callback( void * );
static void tec_released_callback( void * );

/*==================[typedef]================================================*/

typedef struct {
   debounceButton_t  super;
   TickType_t        start_time;
   char              ascii_number;
} appButton_t;

/*=====[Definitions of private global variables]=============================*/

static Messenger obj1, obj2;

static appButton_t tecs[] = {
   { TEC1, TEC_PERIODICITY_MS, tec_pressed_callback, tec_released_callback,
         DEBOUNCE_BUTTON_UP, 0, 0, 0, '1' },
   { TEC2, TEC_PERIODICITY_MS, tec_pressed_callback, tec_released_callback,
         DEBOUNCE_BUTTON_UP, 0, 0, 0, '2' },
};

/*=====[Implementation of public functions]==================================*/

/**
 * @brief   Inicialización de tareas
 */
void tareasInit( void ) {
   BaseType_t res;

   Messenger_ctor( &obj1, QUEUE_SIZE );
   Messenger_ctor( &obj2, QUEUE_SIZE );

   res = xTaskCreate( tarea_A, (const char *) "Tarea A",
         configMINIMAL_STACK_SIZE * 2, 0, tskIDLE_PRIORITY + 2, 0 );

   configASSERT( res == pdPASS );

   res = xTaskCreate( tarea_B, (const char *) "Tarea B",
         configMINIMAL_STACK_SIZE * 2, 0, tskIDLE_PRIORITY + 2, 0 );

   configASSERT( res == pdPASS );

   res = xTaskCreate( tarea_C, (const char *) "Tarea C",
         configMINIMAL_STACK_SIZE * 2, 0, tskIDLE_PRIORITY + 1, 0 );

   configASSERT( res == pdPASS );

   res = xTaskCreate( tarea_D, (const char *) "Tarea D",
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
   char mensaje[] = LED_MSG;

   TickType_t xLastWakeTime = xTaskGetTickCount();

   while ( TRUE ) {
      gpioToggle(LED);

      if( gpioRead(LED) == ON ) {
         Messenger_post( &obj1, (void*) mensaje, sizeof(mensaje) );
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
         debounceButtonUpdate( (debounceButton_t*)&tecs[i] );
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
   char mensaje[UART_MSG_MAX_SIZE];

   uartConfig( UART, UART_BAUDRATE );

   while ( TRUE ) {
      if( Messenger_get( &obj1, (void*) mensaje, UART_MSG_MAX_SIZE ) ) {
         Messenger_post( &obj2, (void*) mensaje, UART_MSG_MAX_SIZE );
         uartWriteString( UART, mensaje );
      }
   }
}

/**
 * @brief   Tarea D
 *
 * Esta tarea consume las cadenas de texto de la Tarea C.
 * Si recibe algo blinkea el LED3.
 */
static void tarea_D( void* pvParameters ) {
   char mensaje[UART_MSG_MAX_SIZE];

   gpioWrite(LED3, OFF);

   while ( TRUE ) {
      if( Messenger_get( &obj2, (void*) mensaje, UART_MSG_MAX_SIZE ) ) {
         gpioWrite(LED3, ON);
         vTaskDelay( pdMS_TO_TICKS(50) );
         gpioWrite(LED3, OFF);
      }
   }
}

/**
 * @brief   Callback para una tecla presionada
 *
 * Empieza a medir el tiempo.
 *
 * @param   tec   Puntero a la tecla presionada
 */
static void tec_pressed_callback( void *tec ) {
   appButton_t *currentTec = (appButton_t *) tec;
   currentTec->start_time = xTaskGetTickCount();
}

/**
 * @brief   Callback para una tecla liberada
 *
 * Toma la medición de tiempo presionada y envía el dato
 * a la cola.
 *
 * @param   tec   Puntero a la tecla liberada
 */
static void tec_released_callback( void *tec ) {
   char mensaje[] = TEC_MSG;
   TickType_t measurement;
   appButton_t *currentTec = (appButton_t *) tec;

   measurement = xTaskGetTickCount() - currentTec->start_time;

   // Si la medición supera 4 dígitos se satura a 9999
   if( measurement >= 10000 ) measurement = 9999;

   sprintf( mensaje, TEC_MSG_FORMAT, currentTec->ascii_number, measurement );
   Messenger_post( &obj1, (void*) mensaje, sizeof(mensaje) );
}
