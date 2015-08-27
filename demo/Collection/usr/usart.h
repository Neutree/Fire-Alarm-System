#ifndef _USART__H
#define _USART__H
#include "stm32f10x.h"
#include "stdio.h"
void Usart_Configuration(void);			//配置Usart1 Tx->PA9 Rx->PA10
void Usart_NVIC_Configuration(void);//中断优先级设定
void USART1_Putc(uint8_t data);			//利用Usart1发送一个8位数据
void USART1_Puts(uint8_t * buffer);			//利用Uart4发送一个字符串

void USART1_Send_Enter(void);//利用串口1发送一换行符

__IO extern u8 USARTNum;//选择当前的usart  
#define MAX_QUEUE_LEN 51
#define elemtype char
typedef struct//队列结构体
{
	u16 MemFront;
	u16 MemRear;
	u16 MemLength;
	elemtype MemDataBuf[MAX_QUEUE_LEN];
}Queue_Mem_Struct, *Queue_Mem_Struct_p;

extern Queue_Mem_Struct USART1Buffer;//定义队列变量，用来缓存接受数据

//////////////////////
//队列操作函数声明
//////////////////////

//初始化队列
void InitQueue(Queue_Mem_Struct_p sq);
//求队列长度
u16 GetQueueSize(Queue_Mem_Struct_p sq);
//读队头元素
elemtype Head(Queue_Mem_Struct_p sq);
//出队
elemtype DelQueue(Queue_Mem_Struct_p sq);
//入队
void EnQueue(Queue_Mem_Struct_p sq,elemtype data);
//入队溢出错误
void ERROverflow(elemtype data);

#endif
