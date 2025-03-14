#include "gpio.h"
#include "stm32f10x_rcc.h"

// GPIO 端口与时钟的映射表
typedef struct
{
    GPIO_TypeDef *GPIOx;
    uint32_t RCC_APB2Periph;
} GPIO_ClockMap;

static const GPIO_ClockMap clockMap[] = {
    {GPIOA, RCC_APB2Periph_GPIOA},
    {GPIOB, RCC_APB2Periph_GPIOB},
    {GPIOC, RCC_APB2Periph_GPIOC},
    {GPIOD, RCC_APB2Periph_GPIOD},
    {GPIOE, RCC_APB2Periph_GPIOE},
    {GPIOF, RCC_APB2Periph_GPIOF},
    {GPIOG, RCC_APB2Periph_GPIOG}};

GPIO_InitStatus GPIO_Init_Pin(
    GPIO_TypeDef *GPIOx,
    uint16_t GPIO_Pin,
    GPIOMode_TypeDef mode,
    GPIOSpeed_TypeDef speed)
{
    // 参数合法性检查
    if (!IS_GPIO_ALL_PERIPH(GPIOx))
    {
        return GPIO_INIT_ERROR_PORT;
    }
    if (!IS_GPIO_PIN(GPIO_Pin))
    {
        return GPIO_INIT_ERROR_PIN;
    }
    if (!IS_GPIO_MODE(mode))
    {
        return GPIO_INIT_ERROR_MODE;
    }

    // 模式与参数兼容性检查（问题3）
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin;

    // 根据模式设置参数
    switch (mode)
    {
    case GPIO_Mode_Out_PP:
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
        GPIO_InitStruct.GPIO_Pin = GPIO_Pin;
        break;
    default:
        return GPIO_INIT_ERROR_MODE;
    }

    uint32_t rcc_apb2_periph = 0;
    for (uint8_t i = 0; i < sizeof(clockMap) / sizeof(clockMap[0]); i++)
    {
        if (GPIOx == clockMap[i].GPIOx)
        {
            rcc_apb2_periph = clockMap[i].RCC_APB2Periph;
            break;
        }
    }
    if (rcc_apb2_periph == 0)
    {
        return GPIO_INIT_ERROR_PORT; // 未找到对应端口
    }
    // 使能时钟
    RCC_APB2PeriphClockCmd(rcc_apb2_periph, ENABLE);

    // 初始化 GPIO
    GPIO_Init(GPIOx, &GPIO_InitStruct);
    return GPIO_INIT_OK;
}

void GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    GPIOx->BRR ^= GPIO_Pin;
}
