/*=============================================================================
 * Author: Nahuel Espinosa <nahue.espinosa@gmail.com>
 * Date: 2020/08/17
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

//! Intervalo de actualización del estado de los relés en milisegundos
#define RELAY_UPDATE_TIME  20

/*=====[Public function-like macros]=========================================*/

/*=====[Definitions of public data types]====================================*/

//! Enumeración de relés disponibles
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

/*=====[Prototypes (declarations) of public functions]=======================*/

//! Inicializar las entradas/salidas y el estado de los relés
void relayInit(void);

/**
 * @brief Escribir el valor del pin de control del relé
 *
 * @param[in]   relay    Identificador del relé
 * @param[in]   value    Valor lógico alto o bajo (ON, OFF)
 */
void relayWrite(relay_t relay, bool_t value);

/**
 * @brief Leer el valor del pin de estado del relé
 *
 * @param[in]   relay    Identificador del relé
 * @return      ON       El pin está en un estado lógico bajo (relé activado)
 * @return      OFF      El pin está en un estado lógico alto (relé desactivado)
 */
bool_t relayRead(relay_t relay);

/**
 * @brief Revisar estado del relé
 *
 * Si se detectó una determinada cantidad de inconsistencias (RELAY_ERROR_THR)
 * se considera que hay una falla en el relé.
 *
 * @return      FALSE    Si el relé presenta una falla
 * @return      TRUE     Si el relé está funcionando con normalidad
 */
bool_t relayCheck(relay_t relay);

/**
 * @brief Revisar estado de todos los relés
 *
 * @return      FALSE    Si algún relé presenta una falla
 * @return      TRUE     Si todos los relés están funcionando con normalidad
 */
bool_t relayCheckAll(void);

/**
 * @brief Actualizar el estado de la llave rotativa
 *
 * Por cada relé, compara el pin de control con el pin de estado del mismo.
 * Si se detecta una inconsistencia se incrementa un contador interno de
 * errores.
 *
 * Hay algunos relés que están acoplados eléctricamente a la llave rotativa,
 * en ese caso puede ocurrir que la llave esté deshabilitada y eso provoque
 * que el relé se desactive independientemente del valor del pin de control.
 * En ese caso se ignoran esos errores.
 */
void relayUpdate(void);

/*=====[Prototypes (declarations) of public interrupt functions]=============*/

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* __RELAY_H__ */
