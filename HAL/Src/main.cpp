#include <vector>

#include "led.hh"
#include "core_init.h"

using std::vector;

void delay_ms(int time) {
    HAL_Delay(time);
}

#define SIZE 200

int main() {
    core_init();

    auto* led = new LedGreen();
    vector<vector<int>> a(SIZE, vector<int>(SIZE, 0));
    while (true) {
        led->on();
        for (int ii = 0; ii < 100; ii++) {
            for (int i = 0; i < SIZE; i++) {
                for (int j = 0; j < SIZE; j++) {
                    a[j][i] = i + j;
                }
            }
        }
        led->off();
        for (int ii = 0; ii < 100; ii++) { 
            for (int i = 0; i < SIZE; i++) {
                for (int j = 0; j < SIZE; j++) {
                    a[i][j] = i + j;
                }
            }
        }
    }
}
