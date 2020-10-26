/*=============================================================================
 * Author: Nahuel Espinosa <nahue.espinosa@gmail.com>
 * Date: 2020/10/26
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "messenger.h"
#include <string.h>

/*==================[typedef]================================================*/

typedef struct {
   void * ptr;
   uint32_t size;
} Message_t;

/*=====[Implementation of public functions]==================================*/

/**
 * @brief   Constructor del objeto
 *
 * @param[in]  me          Referencia al objeto
 * @param[in]  queue_size  Cantidad máxima de mensajes
 */
void Messenger_ctor( Messenger * const me, uint32_t queue_size ) {
   me->queue = xQueueCreate( queue_size, sizeof(Message_t) );

   configASSERT( me->queue != NULL );
}

/**
 * @brief   Enviar mensaje
 *
 * Reserva memoria dinámica para el mensaje y envía el puntero a través
 * de una cola.
 *
 * @param[in]  me          Referencia del objeto
 * @param[in]  buffer      Buffer con el mensaje a transmitir
 * @param[in]  size        Tamaño del mensaje en bytes
 *
 * @return     TRUE        Si pudo alocar la memoria dinámica
 *             FALSE       Si no pudo alocar la memoria dinámica
 */
bool_t Messenger_post( Messenger * const me, void * buffer, uint32_t size ) {
   Message_t msg;

   msg.ptr = pvPortMalloc( size );

   if( msg.ptr != NULL ) {
      msg.size = size;
      memcpy( msg.ptr, buffer, size );
      xQueueSend( me->queue, &msg, portMAX_DELAY );
      return TRUE;
   } else {
      return FALSE;
   }
}

/**
 * @brief   Obtener mensaje
 *
 * Espera de forma BLOQUEANTE la llegada de un nuevo mensaje.
 * Libera la memoria reservada y devuelve el resultado en un buffer de
 * recepción.
 *
 * @param[in]  me          Referencia al objeto
 * @param[out] buffer      Buffer de recepción
 * @param[out] size        Tamaño del mensaje recibido en bytes
 *
 * @return     TRUE        Si recibió un puntero no nulo.
 *             FALSE       Si recibió un puntero nulo.
 */
bool_t Messenger_get( Messenger * const me, void * buffer, uint32_t * size ) {
   Message_t msg;
   xQueueReceive( me->queue, &msg, portMAX_DELAY );

   if( msg.ptr != NULL ) {
      memcpy( buffer, msg.ptr, msg.size );
      *size = msg.size;
      vPortFree( msg.ptr );
      msg.ptr = NULL;
      return TRUE;
   } else {
      return FALSE;
   }
}
