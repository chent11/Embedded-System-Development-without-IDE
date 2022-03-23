#include "led.hh"

Led::Led(bool isOn, GPIO_TypeDef* port, uint16_t pin) : _isOn(isOn), _gpioPort(port), _gpioPin(pin) {

    GPIO_InitTypeDef GPIO_InitStruct{ 0 };

    /* GPIO Ports Clock Enable */
    if (_gpioPort == GPIOA) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    } else if (_gpioPort == GPIOB) {
        __HAL_RCC_GPIOB_CLK_ENABLE();
    } else if (_gpioPort == GPIOC) {
        __HAL_RCC_GPIOC_CLK_ENABLE();
    } else if (_gpioPort == GPIOD) {
        __HAL_RCC_GPIOD_CLK_ENABLE();
    } else if (_gpioPort == GPIOE) {
        __HAL_RCC_GPIOE_CLK_ENABLE();
    }

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(_gpioPort, _gpioPin, _isOn ? GPIO_PIN_RESET : GPIO_PIN_SET);

    /*Configure GPIO pins : ledRed_Pin ledGreen_Pin ledBlue_Pin */
    GPIO_InitStruct.Pin = _gpioPin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(_gpioPort, &GPIO_InitStruct);
}

Led::~Led() {
    off();
    HAL_GPIO_DeInit(_gpioPort, _gpioPin);
}

void Led::on() {
    HAL_GPIO_WritePin(_gpioPort, _gpioPin, GPIO_PIN_RESET);
    _isOn = true;
}

void Led::off() {
    HAL_GPIO_WritePin(_gpioPort, _gpioPin, GPIO_PIN_SET);
    _isOn = false;
}

void Led::toggle() {
    _isOn ? this->off() : this->on();
}

