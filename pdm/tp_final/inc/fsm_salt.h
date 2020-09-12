/*=============================================================================
 * Author: Nahuel Espinosa <nahue.espinosa@gmail.com>
 * Date: 2020/08/17
 *===========================================================================*/

/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef __FSM_SALT_H__
#define __FSM_SALT_H__

/*=====[Inclusions of public function dependencies]==========================*/

#include "../../tp_final/inc/relay.h"
#include "../../tp_final/inc/safety.h"
#include "../../tp_final/inc/switch.h"

/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Definition macros of public constants]===============================*/

//! Intervalo de actualización del estado del SAL/T en milisegundos
#define SALT_UPDATE_TIME  100

/*=====[Prototypes (declarations) of public functions]=======================*/

//! Inicializar los subcomponentes, relés y llave rotativa del módulo SAL/T.
void saltInit(void);

//! Actualizar la máquina de estados del módulo SAL/T
void saltUpdate(void);

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* __FSM_SALT_H__ */
