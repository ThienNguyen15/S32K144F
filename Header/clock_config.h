#ifndef CLOCK_CONFIG_H
#define CLOCK_CONFIG_H

#include "main.h"

void Crystal_Config1(void);
void Crystal_Config2(void);
void Crystal_Config3(void);
void Crystal_Config4(void);
void Crystal_Config5(void);
void Timer(volatile unsigned int counter);
void SysTick_Config(unsigned int value);
void SysTick_Timer(unsigned int count);

#endif /* CLOCK_CONFIG_H */
