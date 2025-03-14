#include <usart.h>
#include <stdio.h>
/**
 * @brief USART初始化
 */
void USART_ConfigAndInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};   // GPIO初始化结构体
    USART_InitTypeDef USART_InitStruct = {0}; // USART初始化结构体
    NVIC_InitTypeDef NVIC_InitStruct = {0};   // NVIC初始化结构体

    // 配置Tx引脚
    GPIO_InitStruct.GPIO_Pin = USART_TX_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;   // 配置复用推挽模式
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; // 最大速率
    // 初始化
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 配置Rx引脚
    GPIO_InitStruct.GPIO_Pin = USART_RX_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;

    // USART参数配置
    USART_InitStruct.USART_BaudRate = USART_BAUDRATE;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

    // 使能时钟
    RCC_APB2PeriphClockCmd(USART_GPIO_PORT_CLK | USART_PERIPH_CLK, ENABLE);

    // 初始化
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    USART_Init(USART_INSTANCE, &USART_InitStruct);

    // 使能USART接收中断
    USART_ITConfig(USART_INSTANCE, USART_IT_RXNE, ENABLE);

    // NVIC配置
    NVIC_InitStruct.NVIC_IRQChannel = USART_IRQN;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    USART_Cmd(USART1, ENABLE);
}

/**
 * @brief  通过USART发送数组数据（只支持数组格式的数据）
 * @param  usartx: USART外设实例（如USART1, USART2等）
 * @param  data: 要发送的字符串数据（以'\0'结尾）
 * @param  data_size:发送数组的长度，可用sizeof获得
 * @retval None
 */
void USART_Transmit(USART_TypeDef *usart_instance, const uint8_t *data, uint16_t data_size)
{
    // 参数有效性检查
    if (usart_instance == NULL || data == NULL || !IS_USART_ALL_PERIPH(usart_instance))
        return;
    uint16_t i;
    // 发送数据
    for (i = 0; i < data_size; i++)
    {
        // 等待发送寄存器空
        while (USART_GetFlagStatus(usart_instance, USART_FLAG_TXE) == RESET)
            ;
        // 发送数据
        USART_SendData(usart_instance, (uint8_t)data[i]);
    }

    // 等待传输完成
    while (USART_GetFlagStatus(usart_instance, USART_FLAG_TC) == RESET)
        ;
}
/**
 * @brief  通过USART发送单字节数据
 * @param  usartx: USART外设实例（如USART1, USART2等）
 * @param  data: 要发送的字符串数据（以'\0'结尾）
 * @retval None
 */
void USART_Transmit_Byte(USART_TypeDef *usart_instance, const uint8_t data)
{
    if (usart_instance == NULL || data == NULL || !IS_USART_ALL_PERIPH(usart_instance))
        return;
    while (USART_GetFlagStatus(usart_instance, USART_FLAG_TXE) == RESET)
        ;
    USART_SendData(usart_instance, (uint8_t)data);
    while (USART_GetFlagStatus(usart_instance, USART_FLAG_TC) == RESET)
        ;
}

// Rx中断回调函数
static USART_RxCallback usart_rx_callback = NULL;
/**
 * @brief USART中断回调注册函数
 */
void USART_RegisterRxCallback(USART_RxCallback callback)
{
    usart_rx_callback = callback;
}

// USART1中断服务程序
void USART1_IRQHandler(void)
{
    // 清除中断标志
    USART_ClearITPendingBit(USART_INSTANCE, USART_IT_RXNE);
    if (USART_GetITStatus(USART_INSTANCE, USART_IT_RXNE) != RESET)
    {
        // 读取接收到的数据
        uint8_t data = USART_ReceiveData(USART_INSTANCE);
        // 使用回调函数处理数据
        if (usart_rx_callback != NULL)
        {
            usart_rx_callback(data);
        }
    }
}
