/*=============================================================================
 * Author: Nahuel Espinosa <nahue.espinosa@gmail.com>
 * Date: 2020/08/17
 *===========================================================================*/

/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef __SWITCH_H__
#define __SWITCH_H__

/*=====[Inclusions of public function dependencies]==========================*/

#include "sapi.h"

/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Definition macros of public constants]===============================*/

//! Intervalo de actualización del estado de la llave rotativa en milisegundos
#define SWITCH_UPDATE_TIME    20

/*=====[Prototypes (declarations) of public functions]=======================*/

//! Inicializar las entradas de la llave rotativa
void switchInit(void);

/**
 * @brief Leer el estado actual del pin de la llave rotativa
 *
 * @return      ON       Si el pin está en estado bajo (activado)
 * @return      OFF      Si el pin está en estado alto (desactivado)
 */
bool_t switchReadCurrent(void);

/**
 * @brief Leer el estado de la llave rotativa
 *
 * @return      ON       Si la llave está en estado activado
 * @return      OFF      Si la llave está en estado desactivado
 */
bool_t switchRead(void);

/**
 * @brief Actualizar el estado de la llave rotativa
 *
 * Debe llamarse en intervalos de tiempo constantes (SWITCH_UPDATE_TIME).
 * Lee el estado del pin SWITCH_GPIO y si se mantiene estable durante una
 * determinada cantidad de lecturas (SWITCH_STATE_THR) actualiza el estado
 * de la llave al nuevo estado.
 */
void switchUpdate(void);

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* __SWITCH_H__ */
