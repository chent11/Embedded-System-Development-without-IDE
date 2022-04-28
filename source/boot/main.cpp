#include "core_init.h"
#include "led.hh"

const uint32_t BOOT_LED_BLINK_TIMES       = 10;
const uint32_t BOOT_LED_BLINK_DELAY       = 100;
const uint32_t NORMAL_WORKING_BLINK_DELAY = 500;

// class GpioPE14FmuCH1 : protected GPIO {
//   public:
//     using GPIO::toggle;
//     GpioPE14FmuCH1() : GPIO(Port::E, Pin::P14, Mode::OutputPushPull, Pull::NoPull, Speed::Medium) {}
// };

int main() {
    core_init();

    const auto&          ledGreen = LedGreen::getInstance();
    const auto&          ledRed   = LedRed::getInstance();
    // const GpioPE14FmuCH1 testPin;

    for (uint32_t i = 0; i < BOOT_LED_BLINK_TIMES; i++) {
        ledRed.toggle();
        delay_ms(BOOT_LED_BLINK_DELAY);
    }
    ledRed.off();
    ledGreen.on();
    while (true) {
        // testPin.toggle();
        ledGreen.on();
        delay_ms(NORMAL_WORKING_BLINK_DELAY);
        // testPin.toggle();
        ledGreen.off();
        delay_ms(NORMAL_WORKING_BLINK_DELAY);
    }
}
