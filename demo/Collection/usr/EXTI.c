#include "stm32f10x_exti.h"
#include "EXTI.h"

void EXTI_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
	
	//KEY
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	
	

	EXTI_ClearITPendingBit(EXTI_Line11);							//清除中断标志
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource11); //引脚映射到外部中断线上

	EXTI_InitStructure.EXTI_Line=EXTI_Line11;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;		//模式为中断模式  另一个是事件
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling; 
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;

	EXTI_Init(&EXTI_InitStructure);
	
	

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //中断组用四位表示，组0：抢占式优先级0位，副优先级4位，。。。。。。组4：抢占式优先级4位，副优先级0位\
//	取值情况如下表：
/*	 The table below gives the allowed values of the pre-emption priority and subpriority according
 to the Priority Grouping configuration performed by NVIC_PriorityGroupConfig function
  ============================================================================================================================
    NVIC_PriorityGroup   | NVIC_IRQChannelPreemptionPriority | NVIC_IRQChannelSubPriority  | Description
  ============================================================================================================================
   NVIC_PriorityGroup_0  |                0                  |            0-15             |   0 bits for pre-emption priority
                         |                                   |                             |   4 bits for subpriority
  ----------------------------------------------------------------------------------------------------------------------------
   NVIC_PriorityGroup_1  |                0-1                |            0-7              |   1 bits for pre-emption priority
                         |                                   |                             |   3 bits for subpriority
  ----------------------------------------------------------------------------------------------------------------------------    
   NVIC_PriorityGroup_2  |                0-3                |            0-3              |   2 bits for pre-emption priority
                         |                                   |                             |   2 bits for subpriority
  ----------------------------------------------------------------------------------------------------------------------------    
   NVIC_PriorityGroup_3  |                0-7                |            0-1              |   3 bits for pre-emption priority
                         |                                   |                             |   1 bits for subpriority
  ----------------------------------------------------------------------------------------------------------------------------    
   NVIC_PriorityGroup_4  |                0-15               |            0                |   4 bits for pre-emption priority
                         |                                   |                             |   0 bits for subpriority                       
  ============================================================================================================================
*/
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; //注意：中断线5~15的中断函数如下，与1~4不同
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
}


void EXTI0_IRQHandler(void)
{
	u8 flag=0;
	static u8 flag_before=0;
	if(EXTI_GetITStatus(EXTI_Line0)!=RESET)
	{
		delay_ms(50);
		flag=GPIO_ReadInputDataBit(SENSOR_LIGHT_GPIO,SENSOR_LIGHT_PIN);
		if(flag_before!=flag)//status changed 状态改变
		{
			if(!flag)
			{
				printf("Light intensity\n ");
			}
			else
			{
				printf("Light Week\n ");
			}
			flag_before=flag;//update status 更新状态
		}	
		EXTI_ClearITPendingBit(EXTI_Line0);	
	}
}
void EXTI1_IRQHandler(void)
{
	u8 flag=0;
	static u8 flag_before=0;
	if(EXTI_GetITStatus(EXTI_Line1)!=RESET)
	{
		delay_ms(50);
		flag=GPIO_ReadInputDataBit(SENSOR_MOV_GPIO,SENSOR_MOV_PIN);
		if(flag_before!=flag)//status changed 状态改变
		{
			if(!flag)
			{
				printf("Moving\n ");
			}
			else
			{
				printf("Not Moving\n ");
			}
			flag_before=flag;//update status 更新状态
		}		
		EXTI_ClearITPendingBit(EXTI_Line1);	
	}
}
//注意：中断线5~15的中断函数如下，与1~4不同
void EXTI9_5_IRQHandler(void)
{
	
}

u8 dataBuffer[10];
void EXTI15_10_IRQHandler(void)
{
	u8 flag=0;
	static u8 flag_before_mov=0,flag_before_fire=0;
	u8 status=0;
	USARTNum=2;
	if(EXTI_GetITStatus(EXTI_Line11)!=RESET)
	{
		status=NRF24L01_RxPacket(dataBuffer);
//		NRF24L01_RX_Mode();//进行下一次接收，否则会保持待机模式
		NRF24L01_CE_H();//进行下一次接收，否则会保持待机模式
		if(NRF24L01_RX_OK==status)//接收数据成功
		{
				printf("%s\n",dataBuffer);
		}
		printf("a\n");
		EXTI_ClearITPendingBit(EXTI_Line11);	
	}
	else if(EXTI_GetITStatus(EXTI_Line(SENSOR_FIRE_PIN_NUM))!=RESET)
	{
		delay_ms(50);
		flag=GPIO_ReadInputDataBit(SENSOR_FIRE_GPIO,SENSOR_FIRE_PIN);
		if(flag_before_fire!=flag)//status changed 状态改变
		{
			if(!flag)
			{
				printf("Fire is burning\n ");
			}
			else
			{
				printf("Fire Back to normal\n ");
			}
			flag_before_fire=flag;//update status 更新状态
		}
		EXTI_ClearITPendingBit(EXTI_Line15);	
	}
	else if(EXTI_GetITStatus(EXTI_Line14)!=RESET)
	{
		delay_ms(50);
		flag=GPIO_ReadInputDataBit(SENSOR_GAS_GPIO,SENSOR_GAS_PIN);
		if(flag_before_mov!=flag)//status changed 状态改变
		{
			if(!flag)
			{
				printf("Gas leak\n ");
			}
			else
			{
				printf("Gas Back to normal\n ");
			}
			flag_before_mov=flag;//update status 更新状态
		}
		EXTI_ClearITPendingBit(EXTI_Line14);	
	}
	
}
