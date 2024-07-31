#ifndef __UTILS_H_
#define __UTILS_H_

#include "stm32f10x.h"

#define countOfArr(arg)     (sizeof(arg)/sizeof(*arg))

void Delay(uint32_t tics);

void Delay_us(uint32_t us);

void Delay_ms(uint32_t ms);

#endif
