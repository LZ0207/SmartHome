// delay.c
#include "delay.h"

__IO uint32_t ulTicks = 0;
static uint8_t delay_initialized_flag = 0;
static float us_per_mini_tick = 0;

void Delay_Init(void)
{
    delay_initialized_flag = 1;

    RCC_ClocksTypeDef clockinfo = {0};
    uint32_t tmp;

    SysTick->CTRL &= ~SysTick_CTRL_ENABLE; // 禁止SYSTICK

    ulTicks = 0;

    RCC_GetClocksFreq(&clockinfo);

    SysTick->CTRL |= SysTick_CTRL_TICKINT; // 开启中断

    // 设置中断优先级为0
    SCB->SHP[7] = 0;

    // 设置自动重装值以保证1ms的时钟
    tmp = clockinfo.HCLK_Frequency / 1000;
    if (tmp > 0x00FFFFFF)
    {
        tmp = tmp / 8;
        SysTick->CTRL &= ~SysTick_CTRL_CLKSOURCE;
    }
    else
    {
        SysTick->CTRL |= SysTick_CTRL_CLKSOURCE;
    }
    SysTick->LOAD = tmp - 1;

    SysTick->CTRL |= SysTick_CTRL_ENABLE;

    us_per_mini_tick = 1000.0 / ((SysTick->LOAD & 0x00FFFFFF) + 1);
}

void Delay(uint32_t ms)
{
#if DELAY_CHECK_INIT
    if (!delay_initialized_flag)
    {
        Delay_Init();
    }
#endif

    uint64_t expiredTime = ulTicks + ms;
    while (ulTicks < expiredTime) {}
}

uint32_t GetTick(void)
{
#if DELAY_CHECK_INIT
    if (!delay_initialized_flag)
    {
        Delay_Init();
    }
#endif

    return ulTicks;
}

uint64_t GetUs(void)
{
#if DELAY_CHECK_INIT
    if (!delay_initialized_flag)
    {
        Delay_Init();
    }
#endif

    uint64_t tick;
    uint32_t mini_tick;

    SysTick->CTRL &= ~SysTick_CTRL_COUNTFLAG; // 清除COUNTFLAG

    tick = ulTicks; // 读取毫秒值
    mini_tick = SysTick->VAL; // 读取SYSTICK的值

    while (SysTick->CTRL & SysTick_CTRL_COUNTFLAG)
    {
        mini_tick = SysTick->VAL;
        tick = ulTicks;
    }

    tick *= 1000; // 毫秒部分乘以1000
    tick += (uint32_t)((SysTick->LOAD - mini_tick) * us_per_mini_tick); // 小数部分折算成微秒

    return tick;
}

void DelayUs(uint32_t us)
{
#if DELAY_CHECK_INIT
    if (!delay_initialized_flag)
    {
        Delay_Init();
    }
#endif

    uint64_t expiredTime = GetUs() + us + 1;
    while (GetUs() < expiredTime);
}
