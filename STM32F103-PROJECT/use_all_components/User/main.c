#include "stm32f10x.h"   // 相当于51单片机中的  #include <reg51.h>
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




