#include "core_init.h"
#include "led.hh"

const uint32_t BOOT_LED_BLINK_TIMES = 10;
const uint32_t BOOT_LED_BLINK_DELAY = 100;
const uint32_t NORMAL_WORKING_BLINK_DELAY = 500;
int main() {
    core_init();

    const LedGreen ledGreen;
    const LedRed ledRed;

    for (uint32_t i = 0; i < BOOT_LED_BLINK_TIMES; i++) {
        ledRed.toggle();
        delay_ms(BOOT_LED_BLINK_DELAY);
    }
    ledRed.off();

    while (true) {
        ledGreen.on();
        delay_ms(NORMAL_WORKING_BLINK_DELAY);
        ledGreen.off();
        delay_ms(NORMAL_WORKING_BLINK_DELAY);
    }
}
