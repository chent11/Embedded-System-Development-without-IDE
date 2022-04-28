#include "GPIO.hh"

#include "stm32f4xx_hal.h"

constexpr static GPIO_TypeDef* hardwareAddressOfGPIOPort(const GPIO::Port port) {
    switch (port) {
        case GPIO::Port::A:
            return GPIOA;  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
        case GPIO::Port::B:
            return GPIOB;  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
        case GPIO::Port::C:
            return GPIOC;  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
        case GPIO::Port::D:
            return GPIOD;  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
        case GPIO::Port::E:
            return GPIOE;  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
        case GPIO::Port::F:
            return GPIOF;  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
        case GPIO::Port::G:
            return GPIOG;  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
        case GPIO::Port::H:
            return GPIOH;  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
        case GPIO::Port::I:
            return GPIOI;  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
        case GPIO::Port::J:
            return GPIOJ;  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
        case GPIO::Port::K:
            return GPIOK;  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
        default:
            assert_param(false);  // should NOT be executed. just for avoiding the possibility of forgetting new adding members in the future
            return nullptr;
    }
}
constexpr static uint16_t hardwarePinout(const GPIO::Pin pin) {
    switch (pin) {
        case GPIO::Pin::P0:
            return GPIO_PIN_0;
        case GPIO::Pin::P1:
            return GPIO_PIN_1;
        case GPIO::Pin::P2:
            return GPIO_PIN_2;
        case GPIO::Pin::P3:
            return GPIO_PIN_3;
        case GPIO::Pin::P4:
            return GPIO_PIN_4;
        case GPIO::Pin::P5:
            return GPIO_PIN_5;
        case GPIO::Pin::P6:
            return GPIO_PIN_6;
        case GPIO::Pin::P7:
            return GPIO_PIN_7;
        case GPIO::Pin::P8:
            return GPIO_PIN_8;
        case GPIO::Pin::P9:
            return GPIO_PIN_9;
        case GPIO::Pin::P10:
            return GPIO_PIN_10;
        case GPIO::Pin::P11:
            return GPIO_PIN_11;
        case GPIO::Pin::P12:
            return GPIO_PIN_12;
        case GPIO::Pin::P13:
            return GPIO_PIN_13;
        case GPIO::Pin::P14:
            return GPIO_PIN_14;
        case GPIO::Pin::P15:
            return GPIO_PIN_15;
        default:
            assert_param(false);  // should NOT be executed. just for avoiding the possibility of forgetting new adding members in the future
            return 0;
    }
}
constexpr static uint32_t hardwareGPIOMode(const GPIO::Mode mode) {
    switch (mode) {
        case GPIO::Mode::Input:
            return GPIO_MODE_INPUT;
        case GPIO::Mode::OutputPushPull:
            return GPIO_MODE_OUTPUT_PP;
        case GPIO::Mode::OutputOpenDrain:
            return GPIO_MODE_OUTPUT_OD;
        case GPIO::Mode::AlternatePushPull:
            return GPIO_MODE_AF_PP;
        case GPIO::Mode::AlternateOpenDrain:
            return GPIO_MODE_AF_OD;
        case GPIO::Mode::Analog:
            return GPIO_MODE_ANALOG;
        case GPIO::Mode::ExternalEventFallingEdge:
            return GPIO_MODE_EVT_FALLING;
        case GPIO::Mode::ExternalEventRisingEdge:
            return GPIO_MODE_EVT_RISING;
        case GPIO::Mode::ExternalEventRisingAndFallingEdge:
            return GPIO_MODE_EVT_RISING_FALLING;
        case GPIO::Mode::ExternalInterruptFallingEdge:
            return GPIO_MODE_IT_FALLING;
        case GPIO::Mode::ExternalInterruptRisingEdge:
            return GPIO_MODE_IT_RISING;
        case GPIO::Mode::ExternalInterruptRsingAndFallingEdge:
            return GPIO_MODE_IT_RISING_FALLING;
        default:
            assert_param(false);  // should NOT be executed. just for avoiding the possibility of forgetting new adding members in the future
            return 0;
    }
}
constexpr static uint32_t hardwareGPIOPull(const GPIO::Pull pull) {
    switch (pull) {
        case GPIO::Pull::PullUp:
            return GPIO_PULLUP;
        case GPIO::Pull::PullDown:
            return GPIO_PULLDOWN;
        case GPIO::Pull::NoPull:
            return GPIO_NOPULL;
        default:
            assert_param(false);  // should NOT be executed. just for avoiding the possibility of forgetting new adding members in the future
            return 0;
    }
}
constexpr static uint32_t hardwareGPIOSpeed(const GPIO::Speed speed) {
    switch (speed) {
        case GPIO::Speed::Low:
            return GPIO_SPEED_FREQ_LOW;
        case GPIO::Speed::Medium:
            return GPIO_SPEED_FREQ_MEDIUM;
        case GPIO::Speed::High:
            return GPIO_SPEED_FREQ_HIGH;
        case GPIO::Speed::VeryHigh:
            return GPIO_SPEED_FREQ_VERY_HIGH;
        default:
            assert_param(false);  // should NOT be executed. just for avoiding the possibility of forgetting new adding members in the future
            return 0;
    }
}

