/*=============================================================================
 * Author: Nahuel Espinosa <nahue.espinosa@gmail.com>
 * Date: 2020/10/25
 *===========================================================================*/

#ifndef _DEBOUNCE_H_
#define _DEBOUNCE_H_

/*=====[Inclusions of function dependencies]=================================*/

#include "sapi.h"

/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================[typedef]================================================*/

typedef enum {
    DEBOUNCE_BUTTON_UP,
    DEBOUNCE_BUTTON_DOWN,
    DEBOUNCE_BUTTON_FALLING,
    DEBOUNCE_BUTTON_RISING
} debounceButtonState_t;

typedef struct {
   gpioMap_t gpio;
   tick_t refreshTime;

   callBackFuncPtr_t pressedCallback;
   callBackFuncPtr_t releasedCallback;

   debounceButtonState_t state;

   uint8_t contFalling;
   uint8_t contRising;

} debounceButton_t;

/*=====[Prototypes (declarations) of public functions]=======================*/

void debounceButtonUpdate( debounceButton_t* button );

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif
