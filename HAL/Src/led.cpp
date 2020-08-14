#include "led.hh"

Led::Led(bool isOn, GPIO_TypeDef* port, uint16_t pin) : _gpioPort(port), _gpioPin(pin) {
    _isOn = isOn;
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(_gpioPort, _gpioPin, _isOn ? GPIO_PIN_RESET : GPIO_PIN_SET);

    /*Configure GPIO pins : ledRed_Pin ledGreen_Pin ledBlue_Pin */
    GPIO_InitStruct.Pin = _gpioPin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(_gpioPort, &GPIO_InitStruct);
}

Led::~Led() {
    off();
    HAL_GPIO_DeInit(_gpioPort, _gpioPin);
}

void Led::on(void) {
    HAL_GPIO_WritePin(_gpioPort, _gpioPin, GPIO_PIN_RESET);
    _isOn = true;
}

void Led::off(void) {
    HAL_GPIO_WritePin(_gpioPort, _gpioPin, GPIO_PIN_SET);
    _isOn = false;
}

void Led::toggle(void) {
    _isOn ? this->off() : this->on();
}

