#include "exti.h"

void KEY_NVIC_Config()
{
    // NVIC 使能按键中断
    NVIC_InitTypeDef KEY1_NVIC_InitStruct;
    NVIC_InitTypeDef KEY2_NVIC_InitStruct;

    // 配置中断优先级
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);


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

void KEY_EXTI_Init()
{

    //由于需要操作AFIO的EXTICR寄存器，选择中断线对应的端口，因此需要施恩使能AFIO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13);
}


void KEY1_EXTI_Cmd(FunctionalState cmd)
{
    //clock cmd EXTI中断依赖GPIO的时钟配置

    // init config
    EXTI_InitTypeDef EXTI_InitStruct;
    EXTI_InitStruct.EXTI_Line = EXTI_Line0;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStruct.EXTI_LineCmd = cmd;

    EXTI_Init(&EXTI_InitStruct);

}

void KEY2_EXTI_Cmd(FunctionalState cmd)
{
    //clock cmd EXTI中断依赖GPIO的时钟配置

    // init config
    EXTI_InitTypeDef EXTI_InitStruct;
    EXTI_InitStruct.EXTI_Line = EXTI_Line13;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStruct.EXTI_LineCmd = cmd;

    EXTI_Init(&EXTI_InitStruct);

}
