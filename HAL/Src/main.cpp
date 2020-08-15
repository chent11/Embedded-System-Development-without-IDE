#include "led.hh"
#include "core_init.h"

void delay_ms(int time) {
    HAL_Delay(time);
}

int main() {
    core_init();

    auto* led = new LedGreen();

    while (true) {
        led->toggle();
        delay_ms(500);
    }
}
