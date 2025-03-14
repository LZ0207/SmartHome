// delay.h
#ifndef DELAY_H
#define DELAY_H

#include "stm32f10x.h"

#define DELAY_CHECK_INIT 1 // 1: 启用初始化检查；0: 禁用初始化检查

void Delay_Init(void);
void Delay(uint32_t ms);
uint32_t GetTick(void);
uint64_t GetUs(void);
void DelayUs(uint32_t us);

#endif // DELAY_H
