#ifndef __LED_H_
#define __LED_H_

// macro defines
#define LED_RED_PORT GPIOB
#define LED_RED_PIN  GPIO_Pin_5
#define LED_RED_CLOCK RCC_APB2Periph_GPIOB
#define LED_RED_CLOCK_CMD RCC_APB2PeriphClockCmd

#define LED_BLUE_PORT GPIOB
#define LED_BLUE_PIN  GPIO_Pin_1
#define LED_BLUE_CLOCK RCC_APB2Periph_GPIOB
#define LED_BLUE_CLOCK_CMD RCC_APB2PeriphClockCmd


#define LED_GREEN_PORT GPIOB
#define LED_GREEN_PIN  GPIO_Pin_0
#define LED_GREEN_CLOCK RCC_APB2Periph_GPIOB
#define LED_GREEN_CLOCK_CMD RCC_APB2PeriphClockCmd



// functions
void LEDInit(void);
void CloseLED(void);

void OpenBlueLED(void);
void OpenRedLED(void);
void OpenGreenLED(void);

void OpenMagentaLED(void);
void OpenYellowLED(void);
void OpenCyanLED(void);

void OpenWhiteLED(void);

#endif
