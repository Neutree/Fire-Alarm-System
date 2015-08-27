# include "stm32f10x.h"
# include "delay.h"
# include "usart.h"
# include "esp8266.h"
# include "time.h"
# include "nrf24l01.h"

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
	
	NRF24L01_Init();//2.4G无线初始化
	
	
	delay_s(2);
}

u8 buf[5];
u8 dataTxBuffer[10]="oh My god";
u8 i;
int main()
{
		init();
	
		USARTNum=2;
		if(NRF24L01_Check())
			printf("connect to nrf24l01 success!\n");
		else
			printf("connect to nrf24l01 failed!\n");
		NRF24L01_RX_Mode();
		while(1)
		{
			
	//		printf("%X\t%d\t%d\n",NRF24L01_Read_Reg(NRF24L01_STATUS),0x01&(GPIOB->IDR&1<<11)>>11,NRF24L01_Read_Reg(NRF24L01_CD));
	//		NRF24L01_Write_Reg(NRF24L01_WRITE_REG+NRF24L01_STATUS,0xff); //清除TX_DS或MAX_RT中断标志
		//	printf("%x\n",NRF24L01_TxPacket(dataTxBuffer));
			
//			NRF24L01_Read_Buf(NRF24L01_RX_ADDR_P0,buf,5); //读出写入的地址 
//			for(i=0;i<5;++i)
//			{
//				printf("%X\t",buf[i]);
//			}
//			printf("\n");
//			if(setEcho(0))//AT 
//			{
//				USARTNum=2;
//				printf("success\n");
//				
//			}
//			else
//			{
//				USARTNum=2;
//				printf("failed\n");
//			}
//			delay_s(2);
		//	NRF24L01_RX_Mode();
		}
}

