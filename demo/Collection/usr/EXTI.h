# ifndef _EXTI_H
# define _EXTI_H

#include "usart.h"
#include "nrf24l01.h"
#include "sensor.h"
#define EXTI_OFF() EXTI->IMR &=~(1<<11);  //�����ж�
#define EXTI_ON() {EXTI->PR |= (1<<11);  EXTI->IMR |= (1<<11);}/*�������Ĵ���*///�����ж�


void EXTI_Config(void);
void EXTI15_10_IRQHandler(void);
#endif
