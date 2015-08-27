#include "usart.h"


__IO u8 USARTNum=1;

Queue_Mem_Struct USART1Buffer;//定义队列变量，用来缓存接受数据

//////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////队列///////////////////////////
///////////////循环队列，头指针不用来存放数据////////////////
///////////////////////////////////////////////////////////
//入队溢出错误

//初始化队列
void InitQueue(Queue_Mem_Struct_p sq)
{
	u16 i;
	sq->MemFront=0;
	sq->MemRear=0;
	sq->MemLength=0;
	for(i=0;i<MAX_QUEUE_LEN;++i)//!!!!!!初始化必须需要，而且不能为0,否则会出现问题
		USART1Buffer.MemDataBuf[i]='0';
}
//求队列长度
u16 GetQueueSize(Queue_Mem_Struct_p sq)
{
	return (MAX_QUEUE_LEN+sq->MemRear-sq->MemFront)%MAX_QUEUE_LEN;
}
//读队头元素
elemtype Head(Queue_Mem_Struct_p sq)
{
	if(sq->MemFront==sq->MemRear)
		return 0;
	else
		return sq->MemDataBuf[(sq->MemFront+1)%MAX_QUEUE_LEN];
}	

//出队
elemtype DelQueue(Queue_Mem_Struct_p sq)
{
	if(sq->MemFront==sq->MemRear)
	{
		return 0;
	}
	else
	{
		sq->MemFront=(sq->MemFront+1)%MAX_QUEUE_LEN;
		--sq->MemLength;
		return (sq->MemDataBuf[sq->MemFront]);
	}
}

//入队
void EnQueue(Queue_Mem_Struct_p sq,elemtype data)
{
	if((sq->MemRear+1)%MAX_QUEUE_LEN==sq->MemFront)//队满，溢出
	{
		ERROverflow(data);
	}
	else{
		sq->MemRear=(sq->MemRear+1)%MAX_QUEUE_LEN;
		sq->MemDataBuf[sq->MemRear]=data;
		++sq->MemLength;
	}
}

//入队溢出错误
void ERROverflow(elemtype data)
{
	DelQueue(&USART1Buffer);//出队，舍弃前面的数据
	EnQueue(&USART1Buffer,data);//新数据入队
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void Usart_Configuration(void)			//配置Usart1 Tx->PA2 Rx->PA3
{
	 GPIO_InitTypeDef GPIO_InitStructure; //GPIO库函数结构体
	 USART_InitTypeDef USART_InitStructure;//USART库函数结构体
	 USART_ClockInitTypeDef USART_ClockInitStructure;
	 //使能串口1，GPIOA，AFIO总线
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO|RCC_APB2Periph_USART1,ENABLE);
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

	 /* Configure USART1 Tx (PA9) as alternate function push-pull */
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_2;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//PA9时钟速度50MHz
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用输出
	 GPIO_Init(GPIOA, &GPIO_InitStructure);
	 /* Configure USART1 Rx (PA10) as input floating */
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_3;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//上拉输入
	 GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	 
	 USART_InitStructure.USART_BaudRate =115200; //波特率115200
	 USART_InitStructure.USART_WordLength = USART_WordLength_8b; //8位数据
	 USART_InitStructure.USART_StopBits = USART_StopBits_1; //1个停止位
	 USART_InitStructure.USART_Parity = USART_Parity_No; //奇偶失能
	 USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //硬件流控制失能
	 USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //发送、接收使能
	
	 USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
	 USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;//空闲时钟为低电平
	 USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;//时钟第二个边沿进行数据捕获
	 USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;//最后一位数据的时钟脉冲不从SCLK输出


	 //使能串口1、2接收中断
	 USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 
	 USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); 
		
	 USART_ClockInit(USART1, &USART_ClockInitStructure);
	 USART_ClockInit(USART2, &USART_ClockInitStructure);
	 USART_Init(USART1, &USART_InitStructure);	//初始化结构体
	 USART_Init(USART2, &USART_InitStructure);	//初始化结构体
//	 USART_ClearFlag(USART1,USART_FLAG_TC);//清除发送完成标志，不然有时可能会有第一个字符发送不出去的情况
//	 USART_ClearFlag(USART2,USART_FLAG_TC);//清除发送完成标志，不然有时可能会有第一个字符发送不出去的情况
	 USART_GetFlagStatus(USART1, USART_FLAG_TC);//清除发送完成标志，不然有时可能会有第一个字符发送不出去的情况
	 USART_GetFlagStatus(USART2, USART_FLAG_TC);//清除发送完成标志，不然有时可能会有第一个字符发送不出去的情况
	 
	 
	 InitQueue(&USART1Buffer);//USART2缓冲区初始化
	 
	 USART_Cmd(USART1, ENABLE); //使能串口1
	 USART_Cmd(USART2, ENABLE); //使能串口2
	 
}
void Usart_NVIC_Configuration(void)
{
	 NVIC_InitTypeDef NVIC_InitStructure;
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	/******************注册串口1中断服务函数************************/
   NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//配置串口中断
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//占先式优先级设置为0
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //副优先级设置为0
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//中断开启
   NVIC_Init(&NVIC_InitStructure);//中断初始化
	
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	/******************注册串口1中断服务函数************************/
   NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//配置串口中断
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//占先式优先级设置为0
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; //副优先级设置为0
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//中断开启
   NVIC_Init(&NVIC_InitStructure);//中断初始化
}
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
/***************************START*********************/
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	if(USARTNum==1)
	{
		USART1->DR = (u8) ch;      
		while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
		return ch;
	}
	else if(USARTNum==2)
	{
		USART2->DR = (u8) ch;      
		while((USART2->SR&0X40)==0);//循环发送,直到发送完毕   
		return ch;
	}
	else
		return ch;
}
#endif 
/***************************END*********************/

void USART1_Putc(uint8_t data)			//利用Usart1发送一个8位数据
{
    USART1->DR = (uint8_t)data; //要发送的字符赋给串口数据寄存器  
	while((USART1->SR&0X40)==0); //等待发送完成  
}
void USART1_Puts(uint8_t * buffer)			//利用Usart1发送一个字符串
{
    while(*buffer)
    {
        USART1->DR= *buffer++;
		while((USART1->SR&0X40)==0);//等待发送完成  
    }
}
void USART1_Send_Enter(void)//利用串口1发送一换行符
{
	USART1_Putc(0x0d);
	USART1_Putc(0x0a);
}

/******************串口1中断服务函数*******************************/
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)
   {
		 EnQueue(&USART1Buffer,USART_ReceiveData(USART1));//新的数据进缓冲区
	 }
}

/******************串口2中断服务函数*******************************/
void USART2_IRQHandler(void)
{
	 u8 data=0;
   if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)
   {
		 data=USART_ReceiveData(USART2);
		USART_SendData(USART1,data);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET){};
	 }
}
