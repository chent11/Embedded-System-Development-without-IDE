#include <array>
#include <vector>
#include <algorithm>
#include "stm32f4xx_hal.h"
#include "led.hh"
#include "core_init.h"
#include "arm_math.h"

namespace {
    const int LENGTH = 1000;
    void delay_ms(int time) {
        HAL_Delay(time);
    }
    std::vector<float> vec(LENGTH, 0);
    std::array<float, LENGTH> arr = { 0 };
} // anonymous

int main() {
    core_init();

    Led* led = new LedGreen();
    Led* ledRed = new LedRed();
    Led* signal1 = new GpioPE14_FmuCH1();
    // Led* signal2 = new GpioPD14_FmuCH6();

    for (uint32_t i = 0; i < 10; i++) {
        ledRed->toggle();
        delay_ms(100);
    }

    float volatile testVar = 0.0F;
    while (true) {
        // Test 1
        led->on();
        signal1->on();
        for (auto& j : vec) {
            j = arm_cos_f32(static_cast<float>(rand()));
        }

        for (auto const& i : vec) {
            testVar += i;
        }

        // Test 2
        led->off();
        signal1->off();
        std::generate(arr.begin(), arr.end(), []() mutable { return arm_cos_f32(static_cast<float>(rand())); });

        for (auto const& i : arr) {
            testVar += i;
        }
    }
}
