/*=============================================================================
 * Author: Nahuel Espinosa <nahue.espinosa@gmail.com>
 * Date: 2020/08/11
 *===========================================================================*/

/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef __RELAY_H__
#define __RELAY_H__

/*=====[Inclusions of public function dependencies]==========================*/

#include "sapi.h"

/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Definition macros of public constants]===============================*/

#define RELAY_UPDATE_TIME  10
#define RELAY_ERROR_THR    5

/*=====[Public function-like macros]=========================================*/

/*=====[Definitions of public data types]====================================*/

typedef enum {
   RELAY_CT_1,
   RELAY_CT_2,
   RELAY_CT_3,
   RELAY_FE_1,
   RELAY_FE_2,
   RELAY_FE_3,
   RELAY_REG_1,
   RELAY_REG_2,
   RELAY_REG_3,
   RELAY_REG_4,
   RELAY_NUM,
} relay_t;

typedef void (*relayCallback_t)(relay_t);

/*=====[Prototypes (declarations) of public functions]=======================*/

//! Inicializar las entradas/salidas y el estado de los relés
void relayInit();

//! Activar o desactivar un relé
void relayWrite(relay_t relay, bool_t value);

//! Leer el estado de un relé
bool_t relayRead(relay_t relay);

//! Revisar inconsistencias en el estado de un relé
bool_t relayCheck(relay_t relay);

//! Revisar inconsistencias en el estado de todos los relés
bool_t relayCheckAll();

//! Actualizar el estado de todos los relés
void relayUpdate();

/*=====[Prototypes (declarations) of public interrupt functions]=============*/

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* __RELAY_H__ */
