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
	if(EXTI_GetITStatus(EXTI_Line0)!=RESET)
	{
		
		
		EXTI_ClearITPendingBit(EXTI_Line0);	
	}
}
//注意：中断线5~15的中断函数如下，与1~4不同
void EXTI9_5_IRQHandler(void)
{
	
}
//////////////////////////////////////////////////
///deal with received data
/////////////////////////////////////////////////
u8 dataBuffer[6]={0,0,0,0,0,0};

volatile u8 EXTI_flag=0;

///////////////////////////////////////
///sensor status
///bit7~4:status before    bit3~0: status now
///////////////////////////////////////
volatile u8 Sensor_Gas_status=0;
volatile u8 Sensor_Mov_status=0;
//volatile u8 Sensor_Light_status=0;
volatile u8 Sensor_Fire_status=0;
volatile u8 Sensor_Door1_status=0;
volatile u8 Sensor_Door2_status=0;
volatile u8 latest_Sensor_detected = 0xff;
void EXTI15_10_IRQHandler(void)
{
	u8 status=0;
	
	if(EXTI_GetITStatus(EXTI_Line11)!=RESET)
	{
		status=NRF24L01_RxPacket(dataBuffer);
//		NRF24L01_RX_Mode();//进行下一次接收，否则会保持待机模式
		if(NRF24L01_RX_OK==status)//接收数据成功
		{
			EXTI_flag=1;
			Rcv_Message_Deal();
		}
		NRF24L01_CE_H();//进行下一次接收，否则会保持待机模式
		EXTI_ClearITPendingBit(EXTI_Line11);	
	}
}

void Rcv_Message_Deal(void)
{
	
//	LCD1602_BL_ON();
	if(dataBuffer[5]==0x11)//传感器信息
	{
		switch(dataBuffer[4])
		{
			case SENSOR_GAS_NUMBER:
				if(dataBuffer[0])
				{
//					LCD1602_WriteString_At_Pos(0,1,"STA:GasL  NOW:Y");
//					printf("Gas leak! \n");
					Sensor_Gas_status=0x11;
					latest_Sensor_detected=SENSOR_GAS_NUMBER;
					printf("Gas leak!\n");
				}
				else
				{
//					LCD1602_WriteString_At_Pos(9,1," NOW:N ");
//					printf("Gas normal! \n");
					Sensor_Gas_status &= 0x10;
					printf("gas return to normal!\n");
				}
				break;
			case SENSOR_FIRE_NUMBER:
				if(dataBuffer[0])
				{
//					LCD1602_WriteString_At_Pos(0,1,"STA:Flame NOW:Y");
//					printf("fire burning! \n");
					Sensor_Fire_status=0x11;
					latest_Sensor_detected=SENSOR_FIRE_NUMBER;
					printf("detected flame!\n");
				}
				else
				{
//					LCD1602_WriteString_At_Pos(9,1," NOW:N ");
//					printf("no fire now \n");
					Sensor_Fire_status &= 0x10;
					printf("flame return to normal\n");
				}
				break;
			case SENSOR_MOV_NUMBER:
				if(dataBuffer[0])
				{
//					LCD1602_WriteString_At_Pos(0,1,"STA:Moved NOW:Y");
//					printf("moved without open door ! \n");
					Sensor_Mov_status=0x11;
					latest_Sensor_detected=SENSOR_MOV_NUMBER;
					printf("detected Vibration!\n");
				}
				else
				{
//					LCD1602_WriteString_At_Pos(9,1," NOW:N ");
//					printf("moved without open door ,and now it is not moving ! \n");
					Sensor_Mov_status &= 0x10;
				}
				break;
		}
	}
	else if(dataBuffer[5]==0x22)//门锁信号
	{
		switch(dataBuffer[4])
		{
			case 1://door 1
				if(dataBuffer[0]==1)//open
				{
	//				LCD1602_WriteString_At_Pos(7,2,"ON ");
	//							printf("Door one is open !\n");
					Sensor_Door1_status=0x11;
					printf("the first door is open!\n");
				}
				else if(dataBuffer[0]==2)//close
				{
	//				LCD1602_WriteString_At_Pos(7,2,"OFF");
	//							printf("Door one is close !\n");
					Sensor_Door1_status = 0;
					printf("the first door is close!\n");
				}
				else if(dataBuffer[0]==3)
				{
					Sensor_Door1_status=0x13;
					printf("Some one tye to open door1, but failed!\n");
				}
				break;
			case 2: //door2
				if(dataBuffer[0]==1)//open
				{
	//				LCD1602_WriteString_At_Pos(13,2,"ON ");
	//							printf("Door two is open !\n");
					Sensor_Door2_status=0x11;
					printf("the second door is open!\n");
				}
				else if(dataBuffer[0]==2)//close
				{
	//				LCD1602_WriteString_At_Pos(13,2,"OFF");
	//							printf("Door two is close !\n");
					Sensor_Door2_status = 0;
					printf("the second door is close!\n");
				}
				else if(dataBuffer[0]==3)
				{
					Sensor_Door2_status=0x13;
					printf("Some one tye to open door2, but failed!\n");
				}
				break;
		}
	}
}
void Sensor_Status_Dis(void)
{
	//显示是否有异常状态发生
	if(latest_Sensor_detected!=0xff)//有非正常动作发生
	{
		if(latest_Sensor_detected==SENSOR_MOV_NUMBER)
		{
					LCD1602_WriteString_At_Pos(0,1,"  STATUS:Moved  ");
		}
		else if(latest_Sensor_detected==SENSOR_FIRE_NUMBER)
		{
					LCD1602_WriteString_At_Pos(0,1,"  STATUS:Flame  ");
		}
		else if(latest_Sensor_detected==SENSOR_GAS_NUMBER)
		{
					LCD1602_WriteString_At_Pos(0,1," STATUS:GasLeak ");
		}
	}
	if(Sensor_Door1_status)
	{
		LCD1602_WriteString_At_Pos(7,2,"ON ");
	}
	else
	{
		LCD1602_WriteString_At_Pos(7,2,"OFF");
	}
	if(Sensor_Door2_status)
	{
		LCD1602_WriteString_At_Pos(13,2,"ON ");
	}
	else
	{
		LCD1602_WriteString_At_Pos(13,2,"OFF");
	}
}
