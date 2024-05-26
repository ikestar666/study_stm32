#ifndef __KEY_H_
#define __KEY_H_

#include "stm32f10x_gpio.h"

#define KEY_1_PORT GPIOA
#define KEY_1_PIN GPIO_Pin_0
#define KEY_1_CLOCK_RCC RCC_APB2Periph_GPIOA
#define KEY_1_CLOCK_CMD RCC_APB2PeriphClockCmd

#define KEY_2_PORT GPIOC
#define KEY_2_PIN GPIO_Pin_13
#define KEY_2_CLOCK_RCC RCC_APB2Periph_GPIOC
#define KEY_2_CLOCK_CMD RCC_APB2PeriphClockCmd

#define KEY_UP 0
#define KEY_DOWN 1

void KeyInit(void);
int IsKeyDown(GPIO_TypeDef *KEYx_PORT, uint16_t KEYx_PIN);
int IsKeyUP(GPIO_TypeDef *KEYx_PORT, uint16_t KEYx_PIN);

#endif
