/*=============================================================================
 * Author: Nahuel Espinosa <nahue.espinosa@gmail.com>
 * Date: 2020/08/17
 *===========================================================================*/

/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef __SAFETY_H__
#define __SAFETY_H__

/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Prototypes (declarations) of public functions]=======================*/

/**
 * @brief Habilitar el modo aislado limitado
 *
 * Acciona los relés correspondientes e informa al registrador de eventos.
 */
void safetyIsolatedModeEnable(void);

/**
 * @brief Deshabilitar el modo aislado limitado
 *
 * Acciona los relés correspondientes e informa al registrador de eventos.
 */
void safetyIsolatedModeDisable(void);

/**
 * @brief Activar la señal de corte de tracción
 *
 * Sólo acciona el relé correspondiente si puede verificar que el resto está
 * en el estado adecuado.
 *
 * Informa al registrador de eventos en caso de éxito.
 */
void safetySignalActivateCT(void);

/**
 * @brief Desactivar la señal de corte de tracción
 *
 * Sólo acciona el relé correspondiente si puede verificar que el resto está
 * en el estado adecuado.
 *
 * Informa al registrador de eventos en caso de éxito.
 */
void safetySignalDeactivateCT(void);

/**
 * @brief Activar la señal de freno de emergencia
 *
 * Sólo acciona el relé correspondiente si puede verificar que el resto está
 * en el estado adecuado.
 *
 * Informa al registrador de eventos en caso de éxito.
 */
void safetySignalActivateFE(void);

/**
 * @brief Desactivar la señal de freno de emergencia
 *
 * Sólo acciona el relé correspondiente si puede verificar que el resto está
 * en el estado adecuado.
 *
 * Informa al registrador de eventos en caso de éxito.
 */
void safetySignalDeactivateFE(void);

/*=====[Prototypes (declarations) of public interrupt functions]=============*/

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* __SAFETY_H__ */
