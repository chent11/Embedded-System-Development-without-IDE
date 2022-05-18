#include "core_devices/GPIO.hh"
#include "core_init.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_system.h"

constexpr static GPIO_TypeDef* hardwareAddressPort(const GPIO::Port port) {
    switch (port) {
        case GPIO::Port::A:
            return GPIOA;  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast,
                           // performance-no-int-to-ptr)
        case GPIO::Port::B:
            return GPIOB;  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast,
                           // performance-no-int-to-ptr)
        case GPIO::Port::C:
            return GPIOC;  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast,
                           // performance-no-int-to-ptr)
        case GPIO::Port::D:
            return GPIOD;  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast,
                           // performance-no-int-to-ptr)
        case GPIO::Port::E:
            return GPIOE;  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast,
                           // performance-no-int-to-ptr)
        case GPIO::Port::F:
            return GPIOF;  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast,
                           // performance-no-int-to-ptr)
        case GPIO::Port::G:
            return GPIOG;  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast,
                           // performance-no-int-to-ptr)
        case GPIO::Port::H:
            return GPIOH;  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast,
                           // performance-no-int-to-ptr)
        case GPIO::Port::I:
            return GPIOI;  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast,
                           // performance-no-int-to-ptr)
        case GPIO::Port::J:
            return GPIOJ;  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast,
                           // performance-no-int-to-ptr)
        case GPIO::Port::K:
            return GPIOK;  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast,
                           // performance-no-int-to-ptr)
        default:
            assert_param(false);  // should NOT be executed. just for avoiding
                                  // the possibility of forgetting new adding
                                  // members in the future
            return nullptr;
    }
}
constexpr static uint16_t hardwareAddressPinout(const GPIO::Pin pin) {
    switch (pin) {
        case GPIO::Pin::P0:
            return LL_GPIO_PIN_0;
        case GPIO::Pin::P1:
            return LL_GPIO_PIN_1;
        case GPIO::Pin::P2:
            return LL_GPIO_PIN_2;
        case GPIO::Pin::P3:
            return LL_GPIO_PIN_3;
        case GPIO::Pin::P4:
            return LL_GPIO_PIN_4;
        case GPIO::Pin::P5:
            return LL_GPIO_PIN_5;
        case GPIO::Pin::P6:
            return LL_GPIO_PIN_6;
        case GPIO::Pin::P7:
            return LL_GPIO_PIN_7;
        case GPIO::Pin::P8:
            return LL_GPIO_PIN_8;
        case GPIO::Pin::P9:
            return LL_GPIO_PIN_9;
        case GPIO::Pin::P10:
            return LL_GPIO_PIN_10;
        case GPIO::Pin::P11:
            return LL_GPIO_PIN_11;
        case GPIO::Pin::P12:
            return LL_GPIO_PIN_12;
        case GPIO::Pin::P13:
            return LL_GPIO_PIN_13;
        case GPIO::Pin::P14:
            return LL_GPIO_PIN_14;
        case GPIO::Pin::P15:
            return LL_GPIO_PIN_15;
        default:
            assert_param(false);  // should NOT be executed. just for avoiding
                                  // the possibility of forgetting new adding
                                  // members in the future
            return 0;
    }
}
constexpr static uint32_t hardwareAddressMode(const GPIO::Mode mode) {
    switch (mode) {
        case GPIO::Mode::Input:
            return LL_GPIO_MODE_INPUT;
        case GPIO::Mode::Output:
            return LL_GPIO_MODE_OUTPUT;
        case GPIO::Mode::Analog:
            return LL_GPIO_MODE_ANALOG;
        case GPIO::Mode::Alternate:
            return LL_GPIO_MODE_ALTERNATE;
        default:
            assert_param(false);  // should NOT be executed. just for avoiding
                                  // the possibility of forgetting new adding
                                  // members in the future
            return 0;
    }
}
constexpr static uint32_t hardwareAddressOutputType(const GPIO::OutputType outputType) {
    switch (outputType) {
        case GPIO::OutputType::None:
            return 0;
        case GPIO::OutputType::PushPull:
            return LL_GPIO_OUTPUT_PUSHPULL;
        case GPIO::OutputType::OpenDrain:
            return LL_GPIO_OUTPUT_OPENDRAIN;
        default:
            assert_param(false);  // should NOT be executed. just for avoiding
                                  // the possibility of forgetting new adding
                                  // members in the future
            return 0;
    }
}
constexpr static uint32_t hardwareAddressAlternateFunction(const GPIO::AlternateFunction af) {
    switch (af) {
        case GPIO::AlternateFunction::None:
            return 0;
        case GPIO::AlternateFunction::AF0:
            return LL_GPIO_AF_0;
        case GPIO::AlternateFunction::AF1:
            return LL_GPIO_AF_1;
        case GPIO::AlternateFunction::AF2:
            return LL_GPIO_AF_2;
        case GPIO::AlternateFunction::AF3:
            return LL_GPIO_AF_3;
        case GPIO::AlternateFunction::AF4:
            return LL_GPIO_AF_4;
        case GPIO::AlternateFunction::AF5:
            return LL_GPIO_AF_5;
        case GPIO::AlternateFunction::AF6:
            return LL_GPIO_AF_6;
        case GPIO::AlternateFunction::AF7:
            return LL_GPIO_AF_7;
        case GPIO::AlternateFunction::AF8:
            return LL_GPIO_AF_8;
        case GPIO::AlternateFunction::AF9:
            return LL_GPIO_AF_9;
        case GPIO::AlternateFunction::AF10:
            return LL_GPIO_AF_10;
        case GPIO::AlternateFunction::AF11:
            return LL_GPIO_AF_11;
        case GPIO::AlternateFunction::AF12:
            return LL_GPIO_AF_12;
        case GPIO::AlternateFunction::AF13:
            return LL_GPIO_AF_13;
        case GPIO::AlternateFunction::AF14:
            return LL_GPIO_AF_14;
        case GPIO::AlternateFunction::AF15:
            return LL_GPIO_AF_15;
        default:
            assert_param(false);  // should NOT be executed. just for avoiding
                                  // the possibility of forgetting new adding
                                  // members in the future
            return 0;
    }
    return 0;
}
constexpr static uint32_t hardwareAddressPull(const GPIO::Pull pull) {
    switch (pull) {
        case GPIO::Pull::PullUp:
            return LL_GPIO_PULL_UP;
        case GPIO::Pull::PullDown:
            return LL_GPIO_PULL_DOWN;
        case GPIO::Pull::NoPull:
            return LL_GPIO_PULL_NO;
        default:
            assert_param(false);  // should NOT be executed. just for avoiding
                                  // the possibility of forgetting new adding
                                  // members in the future
            return 0;
    }
}
constexpr static uint32_t hardwareAddressSpeed(const GPIO::Speed speed) {
    switch (speed) {
        case GPIO::Speed::None:
            return 0;
        case GPIO::Speed::Low:
            return LL_GPIO_SPEED_FREQ_LOW;
        case GPIO::Speed::Medium:
            return LL_GPIO_SPEED_FREQ_MEDIUM;
        case GPIO::Speed::High:
            return LL_GPIO_SPEED_FREQ_HIGH;
        case GPIO::Speed::VeryHigh:
            return LL_GPIO_SPEED_FREQ_VERY_HIGH;
        default:
            assert_param(false);  // should NOT be executed. just for avoiding
                                  // the possibility of forgetting new adding
                                  // members in the future
            return 0;
    }
}

