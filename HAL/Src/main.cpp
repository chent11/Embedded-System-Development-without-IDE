#include "led.hh"
#include "core_init.h"
#include <vector>

void delay_ms(int time) {
    HAL_Delay(time);
}

#define SIZE 100

int main() {
    core_init();

    auto* led = new LedGreen();
    const int LENGTH = 100;
    const int LOOP = 300000;

    static int __attribute__((used)) arr[LENGTH]{ 0 };
    static std::vector<int> __attribute__((used)) vec(LENGTH, 0);
    static int __attribute__((used)) count { 0 };

    while (true) {
        led->on();
        count = HAL_GetTick();
        for (int i = 0; i < LOOP; i++)
            for (int j = 0; j < LENGTH; j++) {
                arr[j] = j * 12;
            }
        static int __attribute__((used)) a = HAL_GetTick() - count;

        led->off();
        count = HAL_GetTick();
        for (int i = 0; i < LOOP; i++)
            for (int j = 0; j < vec.size(); j++) {
                vec[j] = j * 12;
            }
        a = HAL_GetTick() - count;
    }

}
