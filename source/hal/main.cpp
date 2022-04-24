#include <algorithm>
#include <array>
#include <numeric>
#include <vector>

#include "arm_math.h"
#include "core_init.h"
#include "led.hh"
#include "stm32f4xx_hal.h"

void delay_ms(int time) { HAL_Delay(time); }

const uint32_t BOOT_LED_BLINK_TIMES = 10;
const uint32_t BOOT_LED_BLINK_DELAY = 100;
const uint32_t NORMAL_WORKING_BLINK_DELAY = 500;

const uint32_t LENGTH = 1000;
std::array<float, LENGTH> arr = {0};  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
std::vector<float> vec(LENGTH, 0);    // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,fuchsia-statically-constructed-objects)

int main() {
    core_init();

    LedGreen led;
    GpioPE14_FmuCH1 signal1;
    // Led* signal2 = new GpioPD14_FmuCH6();
    LedRed ledRed;
    for (uint32_t i = 0; i < BOOT_LED_BLINK_TIMES; i++) {
        ledRed.toggle();
        delay_ms(BOOT_LED_BLINK_DELAY);
    }

    float volatile testVar = 0.F;  // cppcheck-suppress variableScope
    int volatile alignValue = sizeof(Led);
    while (true) {
        // Test 1
        led.on();
        signal1.on();
        for (auto& j : vec) {
            j = arm_cos_f32(static_cast<float>(rand()));  // cppcheck-suppress useStlAlgorithm; NOLINT(concurrency-mt-unsafe)
        }

        testVar = std::accumulate(vec.begin(), vec.end(), 0.F);
        delay_ms(NORMAL_WORKING_BLINK_DELAY);

        // Test 2
        led.off();
        signal1.off();
        std::generate(arr.begin(), arr.end(), []() mutable { return arm_cos_f32(static_cast<float>(rand())); });  // NOLINT(concurrency-mt-unsafe)

        testVar = std::accumulate(vec.begin(), vec.end(), 0.F);
        delay_ms(NORMAL_WORKING_BLINK_DELAY);
    }
}
