# include "stm32f10x.h"
# include "delay.h"
# include "usart.h"
# include "esp8266.h"
# include "time.h"
# include "nrf24l01.h"
# include "sensor.h"
void GPIO_Init_Config()
{
	 GPIO_InitTypeDef GPIO_InitStructure;
	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	

	
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
		GPIO_Init(GPIOB,&GPIO_InitStructure);
}


void init()
{
	SystemInit();
	GPIO_Init_Config();	
	Usart_Configuration();
	Usart_NVIC_Configuration();
	TIM3_Init();
	
	setEcho(0);//关闭回显
	Sensor_Init();
	NRF24L01_Init();//2.4G无线初始化
	
	
	delay_s(2);
}

u8 buf[5];
u8 dataTxBuffer[11]="oh My god";
u8 i;
int main()
{
		init();
	
		USARTNum=2;
		if(NRF24L01_Check())
			printf("connect to nrf24l01 success!\n");
		else
			printf("connect to nrf24l01 failed!\n");
		NRF24L01_TX_Mode();
		while(1)
		{
			dataTxBuffer[0]='O';
			dataTxBuffer[1]='H';
			NRF24L01_TxPacket(dataTxBuffer);
			delay_ms(200);
			dataTxBuffer[0]='A';
			dataTxBuffer[1]='A';
			NRF24L01_TxPacket(dataTxBuffer);
			delay_ms(200);
		}
}

