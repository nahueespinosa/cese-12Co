/*=============================================================================
 * Author: Nahuel Espinosa <nahue.espinosa@gmail.com>
 * Date: 2020/08/11
 *===========================================================================*/

/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef __FSM_SALT_H__
#define __FSM_SALT_H__

/*=====[Inclusions of public function dependencies]==========================*/

#include "relay.h"
#include "switch.h"

/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Definition macros of public constants]===============================*/

/*=====[Public function-like macros]=========================================*/

/*=====[Definitions of public data types]====================================*/

/*=====[Prototypes (declarations) of public functions]=======================*/

//! Inicializar los subcomponentes, rel�s y llave rotativa del m�dulo SAL/T.
void saltInit();

//! Actualizar la m�quina de estados del m�dulo SAL/T
void saltUpdate();

/*=====[Prototypes (declarations) of public interrupt functions]=============*/

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* __FSM_SALT_H__ */
