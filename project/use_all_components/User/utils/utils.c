#include "utils.h"
#include "core_cm3.h"

void Delay(uint32_t ticks)
{
    while (ticks-- != 0)
        ;
}


void Delay_us(uint32_t us)
{
    uint32_t i = 0;
    SysTick_Config(72);
    for (i = 0; i < us; i++)
    {
        while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
    }
    SysTick ->CTRL &= ~SysTick_CTRL_COUNTFLAG_Msk;
}

void Delay_ms(uint32_t ms)
{
    uint32_t i = 0;
    SysTick_Config(72000);
    for (i = 0; i < ms; i++)
    {
        while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
    }
    SysTick ->CTRL &= ~SysTick_CTRL_COUNTFLAG_Msk;
}