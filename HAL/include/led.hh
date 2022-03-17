#ifndef __LED_H
#define __LED_H

#include "../drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal.h"

class Led {
protected:
    bool _isOn;
    GPIO_TypeDef* const _gpioPort;
    const uint16_t _gpioPin;
public:
    Led(bool isOn, GPIO_TypeDef* port, uint16_t pin);
    ~Led();

    void on(void);
    void off(void);
    void toggle(void);
};

class LedRed : public Led {
public:
    LedRed(void) : Led(false, GPIOD, GPIO_PIN_14) {}
    LedRed(bool isOn) : Led(isOn, GPIOD, GPIO_PIN_14) {}
};

class LedBlue : public Led {
public:
    LedBlue(void) : Led(false, GPIOD, GPIO_PIN_15) {}
    LedBlue(bool isOn) : Led(isOn, GPIOD, GPIO_PIN_15) {}
};

class LedGreen : public Led {
public:
    LedGreen(void) : Led(false, GPIOD, GPIO_PIN_12) {}
    LedGreen(bool isOn) : Led(isOn, GPIOD, GPIO_PIN_12) {}
};

#endif
