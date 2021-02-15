#ifndef __LED_H
#define __LED_H

#include "stm32f4xx_hal.h"

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
    LedRed(void) : Led(false, GPIOB, GPIO_PIN_11) {}
    LedRed(bool isOn) : Led(isOn, GPIOB, GPIO_PIN_11) {}
};

class LedBlue : public Led {
public:
    LedBlue(void) : Led(false, GPIOB, GPIO_PIN_3) {}
    LedBlue(bool isOn) : Led(isOn, GPIOB, GPIO_PIN_3) {}
};

class LedGreen : public Led {
public:
    LedGreen(void) : Led(false, GPIOB, GPIO_PIN_1) {}
    LedGreen(bool isOn) : Led(isOn, GPIOB, GPIO_PIN_1) {}
};

#endif
