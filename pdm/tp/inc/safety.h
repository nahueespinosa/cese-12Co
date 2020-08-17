/*=============================================================================
 * Author: Nahuel Espinosa <nahue.espinosa@gmail.com>
 * Date: 2020/08/11
 *===========================================================================*/

/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef __SAFETY_H__
#define __SAFETY_H__

/*=====[Inclusions of public function dependencies]==========================*/

/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Definition macros of public constants]===============================*/

/*=====[Public function-like macros]=========================================*/

/*=====[Definitions of public data types]====================================*/

/*=====[Prototypes (declarations) of public functions]=======================*/

//! Habilitar el modo aislado limitado
void safetyIsolatedModeEnable(void);

//! Deshabilitar el modo aislado limitado
void safetyIsolatedModeDisable(void);

//! Activar la señal de corte de tracción
void safetySignalActivateCT(void);

//! Desactivar la señal de corte de tracción
void safetySignalDeactivateCT(void);

//! Activar la señal de freno de emergencia
void safetySignalActivateFE(void);

//! Desactivar la señal de freno de emergencia
void safetySignalDeactivateFE(void);

/*=====[Prototypes (declarations) of public interrupt functions]=============*/

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* __SAFETY_H__ */
