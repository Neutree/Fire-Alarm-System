# ifndef _EXTI_H
# define _EXTI_H

#include "usart.h"
#include "nrf24l01.h"
#include "sensor.h"
#define EXTI_OFF() EXTI->IMR &=~(1<<11);  //屏蔽中断
#define EXTI_ON() {EXTI->PR |= (1<<11);  EXTI->IMR |= (1<<11);}/*先清挂起寄存器*///开放中断


void EXTI_Config(void);
void EXTI15_10_IRQHandler(void);
#endif
