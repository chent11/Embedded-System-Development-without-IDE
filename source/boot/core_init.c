#include "core_init.h"

#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_cortex.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_usart.h"
#include "stm32f4xx_ll_utils.h"

#define PLLN  180
#define CLOCK (PLLN * 1000000)

static void pllClock_config(void) {
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_5);
    while (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_5) {
    }
    LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
    LL_PWR_EnableOverDriveMode();
    LL_RCC_HSE_Enable();

    /* Wait till HSE is ready */
    while (LL_RCC_HSE_IsReady() != 1) {
    }
    LL_RCC_HSE_EnableCSS();
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_12, PLLN, LL_RCC_PLLP_DIV_2);
    LL_RCC_PLL_Enable();

    /* Wait till PLL is ready */
    while (LL_RCC_PLL_IsReady() != 1) {
    }
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

    /* Wait till System clock is ready */
    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) {
    }
    LL_Init1msTick(CLOCK);
    LL_SetSystemCoreClock(CLOCK);
    LL_RCC_SetTIMPrescaler(LL_RCC_TIM_PRESCALER_TWICE);
}

void delay_ms(uint32_t time) {
    LL_mDelay(time);
}

const uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
const uint8_t APBPrescTable[8] = {0, 0, 0, 0, 1, 2, 3, 4};
static void MX_UART7_Init(void) {
    /* Peripheral clock enable */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_UART7);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOE);
    /**UART7 GPIO Configuration
    PE7   ------> UART7_RX
    PE8   ------> UART7_TX
    */
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = LL_GPIO_PIN_7 | LL_GPIO_PIN_8;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_8;
    LL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    LL_USART_InitTypeDef USART_InitStruct = {0};
    USART_InitStruct.BaudRate = 115200;
    USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
    USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
    USART_InitStruct.Parity = LL_USART_PARITY_NONE;
    USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
    USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
    USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
    LL_USART_Init(UART7, &USART_InitStruct);
    LL_USART_ConfigAsyncMode(UART7);
    LL_USART_Enable(UART7);
}

void core_init(void) {
    // LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

    // Enable External Clock GPIO Clock
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOH);
    // Enable Debug GPIO Clock
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);

    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_CCMDATARAM);

    /* Configure Flash prefetch, Instruction cache, Data cache */
    LL_FLASH_EnableInstCache();
    LL_FLASH_EnableDataCache();
    LL_FLASH_EnablePrefetch();

    /* Set Interrupt Group Priority */
    // NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

    // const uint32_t prioritygroup   = NVIC_GetPriorityGrouping();
    // const uint32_t preemptPriority = 0;
    // const uint32_t subPriority     = 0;
    // NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(prioritygroup, preemptPriority, subPriority));

    pllClock_config();

    // /* Use systick as time base source and configure 1ms tick (default clock after Reset is HSI) */
    // // LL_Init1msTick(SystemCoreClock);
    // /* Configure the SysTick to have interrupt in 1ms time base */
    // SysTick->LOAD = (uint32_t)((SystemCoreClock / 1000) - 1UL) + 18750; /* set reload register */
    // SysTick->VAL  = 0UL;                                       /* Load the SysTick Counter Value */
    // SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
    //                 SysTick_CTRL_ENABLE_Msk; /* Enable the Systick Timer */
    MX_UART7_Init();
}

void send_char_blocking(char ch) {
    while (!LL_USART_IsActiveFlag_TXE(UART7)) {
        // wait until USART Transmit Data Register is empty
    }
    LL_USART_TransmitData8(UART7, ch);
}

char get_char_blocking(void) {
    while (!LL_USART_IsActiveFlag_RXNE(UART7)) {
        // Check if the USART Read Data Register Not Empty Flag is set or not
    }
    return LL_USART_ReceiveData8(UART7);
}
