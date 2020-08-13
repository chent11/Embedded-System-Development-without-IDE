#include <stdbool.h>

#include "stm32f4xx_hal.h"
#include "led.h"

#define ledRed_Pin          GPIO_PIN_11
#define ledRed_GPIO_Port    GPIOB
#define ledGreen_Pin        GPIO_PIN_1
#define ledGreen_GPIO_Port  GPIOB
#define ledBlue_Pin         GPIO_PIN_3
#define ledBlue_GPIO_Port   GPIOB

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
void MX_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, ledRed_Pin | ledGreen_Pin | ledBlue_Pin, GPIO_PIN_SET);

    /*Configure GPIO pins : ledRed_Pin ledGreen_Pin ledBlue_Pin */
    GPIO_InitStruct.Pin = ledRed_Pin | ledGreen_Pin | ledBlue_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void ledInit(void) {
    MX_GPIO_Init();
}

void ledRed(ledState isOn) {
    if (isOn) {
        HAL_GPIO_WritePin(ledRed_GPIO_Port, ledRed_Pin, GPIO_PIN_RESET);
    } else {
        HAL_GPIO_WritePin(ledRed_GPIO_Port, ledRed_Pin, GPIO_PIN_SET);
    }
}
