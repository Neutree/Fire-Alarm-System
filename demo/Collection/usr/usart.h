#ifndef _USART__H
#define _USART__H
#include "stm32f10x.h"
#include "stdio.h"
void Usart_Configuration(void);			//����Usart1 Tx->PA9 Rx->PA10
void Usart_NVIC_Configuration(void);//�ж����ȼ��趨
void USART1_Putc(uint8_t data);			//����Usart1����һ��8λ����
void USART1_Puts(uint8_t * buffer);			//����Uart4����һ���ַ���

void USART1_Send_Enter(void);//���ô���1����һ���з�

__IO extern u8 USARTNum;//ѡ��ǰ��usart  
#define MAX_QUEUE_LEN 51
#define elemtype char
typedef struct//���нṹ��
{
	u16 MemFront;
	u16 MemRear;
	u16 MemLength;
	elemtype MemDataBuf[MAX_QUEUE_LEN];
}Queue_Mem_Struct, *Queue_Mem_Struct_p;

extern Queue_Mem_Struct USART1Buffer;//������б��������������������

//////////////////////
//���в�����������
//////////////////////

//��ʼ������
void InitQueue(Queue_Mem_Struct_p sq);
//����г���
u16 GetQueueSize(Queue_Mem_Struct_p sq);
//����ͷԪ��
elemtype Head(Queue_Mem_Struct_p sq);
//����
elemtype DelQueue(Queue_Mem_Struct_p sq);
//���
void EnQueue(Queue_Mem_Struct_p sq,elemtype data);
//����������
void ERROverflow(elemtype data);

#endif
