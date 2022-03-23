#ifndef __LED_H
#define __LED_H

#include "stm32f4xx_hal.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
class Led {
protected:
    bool _isOn;
    GPIO_TypeDef* const _gpioPort;
    const uint16_t _gpioPin;
public:
    Led(bool isOn, GPIO_TypeDef* port, uint16_t pin);
    ~Led();

    void on();
    void off();
    void toggle();
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

class GpioPD14_FmuCH6 : public Led {
public:
    GpioPD14_FmuCH6(void) : Led(false, GPIOD, GPIO_PIN_14) {}
    GpioPD14_FmuCH6(bool isOn) : Led(isOn, GPIOD, GPIO_PIN_14) {}
};
class GpioPE14_FmuCH1 : public Led {
public:
    GpioPE14_FmuCH1(void) : Led(false, GPIOE, GPIO_PIN_14) {}
    GpioPE14_FmuCH1(bool isOn) : Led(isOn, GPIOE, GPIO_PIN_14) {}
};
class GpioPD13_FmuCH5 : public Led {
public:
    GpioPD13_FmuCH5(void) : Led(false, GPIOD, GPIO_PIN_13) {}
    GpioPD13_FmuCH5(bool isOn) : Led(isOn, GPIOD, GPIO_PIN_13) {}
};
#pragma GCC diagnostic pop
#endif
