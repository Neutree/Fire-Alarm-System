#ifndef _TIME_H
#define _TIME_H
#include "stm32f10x.h"
extern u16 time_s,time_100ms;

void TIM3_Init(void);
void TIM3_IRQHandler(void);

void initTimeCount(void);//初始化计时器
#endif
