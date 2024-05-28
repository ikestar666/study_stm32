#include "exti.h"
#include "key.h"
#include "led.h"
#include "nvic.h"
#include "stm32f10x.h" // �൱��51��Ƭ���е�  #include <reg51.h>
#include "usart.h"
#include "utils.h"
#include <stdio.h>

// ����ȫ�ֱ���
volatile int KeyPressed = 0;

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

/*
int PassCodeCheck()
{

    uint16_t PassCode[6] = {1, 1, 2, 1, 2, 2};
    uint16_t MaxTryTime = 3;
    int i;
    for (i = 0; i < 6; i++)
    {
        while (KeyPressed == 0)
        {
            Blink(LED_BLUE_PORT, LED_BLUE_PIN, 1, 1000);
        }

        // ������ȷ
        if (PassCode[i] == KeyPressed)
        {
            OpenWhiteLED();
            Delay_ms(500);
            CloseAllLED();
            Delay_ms(500);
            // ��ӡ����
            if (KeyPressed == 1)
            {
                USART_SentString(USARTx, "1 ");
            }
            else
            {
                USART_SentString(USARTx, "2 ");
            }
        }
        else
        {
            OpenYellowLED();
            Delay_ms(500);
            CloseAllLED();
            Delay_ms(500);
            i -= 1;
            if (--MaxTryTime <= 0) // �������������࣬�����ʱ����
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
                USART_SentString(USARTx, "\n");

            }
        }
        KeyPressed = 0;
    }

    Blink(LED_GREEN_PORT, LED_GREEN_PIN, 6, 100);

    return 1;
}
*/

// ͨ�����ܴ�������ʵ��
int PassCodeCheck()
{
    char *PassCode = "112122";
    char inputPassCode[7] = {'0', '0', '0', '0', '0', '0', '\0'};
    char *str = inputPassCode;
    int i = 0;
    printf("��ʼ����Ϊ");
    printf(str);
    printf("\n");
    while (1)
    {
        i = 0;
        USART_ReceiveString(USARTx, str);
        printf("�����������Ϊ");
        printf(str);
        while (i < 6 && *(str + i) == PassCode[i])
        {
            i++;
        }
        if (i == 6)
        {
            printf("������ȷ\n");
            Blink(LED_GREEN_PORT, LED_GREEN_PIN, 3, 200);
            break;
        }
        else
        {
            printf("�������������\n");
            Blink(LED_RED_PORT, LED_RED_PIN, 3, 200);
        }
    }
    return 1;
}


int main(void)
{
    uint16_t res = 0;
    uint16_t music[] =
    {
        // ǰ��
        6, 6, 6, 6, 5, 3, 3, 2, 3, 2, 1, 6, 6, 6, 6, 5, 3, 3, 2, 3, 2, 1,

        // ����
        6, 5, 6, 1, 3, 3, 2, 3, 2, 1, 6, 5, 6, 1, 3, 3, 2, 3, 2, 1, 6, 5, 6, 1, 3,
        3, 2, 3, 2, 1, 6, 5, 6, 1, 3, 3, 2, 3, 2, 1,

        // ����
        6, 5, 6, 1, 3, 2, 1, 2, 3, 2, 1, 6, 5, 6, 1, 3, 2, 1, 2, 3, 2, 1, 6, 5, 6,
        1, 3, 2, 1, 2, 3, 2, 1, 6, 5, 6, 1, 3, 2, 1, 2, 3, 2, 1,

        // ����
        5, 5, 6, 5, 3, 2, 1, 2, 3, 2, 1, 5, 5, 6, 5, 3, 2, 1, 2, 3, 2, 1,

        // ����
        6, 5, 6, 1, 3, 3, 2, 3, 2, 1, 6, 5, 6, 1, 3, 3, 2, 3, 2, 1, 6, 5, 6, 1, 3,
        3, 2, 3, 2, 1, 6, 5, 6, 1, 3, 3, 2, 3, 2, 1,

        // ����
        6, 5, 6, 1, 3, 2, 1, 2, 3, 2, 1, 6, 5, 6, 1, 3, 2, 1, 2, 3, 2, 1, 6, 5, 6,
        1, 3, 2, 1, 2, 3, 2, 1, 6, 5, 6, 1, 3, 2, 1, 2, 3, 2, 1
    };

    // NVIC�ж�����
    MyNVIC_Config();

    // EXTI �ж�����
    KEY_EXTI_Init();
    KEY1_EXTI_Cmd(ENABLE);
    KEY2_EXTI_Cmd(ENABLE);


    // ��ʼ������
    MyUSART_Init();
    USART_Cmd(USARTx, ENABLE);
    USART_SentString(USARTx, "USART��ʼ���ɹ�\n");

    LEDInit();
    USART_SentString(USARTx, "LED��ʼ���ɹ�\n");

    KeyInit();
    USART_SentString(USARTx, "KEY��ʼ���ɹ�\n");

    USART_SentString(USARTx, "��ʼ����LED����\n");
    TestLED();
    USART_SentString(USARTx, "LED��������\n");

    USART_SentString(USARTx, "������������룺\n");

    // USART �ж�����(�ݲ�)
    //USART_TI_Cmd();
    res = PassCodeCheck();

    if (res == 1)
    {
        USART_SentString(USARTx, "����������ȷ���ѿ���\n");

        USART_SentString(USARTx, "�����͵ƹ���\n");
        ShowMusicLed(music, 121);
        CloseAllLED();
        USART_SentString(USARTx, "�ƹ���չʾ���\n");
    }

    USART_SentString(USARTx, "�����̵���˸���κ�ػ�\n");
    Blink(LED_GREEN_PORT, LED_GREEN_PIN, 3, 500);
}
