#ifndef __BSP_EXTI_H
#define __BSP_EXTI_H

#include "stm32f10x_exti.h"
#include "stm32f10x.h"

#define KEY1_EXTI_LINE     EXTI_Line0
#define KEY2_EXTI_LINE       EXTI_Line13


void KEY_NVIC_Config(void);
void KEY_EXTI_Init(void);
void KEY1_EXTI_Cmd(FunctionalState cmd);
void KEY2_EXTI_Cmd(FunctionalState cmd);


#endif /* __BSP_EXIT_H */
