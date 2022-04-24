#ifndef LED_H_
#define LED_H_

#include "stm32f4xx_hal.h"

class Led {
  private:
    GPIO_TypeDef* const _gpioPort;
    const uint16_t _gpioPin;
    uint16_t _padding_reserved{0};

  public:
    Led(bool isOn, GPIO_TypeDef* port, uint16_t pin);
    ~Led();

    Led(const Led&) = delete;
    Led(Led&&) = delete;
    Led& operator=(const Led& rhs) = delete;
    Led& operator=(Led&& rhs) = delete;

    void on() const;
    void off() const;
    void toggle() const;
};

class LedRed : public Led {
  public:
    LedRed() : Led(false, GPIOB, GPIO_PIN_11) {}                   // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
    explicit LedRed(bool isOn) : Led(isOn, GPIOB, GPIO_PIN_11) {}  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
};

class LedBlue : public Led {
  public:
    LedBlue() : Led(false, GPIOB, GPIO_PIN_3) {}                   // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
    explicit LedBlue(bool isOn) : Led(isOn, GPIOB, GPIO_PIN_3) {}  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
};

class LedGreen : public Led {
  public:
    LedGreen() : Led(false, GPIOB, GPIO_PIN_1) {}                   // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
    explicit LedGreen(bool isOn) : Led(isOn, GPIOB, GPIO_PIN_1) {}  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
};

class GpioPD14_FmuCH6 : public Led {
  public:
    GpioPD14_FmuCH6() : Led(false, GPIOD, GPIO_PIN_14) {}                   // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
    explicit GpioPD14_FmuCH6(bool isOn) : Led(isOn, GPIOD, GPIO_PIN_14) {}  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
};
class GpioPE14_FmuCH1 : public Led {
  public:
    GpioPE14_FmuCH1() : Led(false, GPIOE, GPIO_PIN_14) {}                   // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
    explicit GpioPE14_FmuCH1(bool isOn) : Led(isOn, GPIOE, GPIO_PIN_14) {}  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
};
class GpioPD13_FmuCH5 : public Led {
  public:
    GpioPD13_FmuCH5() : Led(false, GPIOD, GPIO_PIN_13) {}                   // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
    explicit GpioPD13_FmuCH5(bool isOn) : Led(isOn, GPIOD, GPIO_PIN_13) {}  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
};

#endif
