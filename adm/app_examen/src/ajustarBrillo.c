#include "ajustarBrillo.h"


void ajustarBrillo_c (uint8_t * imagen, uint32_t ancho, uint32_t alto, int8_t brillo)
{
   uint32_t i;
   int16_t aux;

   for( i = 0 ; i < (ancho * alto) ; i++ ) {
      aux = imagen[i] + brillo;

      if( aux > UINT8_MAX ) {
         imagen[i] = UINT8_MAX;
      } else if( aux < 0 ) {
         imagen[i] = 0;
      } else {
         imagen[i] = aux;
      }
   }
}
