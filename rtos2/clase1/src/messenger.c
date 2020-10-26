/*=============================================================================
 * Author: Nahuel Espinosa <nahue.espinosa@gmail.com>
 * Date: 2020/10/26
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "messenger.h"
#include <string.h>

/*=====[Implementation of public functions]==================================*/

/**
 * @brief   Constructor del objeto
 * @param   me          Referencia al objeto
 * @param   queue_size  Cantidad máxima de mensajes
 */
void Messenger_ctor( Messenger * const me, uint32_t queue_size ) {
   me->queue = xQueueCreate( queue_size, sizeof(void *) );

   configASSERT( me->queue != NULL );
}

/**
 * @brief   Enviar mensaje
 *
 * Reserva memoria dinámica para el mensaje y envía el puntero a través
 * de una cola.
 *
 * @param   me          Referencia del objeto
 * @param   buffer      Buffer con el mensaje a transmitir
 * @param   size        Tamaño del mensaje en bytes
 *
 * @return  TRUE        Si se pudo enviar
 *          FALSE       Si no se pudo enviar
 */
bool_t Messenger_post( Messenger * const me, void * buffer, uint32_t size ) {
   char *message;
   message = pvPortMalloc( size );

   memcpy( message, buffer, size );

   if( buffer != NULL ) {
      xQueueSend( me->queue, &message, portMAX_DELAY );
      return TRUE;
   } else {
      return FALSE;
   }
}

/**
 * @brief   Obtener mensaje
 *
 * Espera de forma bloqueante la llegada de un nuevo mensaje.
 * Libera la memoria reservada y devuelve el resultado en un buffer de
 * recepción.
 *
 * @param   me          Referencia al objeto
 * @param   buffer      Buffer de recepción
 * @param   size        Tamaño máximo del mensaje en bytes
 *
 * @return  TRUE        Si recibió un puntero no nulo.
 *          FALSE       Si recibió un puntero nulo.
 */
bool_t Messenger_get( Messenger * const me, void * buffer, uint32_t size ) {
   char *message;
   xQueueReceive( me->queue, &message, portMAX_DELAY );

   if( buffer != NULL ) {
      memcpy( buffer, message, size );
      vPortFree( message );
      message = NULL;
      return TRUE;
   } else {
      return FALSE;
   }
}
