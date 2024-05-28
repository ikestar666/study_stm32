#include "exti.h"
#include "usart.h"

void KEY_EXTI_Init()
{

    //由于需要操作AFIO的EXTICR寄存器，选择中断线对应的端口，因此需要使能AFIO时钟
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
