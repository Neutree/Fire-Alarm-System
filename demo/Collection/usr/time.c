#include "time.h"

u16 time_s=0,time_100ms=0;//计时 time count   ms s 

void TIM3_Init()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
	NVIC_InitTypeDef NVIC_InitStructure; 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	
	
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);

	TIM_TimeBaseStruct.TIM_Period=199;//初值  100ms
	TIM_TimeBaseStruct.TIM_Prescaler=35999;//预分频
	TIM_TimeBaseStruct.TIM_ClockDivision=0;
	TIM_TimeBaseStruct.TIM_CounterMode=TIM_CounterMode_Up;//向上

	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStruct);

	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM3,ENABLE);	 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
}
//初始化定时器
void initTimeCount(void)
{
	TIM_Cmd(TIM3,DISABLE);
	time_s=0;
	time_100ms=0;
	TIM_Cmd(TIM3,ENABLE);
}
////////////////////////////////////
///定时器3中断程序
////////////////////////////////////
void TIM3_IRQHandler(void)
{
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	++time_100ms;
	if(time_100ms>=10)
	{
		++time_s;
		time_100ms=0;
	}
	
}

