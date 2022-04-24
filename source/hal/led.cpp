#include "led.hh"

Led::Led(bool isOn, GPIO_TypeDef* port, uint16_t pin) : _gpioPort(port), _gpioPin(pin) {
    /* GPIO Ports Clock Enable */
    if (_gpioPort == GPIOA) {          // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
        __HAL_RCC_GPIOA_CLK_ENABLE();  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
    } else if (_gpioPort == GPIOB) {   // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
        __HAL_RCC_GPIOB_CLK_ENABLE();  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
    } else if (_gpioPort == GPIOC) {   // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
        __HAL_RCC_GPIOC_CLK_ENABLE();  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
    } else if (_gpioPort == GPIOD) {   // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
        __HAL_RCC_GPIOD_CLK_ENABLE();  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
    } else if (_gpioPort == GPIOE) {   // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
        __HAL_RCC_GPIOE_CLK_ENABLE();  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
    } else {
        /* Code should not be executed in here */
        while (true) {
        }
    }

    /* Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(_gpioPort, _gpioPin, isOn ? GPIO_PIN_RESET : GPIO_PIN_SET);

    /* Configure GPIO pins : ledRed_Pin ledGreen_Pin ledBlue_Pin */
    GPIO_InitTypeDef GPIO_InitStruct{};
    GPIO_InitStruct.Pin = _gpioPin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(_gpioPort, &GPIO_InitStruct);
}

Led::~Led() {
    off();
    HAL_GPIO_DeInit(_gpioPort, _gpioPin);
}

void Led::on() const {
    HAL_GPIO_WritePin(_gpioPort, _gpioPin, GPIO_PIN_RESET);
}

void Led::off() const {
    HAL_GPIO_WritePin(_gpioPort, _gpioPin, GPIO_PIN_SET);
}

void Led::toggle() const {
    HAL_GPIO_TogglePin(_gpioPort, _gpioPin);
}
