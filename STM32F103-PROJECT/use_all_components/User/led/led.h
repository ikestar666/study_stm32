#ifndef __LED_H_
#define __LED_H_

#include "stm32f10x_gpio.h"

// macro defines
#define LED_RED_PORT GPIOB
#define LED_RED_PIN GPIO_Pin_5
#define LED_RED_CLOCK RCC_APB2Periph_GPIOB
#define LED_RED_CLOCK_CMD RCC_APB2PeriphClockCmd

#define LED_BLUE_PORT GPIOB
#define LED_BLUE_PIN GPIO_Pin_1
#define LED_BLUE_CLOCK RCC_APB2Periph_GPIOB
#define LED_BLUE_CLOCK_CMD RCC_APB2PeriphClockCmd

#define LED_GREEN_PORT GPIOB
#define LED_GREEN_PIN GPIO_Pin_0
#define LED_GREEN_CLOCK RCC_APB2Periph_GPIOB
#define LED_GREEN_CLOCK_CMD RCC_APB2PeriphClockCmd

// functions
void LEDInit(void);
void OpenLED(GPIO_TypeDef *LEDx_PORT, uint16_t LEDx_PIN);
void CloseLED(GPIO_TypeDef *LEDx_PORT, uint16_t LEDx_PIN);

void Blink(GPIO_TypeDef *LEDx_PORT, uint16_t LEDx_PIN, uint16_t times,  uint32_t DelayTime);

void OpenBlueLED(void);
void OpenRedLED(void);
void OpenGreenLED(void);

void OpenMagentaLED(void);
void OpenYellowLED(void);
void OpenCyanLED(void);

void OpenWhiteLED(void);
void CloseAllLED(void);

#endif
