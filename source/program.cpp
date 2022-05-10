#include "Leds.hh"
#include "boot/core_init.h"

const uint32_t BOOT_LED_BLINK_TIMES       = 10;
const uint32_t BOOT_LED_BLINK_DELAY       = 100;
const uint32_t NORMAL_WORKING_BLINK_DELAY = 500;

__NO_RETURN void program() {
    const auto& ledGreen = LedGreen::getInstance();
    const auto& ledRed   = LedRed::getInstance();
    // const Led<GPIO::Port::B, GPIO::Pin::P1> ledRed;
    // const Led<GPIO::Port::B, GPIO::Pin::P11> ledGreen;

    // const GPIO::OutputMode<GPIO::Port::E, GPIO::Pin::P14> fmuCH1Pin;

    for (uint32_t i = 0; i < BOOT_LED_BLINK_TIMES; i++) {
        ledRed.toggle();
        delay_ms(BOOT_LED_BLINK_DELAY);
    }
    ledRed.off();
    while (true) {
        // fmuCH1Pin.toggle();
        ledGreen.toggle();
        delay_ms(NORMAL_WORKING_BLINK_DELAY);
    }
}
