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
void safetyIsolatedModeEnable();

//! Deshabilitar el modo aislado limitado
void safetyIsolatedModeDisable();

//! Activar la se�al de corte de tracci�n
void safetySignalActivateCT();

//! Desactivar la se�al de corte de tracci�n
void safetySignalDeactivateCT();

//! Activar la se�al de freno de emergencia
void safetySignalActivateFE();

//! Desactivar la se�al de freno de emergencia
void safetySignalDeactivateFE();

/*=====[Prototypes (declarations) of public interrupt functions]=============*/

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* __SAFETY_H__ */
