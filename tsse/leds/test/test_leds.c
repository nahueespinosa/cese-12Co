#include "unity.h"
#include "leds.h"

typedef int bool;

#define false   0
#define true    1

uint16_t ledsVirtuales;
bool error_informado = false;

void RegistrarError(void) {
    error_informado = true;
}

void setUp(void) {
    error_informado = false;
    Leds_Create(&ledsVirtuales, RegistrarError);
}

void tearDown(void) {
}

void test_LedsOffAfterCreate(void) {
    uint16_t ledsVirtuales = 0xFFFF;
    Leds_Create(&ledsVirtuales, RegistrarError);
    TEST_ASSERT_EQUAL_HEX16(0, ledsVirtuales);
}

void test_prender_led_individual(void) {
    Leds_TurnOn(4);
    TEST_ASSERT_EQUAL_HEX16(0x8, ledsVirtuales);
}

void test_apagar_led_individual(void) {
    Leds_TurnOn(1);
    Leds_TurnOff(1);
    TEST_ASSERT_EQUAL_HEX16(0, ledsVirtuales);
}

// Revisar parámetros fuera de los límites.
void test_prender_led_invalido(void) {
    Leds_TurnOn(17);
    TEST_ASSERT_TRUE(error_informado);
}