# ifndef _EXTI_H
# define _EXTI_H

//#include "usart.h"
#include "nrf24l01.h"
#include "usart.h"
#include "LCD1602.h"
#define EXTI_OFF() EXTI->IMR &=~(1<<11);  //屏蔽中断
#define EXTI_ON() {EXTI->PR |= (1<<11);  EXTI->IMR |= (1<<11);}/*先清挂起寄存器*///开放中断

#define SENSOR_FIRE_NUMBER 1
#define SENSOR_MOV_NUMBER 3
#define SENSOR_LIGHT_NUMBER 2
#define SENSOR_GAS_NUMBER 0


extern u8 dataBuffer[6];
extern volatile u8 EXTI_flag;

void EXTI_Config(void);
void EXTI15_10_IRQHandler(void);
void Rcv_Message_Deal(void);
void Sensor_Status_Dis(void);
#endif
