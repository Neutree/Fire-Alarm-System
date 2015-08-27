# include "stm32f10x.h"
# include "delay.h"
# include "nrf24l01.h"


void GPIO_Init_Config()
{
	 GPIO_InitTypeDef GPIO_InitStructure;
	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//PA9时钟速度50MHz
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //
	 GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	 GPIO_ResetBits(GPIOB,GPIO_Pin_1);
}


void init()
{
	SystemInit();
	GPIO_Init_Config();
	NRF24L01_Init();//2.4G无线初始化
	
	delay_s(2);
}
u8 dataTxBuffer[10]="oh My god";
int main()
{
		u8 flag=0;
		init();
		if(NRF24L01_Check())
		{
				GPIO_SetBits(GPIOB,GPIO_Pin_1);
				delay_ms(200);
				GPIO_ResetBits(GPIOB,GPIO_Pin_1);
				delay_ms(200);
				GPIO_SetBits(GPIOB,GPIO_Pin_1);
				delay_ms(200);
				GPIO_ResetBits(GPIOB,GPIO_Pin_1);
				delay_ms(200);
				delay_s(3);
		}
		else
		{
				GPIO_SetBits(GPIOB,GPIO_Pin_1);
				delay_ms(200);
				GPIO_ResetBits(GPIOB,GPIO_Pin_1);
				delay_ms(200);
				GPIO_SetBits(GPIOB,GPIO_Pin_1);
				delay_ms(200);
				GPIO_ResetBits(GPIOB,GPIO_Pin_1);
				delay_ms(200);
				GPIO_SetBits(GPIOB,GPIO_Pin_1);
				delay_ms(200);
				GPIO_ResetBits(GPIOB,GPIO_Pin_1);
				delay_s(3);
		}
		NRF24L01_TX_Mode();
		while(1)
		{
//			NRF24L01_TX_Mode();
//			NRF24L01_TxPacket(dataTxBuffer);
			
			flag=NRF24L01_TxPacket(dataTxBuffer);
			
			if(NRF24L01_TX_OK==flag)
			{
				GPIO_SetBits(GPIOB,GPIO_Pin_1);
				delay_ms(100);
				GPIO_ResetBits(GPIOB,GPIO_Pin_1);
				delay_ms(100);
			}
			else if(0xff==flag)
			{
				GPIO_SetBits(GPIOB,GPIO_Pin_1);
				delay_ms(100);
				GPIO_ResetBits(GPIOB,GPIO_Pin_1);
				delay_ms(100);
				GPIO_SetBits(GPIOB,GPIO_Pin_1);
				delay_ms(100);
				GPIO_ResetBits(GPIOB,GPIO_Pin_1);
				delay_ms(100);
			}
			else if(NRF24L01_MAX_TX==flag)
			{
				GPIO_SetBits(GPIOB,GPIO_Pin_1);
				delay_ms(100);
				GPIO_ResetBits(GPIOB,GPIO_Pin_1);
				delay_ms(100);
				GPIO_SetBits(GPIOB,GPIO_Pin_1);
				delay_ms(100);
				GPIO_ResetBits(GPIOB,GPIO_Pin_1);
				delay_ms(100);
				GPIO_SetBits(GPIOB,GPIO_Pin_1);
				delay_ms(100);
				GPIO_ResetBits(GPIOB,GPIO_Pin_1);
				delay_ms(100);
			}
			else
			{
				GPIO_SetBits(GPIOB,GPIO_Pin_1);
				delay_ms(100);
				GPIO_ResetBits(GPIOB,GPIO_Pin_1);
				delay_ms(100);
				GPIO_SetBits(GPIOB,GPIO_Pin_1);
				delay_ms(100);
				GPIO_ResetBits(GPIOB,GPIO_Pin_1);
				delay_ms(100);
				GPIO_SetBits(GPIOB,GPIO_Pin_1);
				delay_ms(100);
				GPIO_ResetBits(GPIOB,GPIO_Pin_1);
				delay_ms(100);
				GPIO_SetBits(GPIOB,GPIO_Pin_1);
				delay_ms(100);
				GPIO_ResetBits(GPIOB,GPIO_Pin_1);
				delay_ms(100);
			}
			GPIO_ResetBits(GPIOB,GPIO_Pin_1);
			delay_ms(500);
		}
}

