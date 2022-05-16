#include "Leds.hh"
#include "boot/cmsis_startup/system_ARMCM4.h"
#include "core_init.h"

const uint32_t BOOT_LED_BLINK_TIMES = 10;
const uint32_t BOOT_LED_BLINK_DELAY = 100;
const uint32_t TOGGLE_DELAY = 500;

__NO_RETURN void program() noexcept {
    using namespace GPIO;  // NOLINT(google-build-using-namespace)

    Output<Port::E, Pin::P3>::instancelessInit(DefaultState::Low);  // sensor power pin

    const auto& ledGreen = LedGreenInstance::get();
    const auto& ledRed = LedRedInstance::get();

    for (uint32_t i = 0; i < BOOT_LED_BLINK_TIMES; i++) {
        ledRed.toggle();
        delay_ms(BOOT_LED_BLINK_DELAY);
    }
    ledRed.off();

    while (true) {
        ledGreen.toggle();
        delay_ms(TOGGLE_DELAY);
    }
}
