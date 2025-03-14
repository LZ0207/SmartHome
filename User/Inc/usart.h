#ifndef _USART_H
#define _USART_H
#include <stm32f10x.h>
// 定义使用USART1
#define USE_USART1
// 波特率
#define USART_BAUDRATE 115200

#ifdef USE_USART1
// USART外设实例
#define USART_INSTANCE USART1
#define USART_IRQN USART1_IRQn

// 端口配置
#define USART_GPIO_PORT GPIOA

// 引脚配置
#define USART_TX_PIN GPIO_Pin_9  // 发送引脚
#define USART_RX_PIN GPIO_Pin_10 // 接收引脚

// 时钟配置
#define USART_GPIO_PORT_CLK RCC_APB2Periph_GPIOA // GPIO端口时钟
#define USART_PERIPH_CLK RCC_APB2Periph_USART1   // USART外设时钟
#endif
// 函数声明
void USART_ConfigAndInit(void);
void USART_Transmit(USART_TypeDef *usartx, const uint8_t *data, uint16_t data_size);
typedef void (*USART_RxCallback)(uint8_t data);
void USART_RegisterRxCallback(USART_RxCallback callback);
#endif