GPIO::GPIO(const Port port, const Pin pin, const Mode mode, const Pull pull, const Speed speed)
    : _port{port}, _pin{pin} {
    /* GPIO Ports Clock Enable */
    switch (_port) {
        case GPIO::Port::A:
            __HAL_RCC_GPIOA_CLK_ENABLE();  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
            break;
        case GPIO::Port::B:
            __HAL_RCC_GPIOB_CLK_ENABLE();  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
            break;
        case GPIO::Port::C:
            __HAL_RCC_GPIOC_CLK_ENABLE();  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
            break;
        case GPIO::Port::D:
            __HAL_RCC_GPIOD_CLK_ENABLE();  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
            break;
        case GPIO::Port::E:
            __HAL_RCC_GPIOE_CLK_ENABLE();  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
            break;
        case GPIO::Port::F:
            __HAL_RCC_GPIOF_CLK_ENABLE();  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
            break;
        case GPIO::Port::G:
            __HAL_RCC_GPIOG_CLK_ENABLE();  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
            break;
        case GPIO::Port::H:
            __HAL_RCC_GPIOH_CLK_ENABLE();  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
            break;
        case GPIO::Port::I:
            __HAL_RCC_GPIOI_CLK_ENABLE();  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
            break;
        case GPIO::Port::J:
            __HAL_RCC_GPIOJ_CLK_ENABLE();  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
            break;
        case GPIO::Port::K:
            __HAL_RCC_GPIOK_CLK_ENABLE();  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
            break;
        default:
            assert_param(false);  // should NOT be executed. just for avoiding the possibility of forgetting new adding members in the future
            return;
    }

    /* Configure GPIO pins */
    GPIO_InitTypeDef GPIO_InitStruct{};
    GPIO_InitStruct.Pin = hardwarePinout(_pin);
    GPIO_InitStruct.Mode = hardwareGPIOMode(mode);
    GPIO_InitStruct.Pull = hardwareGPIOPull(pull);
    GPIO_InitStruct.Speed = hardwareGPIOSpeed(speed);
    HAL_GPIO_Init(hardwareAddressOfGPIOPort(_port), &GPIO_InitStruct);
}

GPIO::~GPIO() {
    HAL_GPIO_DeInit(hardwareAddressOfGPIOPort(_port), hardwarePinout(_pin));
}

void GPIO::setHigh() const {
    HAL_GPIO_WritePin(hardwareAddressOfGPIOPort(_port), hardwarePinout(_pin), GPIO_PIN_RESET);
}

void GPIO::setLow() const {
    HAL_GPIO_WritePin(hardwareAddressOfGPIOPort(_port), hardwarePinout(_pin), GPIO_PIN_SET);
}

void GPIO::toggle() const {
    HAL_GPIO_TogglePin(hardwareAddressOfGPIOPort(_port), hardwarePinout(_pin));
}

GPIO::State GPIO::getState() const {
    GPIO_PinState state = HAL_GPIO_ReadPin(hardwareAddressOfGPIOPort(_port), hardwarePinout(_pin));
    return (state == GPIO_PIN_SET) ? State::High : State::Low;
}
