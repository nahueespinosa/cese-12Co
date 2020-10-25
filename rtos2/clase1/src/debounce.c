/*=============================================================================
 * Author: Nahuel Espinosa <nahue.espinosa@gmail.com>
 * Date: 2020/10/25
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "debounce.h"

/*=====[Definition macros of private constants]==============================*/

#define DEBOUNCE_TIME      40

/*=====[Prototypes (declarations) of private functions]======================*/

/*=====[Implementation of public functions]==================================*/

void debounceButtonUpdate( debounceButton_t* button ) {
   switch( button->state ) {
      case DEBOUNCE_BUTTON_UP:
         if( !gpioRead( button->gpio ) ) {
            button->state = DEBOUNCE_BUTTON_FALLING;
         }
         break;

      case DEBOUNCE_BUTTON_FALLING:
         if( button->contFalling >= DEBOUNCE_TIME ) {
            if( !gpioRead( button->gpio ) ) {
               button->state = DEBOUNCE_BUTTON_DOWN;
               if( button->pressedCallback != NULL ) {
                  (*(button->pressedCallback))(button);
               }
            } else {
               button->state = DEBOUNCE_BUTTON_UP;
            }
            button->contFalling = 0;
         }

         button->contFalling += button->refreshTime;
         break;

      case DEBOUNCE_BUTTON_DOWN:
         if( gpioRead( button->gpio ) ) {
            button->state = DEBOUNCE_BUTTON_RISING;
         }
         break;

      case DEBOUNCE_BUTTON_RISING:
         if( button->contRising >= DEBOUNCE_TIME ) {
            if( gpioRead( button->gpio ) ) {
               button->state = DEBOUNCE_BUTTON_UP;
               if( button->releasedCallback != NULL ) {
                  (*(button->releasedCallback))(button);
               }
            } else {
               button->state = DEBOUNCE_BUTTON_DOWN;
            }
            button->contRising = 0;
         }
         button->contRising += button->refreshTime;
         break;

      default:
         button->state = DEBOUNCE_BUTTON_UP;
         break;
   }
}
