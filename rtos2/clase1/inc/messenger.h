/*=============================================================================
 * Author: Nahuel Espinosa <nahue.espinosa@gmail.com>
 * Date: 2020/10/26
 *===========================================================================*/

#ifndef _MESSENGER_H_
#define _MESSENGER_H_

/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Inclusions of function dependencies]=================================*/

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "sapi.h"

/*==================[typedef]================================================*/

typedef struct {
   QueueHandle_t queue;
} Messenger;

/*=====[Prototypes (declarations) of public functions]=======================*/

void Messenger_ctor( Messenger * const me, uint32_t queue_size );
bool_t Messenger_post( Messenger * const me, void * buffer, uint32_t size );
bool_t Messenger_get( Messenger * const me, void * buffer, uint32_t size );

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif
