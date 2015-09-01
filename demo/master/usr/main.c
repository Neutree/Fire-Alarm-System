# include "stm32f10x.h"
# include "delay.h"
# include "nrf24l01.h"
# include "LCD1602.h"
# include "usart.h"
# include "EXTI.h"

void init()
{
	SystemInit();
	LCD1602_Init();
	NRF24L01_Init();//2.4G无线初始化
	Usart_Configuration();
	Usart_NVIC_Configuration();
	LCD1602_WriteString_At_Pos(0,1,"  STATUS:Normal ");
	LCD1602_WriteString_At_Pos(0,2,"LOCK 1:OFF 2:OFF");
	delay_s(2);
}


int main()
{
		init();
	//检测nrf24l01模块连接状况
		if(NRF24L01_Check())//检测正常
		{
				LCD1602_BL_ON()
				delay_ms(100);
				LCD1602_BL_OFF();
				delay_ms(100);
				LCD1602_BL_ON()
				delay_ms(100);
				LCD1602_BL_OFF();
				delay_ms(100);
				delay_ms(500);
		}
		else				//未检测到nrf24l01模块
		{
				LCD1602_BL_ON()
				delay_ms(100);
				LCD1602_BL_OFF();
				delay_ms(100);
				LCD1602_BL_ON()
				delay_ms(100);
				LCD1602_BL_OFF();
				delay_ms(100);
				LCD1602_BL_ON()
				delay_ms(100);
				LCD1602_BL_OFF();
				delay_ms(100);
				delay_ms(500);
		}
		NRF24L01_RX_Mode();
		GPIO_SetBits(GPIOB,GPIO_Pin_1);
		while(1)
		{
			if(EXTI_flag)
			{
				EXTI_flag=0;
				Sensor_Status_Dis();
			}
			
		}
}

