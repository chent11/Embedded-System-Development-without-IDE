#include "Leds.hh"
#include "core_init.h"

const uint32_t BOOT_LED_BLINK_TIMES       = 10;
const uint32_t BOOT_LED_BLINK_DELAY       = 100;
const uint32_t NORMAL_WORKING_BLINK_DELAY = 500;

// class GpioPE14FmuCH1 : protected GPIODef<GPIO::Mode::Output> {
//   public:
//     using GPIO::toggle;
//     GpioPE14FmuCH1() : GPIODef(Port::E, Pin::P14, Pull::NoPull, OutputType::PushPull, Speed::Medium) {}
// };

int main() {
    core_init();

    const auto& ledGreen = LedGreen::getInstance();
    const auto& ledRed   = LedRed::getInstance();
    // const GpioPE14FmuCH1 testPin;

    for (uint32_t i = 0; i < BOOT_LED_BLINK_TIMES; i++) {
        ledRed.toggle();
        delay_ms(BOOT_LED_BLINK_DELAY);
    }
    ledRed.off();
    while (true) {
        // testPin.toggle();
        ledGreen.toggle();
        delay_ms(NORMAL_WORKING_BLINK_DELAY);
    }
}
