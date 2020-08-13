#include "led.h"
#include "core_init.h"
#include "arm_math.h"

#define time_coe (15000000.f / 15000000.f)

void delay_ms_about(float time) {
    HAL_Delay((int)(time * time_coe));
}

int main(void) {
    core_init();
    
    volatile float32_t x = 0;
    volatile float32_t y = 0;
    volatile uint32_t tickstart = HAL_GetTick();
    volatile uint32_t elapse0, elapse1 = 0;
    while (1) {
        ledRed(ON);
        tickstart = HAL_GetTick();
        for (volatile int i = 0; i < 1000000; i++) {
          x = i/782918.f;
          y = arm_sin_f32(x);
        }
        volatile double b = sin(1000000.0/782918.0);
        volatile double a = fabs(b - (double)y);
        elapse0 = HAL_GetTick() - tickstart;
        ledRed(OFF);
        tickstart = HAL_GetTick();
        for (volatile int i = 0; i < 1000000; i++) {
          x = i/782918.f;
          // sqrtf(x);
          y = sinf(x);
        }
        a = fabs(b - (double)y);
        elapse1 = HAL_GetTick() - tickstart;
    }
}
