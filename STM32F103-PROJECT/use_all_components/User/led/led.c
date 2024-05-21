#include "stm32f10x_gpio.h"
#include "led.h"


void LEDInit(void)
{
    GPIO_InitTypeDef LED_GREEN_GPIO_InitStruct;
    GPIO_InitTypeDef LED_BLUE_GPIO_InitStruct;
    GPIO_InitTypeDef LED_RED_GPIO_InitStruct;


    // GPIO clock cmd
    LED_RED_CLOCK_CMD(LED_RED_CLOCK, ENABLE);
    LED_GREEN_CLOCK_CMD(LED_GREEN_CLOCK, ENABLE);
    LED_BLUE_CLOCK_CMD(LED_BLUE_CLOCK, ENABLE);


    LED_BLUE_GPIO_InitStruct.GPIO_Pin = LED_BLUE_PIN;
    LED_BLUE_GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
    LED_BLUE_GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LED_BLUE_PORT, &LED_BLUE_GPIO_InitStruct);

    LED_RED_GPIO_InitStruct.GPIO_Pin = LED_RED_PIN;
    LED_RED_GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
    LED_RED_GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LED_RED_PORT, &LED_RED_GPIO_InitStruct);

    LED_GREEN_GPIO_InitStruct.GPIO_Pin = LED_GREEN_PIN;
    LED_GREEN_GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
    LED_GREEN_GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LED_GREEN_PORT, &LED_GREEN_GPIO_InitStruct);


}

void CloseLED(void)
{
    GPIO_SetBits(LED_BLUE_PORT, LED_BLUE_PIN);
    GPIO_SetBits(LED_RED_PORT, LED_RED_PIN);
    GPIO_SetBits(LED_GREEN_PORT, LED_GREEN_PIN);
}

void OpenBlueLED(void)
{
    CloseLED();
    GPIO_ResetBits(LED_BLUE_PORT, LED_BLUE_PIN);
}

void OpenRedLED(void)
{
    CloseLED();
    GPIO_ResetBits(LED_RED_PORT, LED_RED_PIN);
}

void OpenGreenLED(void)
{
    CloseLED();
    GPIO_ResetBits(LED_GREEN_PORT, LED_GREEN_PIN);
}

void OpenMagentaLED(void)
{
    CloseLED();
    GPIO_ResetBits(LED_RED_PORT, LED_RED_PIN);
    GPIO_ResetBits(LED_BLUE_PORT, LED_BLUE_PIN);
}

void OpenYellowLED(void)
{
    CloseLED();
    GPIO_ResetBits(LED_RED_PORT, LED_RED_PIN);
    GPIO_ResetBits(LED_GREEN_PORT, LED_GREEN_PIN);
}

void OpenCyanLED(void)
{
    CloseLED();
    GPIO_ResetBits(LED_BLUE_PORT, LED_BLUE_PIN);
    GPIO_ResetBits(LED_GREEN_PORT, LED_GREEN_PIN);
}

void OpenWhiteLED(void)
{
    GPIO_ResetBits(LED_RED_PORT, LED_RED_PIN);
    GPIO_ResetBits(LED_GREEN_PORT, LED_GREEN_PIN);
    GPIO_ResetBits(LED_BLUE_PORT, LED_BLUE_PIN);
}

