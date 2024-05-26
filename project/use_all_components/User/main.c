#include "key.h"
#include "led.h"
#include "stm32f10x.h" // 相当于51单片机中的  #include <reg51.h>
#include "utils.h"
#include "exti.h"

// 定义全局变量
volatile int ErrorKeyPressed = 0;

void TestLED(void)
{
    uint32_t time = 200;
    OpenRedLED();
    Delay_ms(time);

    OpenGreenLED();
    Delay_ms(time);

    OpenBlueLED();
    Delay_ms(time);

    OpenCyanLED();
    Delay_ms(time);

    OpenMagentaLED();
    Delay_ms(time);

    OpenYellowLED();
    Delay_ms(time);

    OpenWhiteLED();
    Delay_ms(time);
    CloseAllLED();
    Delay_ms(time);
    OpenWhiteLED();
    Delay_ms(time);
    CloseAllLED();
    Delay_ms(time);
    OpenWhiteLED();
    Delay_ms(time);
    CloseAllLED();
}

int PressKey(GPIO_TypeDef *KEYx_PORT, uint16_t KEYx_PIN,
             GPIO_TypeDef *LEDx_PORT, uint16_t LEDx_PIN)
{
    int WaitTime = 60;
    int TimePassed = 0;
    while (IsKeyUP(KEYx_PORT, KEYx_PIN))
    {
        Blink(LED_BLUE_PORT, LED_BLUE_PIN, 1, 1000);
        TimePassed += 1;
        // 超过调用单次最大等待时常
        if (TimePassed >= WaitTime)
        {
            Blink(LED_RED_PORT, LED_RED_PIN, 6, 1000);
            return 0;
        }
        // 如果错误的按键被按下，则需要重新输入密码
        if (ErrorKeyPressed == 1)
        {
            OpenRedLED();
            Delay_ms(2000);
            CloseAllLED();
            Delay_ms(1000);
            ErrorKeyPressed = 0;
            return 0;
        }
    }
    while (IsKeyDown(KEYx_PORT, KEYx_PIN))
    {
        Blink(LED_BLUE_PORT, LED_BLUE_PIN, 1, 200);
        TimePassed += 1;
        if (TimePassed >= WaitTime)
        {
            Blink(LED_RED_PORT, LED_RED_PIN, 6, 200);
            return 0;
        }
    }
    CloseAllLED();
    return 1;
}

void ShowMusicLed(uint16_t *music, uint16_t music_len)
{
    int i;
    for (i = 0; i < music_len; i++)
    {
        if (music[i] == 1)
        {
            OpenRedLED();
        }
        else if (music[i] == 2)
        {
            OpenGreenLED();
        }
        else if (music[i] == 3)
        {
            OpenBlueLED();
        }
        else if (music[i] == 4)
        {
            OpenMagentaLED();
        }
        else if (music[i] == 5)
        {
            OpenYellowLED();
        }
        else if (music[i] == 6)
        {
            OpenCyanLED();
        }
        else if (music[i] == 7)
        {
            OpenWhiteLED();
        }
        else
        {
            CloseAllLED();
        }
        Delay_ms(100);
    }
    CloseAllLED();
}

int PassCodeCheck()
{

    uint16_t PassCode[6] = {1, 1, 2, 1, 2, 2};
    GPIO_TypeDef *Key_xPort;
    uint16_t Key_xPin;

    uint16_t MaxTryTime = 3;
    int i;
    for (i = 0; i < 6; i++)
    {
        // 需要设置监听哪个按键的中断
        if (PassCode[i] == 1)
        {
            Key_xPort = KEY_1_PORT;
            Key_xPin = KEY_1_PIN;
            KEY1_EXTI_Cmd(DISABLE);
            KEY2_EXTI_Cmd(ENABLE);
        }
        else
        {
            Key_xPort = KEY_2_PORT;
            Key_xPin = KEY_2_PIN;
            KEY1_EXTI_Cmd(ENABLE);
            KEY2_EXTI_Cmd(DISABLE);
        }

        // 等待按到正确的按钮，或者超时
        if (PressKey(Key_xPort, Key_xPin, LED_RED_PORT, LED_RED_PIN))
        {
            OpenWhiteLED();
            Delay_ms(2000);
            CloseAllLED();
            Delay_ms(1000);
        }
        else
        {
            // 超时没有输入或输入位错误的按钮，则重置密码输入开关
            i -= 1;
            if (--MaxTryTime <= 0) // 输入错误次数过多，则会暂时锁机
            {
                OpenMagentaLED();
                Delay_ms(200);

                CloseAllLED();
                Delay_ms(200);

                OpenMagentaLED();
                Delay_ms(200);

                CloseAllLED();
                Delay_ms(200);

                OpenMagentaLED();
                Delay_ms(200);
                CloseAllLED();
                Delay_ms(200);

                MaxTryTime = 3;
                i = -1;

            }
        }
    }

    Blink(LED_GREEN_PORT, LED_GREEN_PIN, 6, 100);

    return 1;
}

int main(void)
{

    uint16_t res = 0;
    uint16_t music[] =
    {
        // 前奏
        6, 6, 6, 6, 5, 3, 3, 2, 3, 2, 1, 6, 6, 6, 6, 5, 3, 3, 2, 3, 2, 1,

        // 主歌
        6, 5, 6, 1, 3, 3, 2, 3, 2, 1, 6, 5, 6, 1, 3, 3, 2, 3, 2, 1, 6, 5, 6, 1, 3,
        3, 2, 3, 2, 1, 6, 5, 6, 1, 3, 3, 2, 3, 2, 1,

        // 副歌
        6, 5, 6, 1, 3, 2, 1, 2, 3, 2, 1, 6, 5, 6, 1, 3, 2, 1, 2, 3, 2, 1, 6, 5, 6,
        1, 3, 2, 1, 2, 3, 2, 1, 6, 5, 6, 1, 3, 2, 1, 2, 3, 2, 1,

        // 过渡
        5, 5, 6, 5, 3, 2, 1, 2, 3, 2, 1, 5, 5, 6, 5, 3, 2, 1, 2, 3, 2, 1,

        // 主歌
        6, 5, 6, 1, 3, 3, 2, 3, 2, 1, 6, 5, 6, 1, 3, 3, 2, 3, 2, 1, 6, 5, 6, 1, 3,
        3, 2, 3, 2, 1, 6, 5, 6, 1, 3, 3, 2, 3, 2, 1,

        // 副歌
        6, 5, 6, 1, 3, 2, 1, 2, 3, 2, 1, 6, 5, 6, 1, 3, 2, 1, 2, 3, 2, 1, 6, 5, 6,
        1, 3, 2, 1, 2, 3, 2, 1, 6, 5, 6, 1, 3, 2, 1, 2, 3, 2, 1
    };

    KEY_NVIC_Config();
    KEY_EXTI_Init();
    LEDInit();
    KeyInit();

    TestLED();

    res = PassCodeCheck();

    if (res == 1)
    {
        ShowMusicLed(music, 121);
        CloseAllLED();
    }

    Blink(LED_GREEN_PORT, LED_GREEN_PIN, 3, 500);
}