static void enableClock(const GPIO::Port port) {
    /* GPIO Ports Clock Enable */
    switch (port) {
        case GPIO::Port::A:
            if (!static_cast<bool>(LL_AHB1_GRP1_IsEnabledClock(LL_AHB1_GRP1_PERIPH_GPIOA))) {
                LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast,
                                                                      // performance-no-int-to-ptr)
            }
            break;
        case GPIO::Port::B:
            if (!static_cast<bool>(LL_AHB1_GRP1_IsEnabledClock(LL_AHB1_GRP1_PERIPH_GPIOB))) {
                LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast,
                                                                      // performance-no-int-to-ptr)
            }
            break;
        case GPIO::Port::C:
            if (!static_cast<bool>(LL_AHB1_GRP1_IsEnabledClock(LL_AHB1_GRP1_PERIPH_GPIOC))) {
                LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast,
                                                                      // performance-no-int-to-ptr)
            }
            break;
        case GPIO::Port::D:
            if (!static_cast<bool>(LL_AHB1_GRP1_IsEnabledClock(LL_AHB1_GRP1_PERIPH_GPIOD))) {
                LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast,
                                                                      // performance-no-int-to-ptr)
            }
            break;
        case GPIO::Port::E:
            if (!static_cast<bool>(LL_AHB1_GRP1_IsEnabledClock(LL_AHB1_GRP1_PERIPH_GPIOE))) {
                LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOE);  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast,
                                                                      // performance-no-int-to-ptr)
            }
            break;
        case GPIO::Port::F:
            if (!static_cast<bool>(LL_AHB1_GRP1_IsEnabledClock(LL_AHB1_GRP1_PERIPH_GPIOF))) {
                LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOF);  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast,
                                                                      // performance-no-int-to-ptr)
            }
            break;
        case GPIO::Port::G:
            if (!static_cast<bool>(LL_AHB1_GRP1_IsEnabledClock(LL_AHB1_GRP1_PERIPH_GPIOG))) {
                LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOG);  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast,
                                                                      // performance-no-int-to-ptr)
            }
            break;
        case GPIO::Port::H:
            if (!static_cast<bool>(LL_AHB1_GRP1_IsEnabledClock(LL_AHB1_GRP1_PERIPH_GPIOH))) {
                LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOH);  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast,
                                                                      // performance-no-int-to-ptr)
            }
            break;
        case GPIO::Port::I:
            if (!static_cast<bool>(LL_AHB1_GRP1_IsEnabledClock(LL_AHB1_GRP1_PERIPH_GPIOI))) {
                LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOI);  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast,
                                                                      // performance-no-int-to-ptr)
            }
            break;
        case GPIO::Port::J:
            if (!static_cast<bool>(LL_AHB1_GRP1_IsEnabledClock(LL_AHB1_GRP1_PERIPH_GPIOJ))) {
                LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOJ);  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast,
                                                                      // performance-no-int-to-ptr)
            }
            break;
        case GPIO::Port::K:
            if (!static_cast<bool>(LL_AHB1_GRP1_IsEnabledClock(LL_AHB1_GRP1_PERIPH_GPIOK))) {
                LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOK);  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast,
                                                                      // performance-no-int-to-ptr)
            }
            break;
        default:
            assert_param(false);  // should NOT be executed. just for avoiding
                                  // the possibility of forgetting new adding
                                  // members in the future
            return;
    }
}

