#include "leds.h"

#define LEDS_ALL_OFF    0x0000
#define LED_OFFSET      1
#define LSB             1

static uint16_t *direccion;
static LedError_t RegistrarError;

uint16_t LedToMask(uint8_t led) {
    if (led > 16) {
        RegistrarError();
        return 0;
    }

    return LSB << (led-LED_OFFSET);
}

void Leds_Create(uint16_t * puerto, LedError_t handler) {
    *puerto = LEDS_ALL_OFF;
    direccion = puerto;
    RegistrarError = handler;
}

void Leds_TurnOn(uint8_t led) {
    *direccion |= LedToMask(led);
}

void Leds_TurnOff(uint8_t led) {
    *direccion &= ~LedToMask(led);
}