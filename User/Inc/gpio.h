#ifndef GPIO_INIT_H
#define GPIO_INIT_H

#include "stm32f10x_gpio.h"
#include <stdint.h>

// 自定义错误码
typedef enum {
    GPIO_INIT_OK = 0,          // 初始化成功
    GPIO_INIT_ERROR_PORT,       // 无效的 GPIO 端口
    GPIO_INIT_ERROR_PIN,        // 无效的 GPIO 引脚
    GPIO_INIT_ERROR_MODE        // 无效的模式与参数组合
} GPIO_InitStatus;

typedef enum
{ GPIO_Mode_AIN = 0x0,   //模拟输入
  GPIO_Mode_IN_FLOATING = 0x04,  // 输入浮空
  GPIO_Mode_IPD = 0x28,  //输入下拉
  GPIO_Mode_IPU = 0x48,  //输入上拉
  GPIO_Mode_Out_OD = 0x14,   // 开漏输出
  GPIO_Mode_Out_PP = 0x10,  // 推挽输出
  GPIO_Mode_AF_OD = 0x1C,  // 复用功能开漏
  GPIO_Mode_AF_PP = 0x18   // 复用功能推挽
}GPIOMode_TypeDef;
/**
 * @brief 初始化 GPIO 引脚
 * @param GPIOx: GPIO 端口（如 GPIOA, GPIOB）
 * @param GPIO_Pin: GPIO 引脚（如 GPIO_Pin_0）
 * @param mode: 自定义 GPIO 模式
 * @param speed: 速度（仅在输出模式下有效）
 * @retval GPIO_InitStatus 错误码
 */
GPIO_InitStatus GPIO_Init_Pin(
    GPIO_TypeDef* GPIOx, 
    uint16_t GPIO_Pin, 
    GPIOMode_TypeDef mode, 
    GPIOSpeed_TypeDef speed
);

#endif // GPIO_INIT_H