namespace GPIO {

void Hardware::init(const Port port, const Pin pin, const DefaultState state, const Mode mode, const Pull pull,
                    OutputType outputType, const Speed speed, const AlternateFunction alternateFunction) {
    enableClock(port);

    /* Configure compensation cell for high speed IO */
    if (speed >= Speed::High) {
        if (!static_cast<bool>(LL_SYSCFG_IsActiveFlag_CMPCR())) {
            // if compensation cell is disabled
            LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
            LL_SYSCFG_EnableCompensationCell();
        }
    }

    LL_GPIO_InitTypeDef gpioInitStruct{};
    gpioInitStruct.Pin = hardwareAddressPinout(pin);
    gpioInitStruct.Mode = hardwareAddressMode(mode);
    gpioInitStruct.OutputType = hardwareAddressOutputType(outputType);
    gpioInitStruct.Pull = hardwareAddressPull(pull);
    gpioInitStruct.Speed = hardwareAddressSpeed(speed);
    gpioInitStruct.Alternate = hardwareAddressAlternateFunction(alternateFunction);
    LL_GPIO_Init(hardwareAddressPort(port), &gpioInitStruct);
    (state == State::High) ? setHigh(port, pin) : setLow(port, pin);
}

void Hardware::setHigh(const Port port, const Pin pin) {
    LL_GPIO_SetOutputPin(hardwareAddressPort(port), hardwareAddressPinout(pin));
}

void Hardware::setLow(const Port port, const Pin pin) {
    LL_GPIO_ResetOutputPin(hardwareAddressPort(port), hardwareAddressPinout(pin));
}

void Hardware::toggle(const Port port, const Pin pin) {
    LL_GPIO_TogglePin(hardwareAddressPort(port), hardwareAddressPinout(pin));
}

State Hardware::getState(const Port port, const Pin pin) {
    uint32_t state = LL_GPIO_IsInputPinSet(hardwareAddressPort(port), hardwareAddressPinout(pin));
    return (state == 1) ? State::High : State::Low;
}

};  // namespace GPIO
