#include "nvic.h"
#include "usart.h"

void MyNVIC_Config(void)
{
    // 配置中断优先级
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    KEY_NVIC_Config();
    USART_NVIC_Config();
}


void USART_NVIC_Config(void)
{
    // NVIC 使能按键中断
    NVIC_InitTypeDef USART_NVIC_InitStruct;

    // 按键中断
    USART_NVIC_InitStruct.NVIC_IRQChannel = USARTx_IRQ;
    USART_NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    USART_NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    USART_NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&USART_NVIC_InitStruct);
}

void KEY_NVIC_Config(void)
{
    // NVIC 使能按键中断
    NVIC_InitTypeDef KEY1_NVIC_InitStruct;
    NVIC_InitTypeDef KEY2_NVIC_InitStruct;

    // 按键中断
    KEY1_NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
    KEY1_NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    KEY1_NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
    KEY1_NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&KEY1_NVIC_InitStruct);

    KEY2_NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
    KEY2_NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    KEY2_NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;
    KEY2_NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&KEY2_NVIC_InitStruct);
}
