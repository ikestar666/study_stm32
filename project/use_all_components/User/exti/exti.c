#include "exti.h"
#include "usart.h"

void KEY_EXTI_Init()
{

    //������Ҫ����AFIO��EXTICR�Ĵ�����ѡ���ж��߶�Ӧ�Ķ˿ڣ������Ҫʹ��AFIOʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13);
}


void KEY1_EXTI_Cmd(FunctionalState cmd)
{
    //clock cmd EXTI�ж�����GPIO��ʱ������

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
    //clock cmd EXTI�ж�����GPIO��ʱ������

    // init config
    EXTI_InitTypeDef EXTI_InitStruct;
    EXTI_InitStruct.EXTI_Line = EXTI_Line13;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStruct.EXTI_LineCmd = cmd;

    EXTI_Init(&EXTI_InitStruct);

}
