#include "stm32f4xx_hal.h"
#include "stm32f4xx_it.h"
#include "arm_math.h"

#define ledRed_Pin GPIO_PIN_11
#define ledRed_GPIO_Port GPIOB
#define ledGreen_Pin GPIO_PIN_1
#define ledGreen_GPIO_Port GPIOB
#define ledBlue_Pin GPIO_PIN_3
#define ledBlue_GPIO_Port GPIOB

#define time_coe (15000000.f / 15000000.f)

void SystemClock_Config(void);
void MX_GPIO_Init(void);

void delay_ms_about(float time) {
    HAL_Delay((int)(time * time_coe));
}

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    volatile float32_t x = 0;
    volatile float32_t y = 0;
    volatile uint32_t tickstart = HAL_GetTick();
    volatile uint32_t elapse = 0;
    while (1) {
        
        HAL_GPIO_WritePin(GPIOB, ledGreen_Pin, GPIO_PIN_RESET);
        tickstart = HAL_GetTick();
        for (volatile int i = 0; i < 1000000; i++) {
          // y = arm_sin_f32(x);
          x = i/782918.f;
          y = arm_sin_f32 (x);
        }
        volatile double b = sin(1000000.0/782918.0);
        volatile double a = fabs(b - (double)y);
        elapse = HAL_GetTick() - tickstart;
        HAL_GPIO_WritePin(GPIOB, ledGreen_Pin, GPIO_PIN_SET);
        // HAL_GPIO_WritePin(GPIOB, ledRed_Pin, GPIO_PIN_RESET);
        tickstart = HAL_GetTick();
        for (volatile int i = 0; i < 1000000; i++) {
          x = i/782918.f;
          // sqrtf(x);
          y = sinf(x);
        }
        a = fabs(b - (double)y);
        elapse = HAL_GetTick() - tickstart;
    }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
  */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 12;
    RCC_OscInitStruct.PLL.PLLN = 180;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }
    /** Activate the Over-Drive mode
  */
    if (HAL_PWREx_EnableOverDrive() != HAL_OK) {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB buses clocks
  */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
        Error_Handler();
    }
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_TIM;
    PeriphClkInitStruct.TIMPresSelection = RCC_TIMPRES_ACTIVATED;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
        Error_Handler();
    }
    /** Enables the Clock Security System
  */
    HAL_RCC_EnableCSS();
}

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

void HAL_MspInit(void) {
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line) {
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif
