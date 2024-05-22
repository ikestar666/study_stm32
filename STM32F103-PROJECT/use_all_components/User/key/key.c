#include "key.h"

void KeyInit(void) {
  GPIO_InitTypeDef KEY1_GPIO_InitStruct;
  GPIO_InitTypeDef KEY2_GPIO_InitStruct;

  // clock cmd
  KEY_1_CLOCK_CMD(KEY_1_CLOCK_RCC, ENABLE);
  KEY_2_CLOCK_CMD(KEY_2_CLOCK_RCC, ENABLE);

  // gpio cmd
  KEY1_GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  KEY1_GPIO_InitStruct.GPIO_Pin = KEY_1_PIN;
  GPIO_Init(KEY_1_PORT, &KEY1_GPIO_InitStruct);

  KEY2_GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  KEY2_GPIO_InitStruct.GPIO_Pin = KEY_2_PIN;
  GPIO_Init(KEY_2_PORT, &KEY2_GPIO_InitStruct);
}

int IsKeyDown(GPIO_TypeDef *KEYx_PORT, uint16_t KEYx_PIN) {
  return GPIO_ReadInputDataBit(KEYx_PORT, KEYx_PIN) == KEY_DOWN;
}

int IsKeyUP(GPIO_TypeDef *KEYx_PORT, uint16_t KEYx_PIN) {
  return GPIO_ReadInputDataBit(KEYx_PORT, KEYx_PIN) == KEY_UP;
}
