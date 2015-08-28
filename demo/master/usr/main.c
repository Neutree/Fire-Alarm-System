# include "stm32f10x.h"
# include "delay.h"
# include "nrf24l01.h"
# include "LCD1602.h"


void init()
{
	SystemInit();
	LCD1602_Init();
	NRF24L01_Init();//2.4G无线初始化
	
	delay_s(2);
}
u8 dataTxBuffer[10]="oh My god";
u8 vl = 50;
int main()
{
		init();
	//检测nrf24l01模块连接状况
		if(NRF24L01_Check())
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
		else
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
			
		}
}

