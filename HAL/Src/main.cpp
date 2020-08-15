#include "led.hh"
#include "core_init.h"
#include "stm32f4xx_it.h"

void delay_ms_about(float time) {
    const float time_coe = 18000000.f / 15000000.f;
    HAL_Delay((int)(time * time_coe));
}

// namespace __cxxabiv1 {
//     std::terminate_handler __terminate_handler = Error_Handler;
// }

int main(void) {
    core_init();

    LedRed* ledRed = new LedRed();

    while (1) {
        ledRed->toggle();
        delay_ms_about(1000);
    }
}
