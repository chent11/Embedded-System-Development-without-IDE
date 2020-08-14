#include "led.hh"
#include "core_init.h"
#include "arm_math.h"
#include "stm32f4xx_it.h"
#include <cmath>
#include <exception>

#define time_coe (15000000.f / 15000000.f)

void delay_ms_about(float time) {
    HAL_Delay((int)(time * time_coe));
}

// namespace __cxxabiv1 {
//     std::terminate_handler __terminate_handler = Error_Handler;
// }

int main(void) {
    core_init();

    volatile float32_t x = 0;
    volatile float32_t y = 0;
    volatile uint32_t tickstart = HAL_GetTick();
    volatile uint32_t elapse0, elapse1 = 0;

    LedRed* ledRed = new LedRed();
    LedBlue* ledBlue = new LedBlue();
    LedGreen* ledGreen = new LedGreen();

    while (1) {
        ledRed->on();
        tickstart = HAL_GetTick();
        for (int i = 0; i < 1000000; i++) {
            x = i / 782918.f;
            y = arm_sin_f32(x);
        }
        volatile double b = sin(1000000.0 / 782918.0);
        volatile double a = fabs(b - (double)y);

        elapse0 = HAL_GetTick() - tickstart;
        ledRed->off();
        tickstart = HAL_GetTick();
        for (int i = 0; i < 1000000; i++) {
            x = i / 782918.f;
            // sqrtf(x);
            y = sinf(x);
        }
        a = fabs(b - (double)y);
        elapse1 = HAL_GetTick() - tickstart;
    }
}
