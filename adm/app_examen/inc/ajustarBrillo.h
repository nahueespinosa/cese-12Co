#ifndef _ASSEMBLY_H_
#define _ASSEMBLY_H_

#include "stdint.h"

/** \addtogroup blink Bare-metal blink example
 ** @{ */

/*==================[inclusions]=============================================*/

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

extern void ajustarBrillo_simd (uint8_t * imagen, uint32_t ancho, uint32_t alto, int8_t brillo);

extern void ajustarBrillo_asm (uint8_t * imagen, uint32_t ancho, uint32_t alto, int8_t brillo);

extern void ajustarBrillo_c (uint8_t * imagen, uint32_t ancho, uint32_t alto, int8_t brillo);

/** @brief main function
 * @return main function should never return
 */
/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
}
#endif

/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* #ifndef _MAIN_H_ */
