/*=============================================================================
 * Author: Nahuel Espinosa <nahue.espinosa@gmail.com>
 * Date: 2020/10/25
 *===========================================================================*/

#ifndef _DEBOUNCE_H_
#define _DEBOUNCE_H_

/*=====[Inclusions of function dependencies]=================================*/

#include "FreeRTOS.h"
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
   debounceButtonState_t state;
   tick_t refreshTime;

   uint8_t contFalling;
   uint8_t contRising;

   callBackFuncPtr_t pressedCallback;
   callBackFuncPtr_t releasedCallback;

} debounceButton_t;

/*=====[Prototypes (declarations) of public functions]=======================*/

void debounceButtonInit(
      debounceButton_t* button,
      gpioMap_t gpio,
      tick_t refreshTime,
      callBackFuncPtr_t pressedCallback,
      callBackFuncPtr_t releasedCallback
);

void debounceButtonUpdate( debounceButton_t* button );

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif
