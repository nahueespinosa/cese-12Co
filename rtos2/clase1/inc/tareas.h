/*=============================================================================
 * Author: Nahuel Espinosa <nahue.espinosa@gmail.com>
 * Date: 2020/10/25
 *===========================================================================*/

#ifndef _TAREAS_H_
#define _TAREAS_H_

/*=====[Inclusions of function dependencies]=================================*/

/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Definition macros of public constants]===============================*/

#define  QUEUE_SIZE           20

#define  LED_PERIODICITY_MS   1000
#define  LED_PERIODICITY      pdMS_TO_TICKS(LED_PERIODICITY_MS)

#define  LED_MSG              "LED ON\n"

#define  TEC_PERIODICITY_MS   5
#define  TEC_PERIODICITY      pdMS_TO_TICKS(TEC_PERIODICITY_MS)

#define  TEC_MSG              "TECx Tyyyy\n"
#define  TEC_MSG_FORMAT       "TEC%d T%04d\n"

#define  UART_BAUDRATE        115200

/*=====[Prototypes (declarations) of public functions]=======================*/

void tareasInit( void );
void tarea_A( void* pvParameters );
void tarea_B( void* pvParameters );
void tarea_C( void* pvParameters );

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif
