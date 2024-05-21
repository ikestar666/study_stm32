#include "stm32f10x.h"   // �൱��51��Ƭ���е�  #include <reg51.h>
#include "led.h"

void SoftDelay(void)
{
    int i = 0;
    while (i < 5000000)
    {
        i += 1;
    }
}

void TestLED(void)
{
    LEDInit();

    OpenRedLED();
    SoftDelay();

    OpenGreenLED();
    SoftDelay();

    OpenBlueLED();
    SoftDelay();

    OpenCyanLED();
    SoftDelay();

    OpenMagentaLED();
    SoftDelay();

    OpenYellowLED();
    SoftDelay();

    OpenWhiteLED();
    SoftDelay();
    CloseLED();
    SoftDelay();
    OpenWhiteLED();
    SoftDelay();
    CloseLED();
    SoftDelay();
    OpenWhiteLED();
    SoftDelay();
    CloseLED();
}

int main(void)
{
    TestLED();
}




