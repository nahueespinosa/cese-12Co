#include "build/temp/_test_leds.c"
#include "src/leds.h"
#include "/var/lib/gems/2.5.0/gems/ceedling-0.30.0/vendor/unity/src/unity.h"


typedef int bool;









uint16_t ledsVirtuales;

bool error_informado = 0;



void RegistrarError(void) {

    error_informado = 1;

}



void setUp(void) {

    error_informado = 0;

    Leds_Create(&ledsVirtuales, RegistrarError);

}



void tearDown(void) {

}



void test_LedsOffAfterCreate(void) {

    uint16_t ledsVirtuales = 0xFFFF;

    Leds_Create(&ledsVirtuales, RegistrarError);

    UnityAssertEqualNumber((UNITY_INT)(UNITY_INT16)((0)), (UNITY_INT)(UNITY_INT16)((ledsVirtuales)), (

   ((void *)0)

   ), (UNITY_UINT)(27), UNITY_DISPLAY_STYLE_HEX16);

}



void test_prender_led_individual(void) {

    Leds_TurnOn(4);

    UnityAssertEqualNumber((UNITY_INT)(UNITY_INT16)((0x8)), (UNITY_INT)(UNITY_INT16)((ledsVirtuales)), (

   ((void *)0)

   ), (UNITY_UINT)(32), UNITY_DISPLAY_STYLE_HEX16);

}



void test_apagar_led_individual(void) {

    Leds_TurnOn(1);

    Leds_TurnOff(1);

    UnityAssertEqualNumber((UNITY_INT)(UNITY_INT16)((0)), (UNITY_INT)(UNITY_INT16)((ledsVirtuales)), (

   ((void *)0)

   ), (UNITY_UINT)(38), UNITY_DISPLAY_STYLE_HEX16);

}





void test_prender_led_invalido(void) {

    Leds_TurnOn(17);

    do {if ((error_informado)) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(44)));}} while(0);

}
