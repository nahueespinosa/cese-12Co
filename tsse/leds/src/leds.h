#include <stdint.h>

typedef void (*LedError_t)(void);

void Leds_Create(uint16_t * puerto, LedError_t handler);
void Leds_TurnOn(uint8_t led);
void Leds_TurnOff(uint8_t led);