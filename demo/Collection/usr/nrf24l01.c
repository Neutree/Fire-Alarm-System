# include "nrf24l01.h"


const u8 TX_ADDRESS[NRF24L01_TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //���͵�ַ
const u8 RX_ADDRESS[NRF24L01_RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //���յ�ַ

// SPI�����ٶ����� 
#define SPI_SPEED_2   0
#define SPI_SPEED_8   1
#define SPI_SPEED_16  2
#define SPI_SPEED_256 3

void SPI2_SetSpeed(u8 SpeedSet)
{
   SPI2->CR1&=0XFFC7;//Fsck=Fcpu/256
   if(SpeedSet==SPI_SPEED_2)//����Ƶ
{
SPI2->CR1|=0<<3;//Fsck=Fpclk/2=36Mhz
}else if(SpeedSet==SPI_SPEED_8)//�˷�Ƶ 
{
SPI2->CR1|=2<<3;//Fsck=Fpclk/8=9Mhz
}else if(SpeedSet==SPI_SPEED_16)//ʮ����Ƶ
{
SPI2->CR1|=3<<3;//Fsck=Fpclk/16=4.5Mhz
}else //256��Ƶ
{
SPI2->CR1|=7<<3; //Fsck=Fpclk/256=281.25Khz ����ģʽ
}
SPI2->CR1|=1<<6; //SPI�豸ʹ��
}

u8 SPI_ReadWriteByte(u8 data)
{
	u8 retry=0;  
	while((SPI2->SR&1<<1)==0)//�ȴ���������//while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET)
	{
		retry++;
		if(retry>200)return 0;
	}  
	SPI2->DR=data;  //����һ��byte //SPI_I2S_SendData(SPI2,data);
	retry=0;
	while((SPI2->SR&1<<0)==0) //�ȴ�������һ��byte  //while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET)
	{
		retry++;
		if(retry>200)return 0;
	}      
	return SPI2->DR;          //�����յ������� //SPI_I2S_ReceiveData(SPI2);
}

void SPI2_Init()
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//����SPI�������ŵ�ʱ�ӣ���������ʱ�ӣ�����CE ��CSN ���Ŷ�Ӧ��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
	
	//���� CE �� CSN ���� 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//PA9ʱ���ٶ�50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//����SCK MISO MOSI���� ���������
	/*!< Configure SPI_FLASH_SPI pins: SCK MISO MOSI*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//����IRQ���ţ� ����Ϊ��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	SPI_I2S_DeInit(SPI2);                          
	SPI_Cmd(SPI2, DISABLE); //�����Ƚ���,���ܸı�MODE
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;       //��
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;      //8λ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;        //CPOL=0 ʱ�����յ�
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;       //CPHA=0 ���ݲ����1��
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;        //���NSS
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8 ; //8��Ƶ 9MHz
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;      //��λ��ǰ
	SPI_InitStructure.SPI_CRCPolynomial = 7;        //CRC7

	SPI_Init(SPI2, &SPI_InitStructure);
	SPI_Cmd(SPI2, ENABLE);
	SPI_ReadWriteByte(0xff);   //��������
}



void  NRF24L01_Init(void)
{
	SPI2_Init(); //��ʼ��SPI
	NRF24L01_CE_L();          //ʹ��24L01
	NRF24L01_CSN_H();        //SPIƬѡȡ��
	EXTI_Config();//�����ж�
	//������ա����ͻ���������־
	NRF24L01_Write_Reg(NRF24L01_FLUSH_RX,0xff);//���RX FIFO�Ĵ���
	NRF24L01_Write_Reg(NRF24L01_FLUSH_TX,0xff);//���TX FIFO�Ĵ��� 
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG+NRF24L01_STATUS,0xff); //���TX_DS��MAX_RT�жϱ�־	
}

u8 NRF24L01_Check(void)
{
	u8 buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	u8 i;
	SPI2_SetSpeed(SPI_SPEED_8); //spi�ٶ�Ϊ9Mhz��24L01�����SPIʱ��Ϊ10Mhz��    
	NRF24L01_Write_Buf(NRF24L01_WRITE_REG+NRF24L01_TX_ADDR,buf,5);//д��5���ֽڵĵ�ַ.
	NRF24L01_Read_Buf(NRF24L01_TX_ADDR,buf,5); //����д��ĵ�ַ  
	for(i=0;i<5;i++)
	{
		if(buf[i]!=0XA5)
			break; 
	}		
	if(i==5)
		return 1;//��⵽24L01
	return 0; //���24L01����
}  
   
 
u8 NRF24L01_Write_Reg(u8 reg,u8 value)
{
	u8 status;
	NRF24L01_CE_L();                            //����ģʽ
	NRF24L01_CSN_L();                          //ʹ��SPI����
	status=SPI_ReadWriteByte(reg);    //���ͼĴ����� 
	SPI_ReadWriteByte(value);         //д��Ĵ�����ֵ
	NRF24L01_CSN_H();                          //��ֹSPI����
	return(status);    //����״ֵ̬
}
u8 NRF24L01_Read_Reg(u8 reg)
{
	u8 reg_val;    
	NRF24L01_CSN_L();                        //ʹ��SPI����
	SPI_ReadWriteByte(reg);         //���ͼĴ�����
	reg_val=SPI_ReadWriteByte(0XFF);//��ȡ�Ĵ�������
	NRF24L01_CSN_H();                        //��ֹSPI����    
	return(reg_val);                //����״ֵ̬
}

u8 NRF24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len)
{
	u8 status,u8_ctr;       
	NRF24L01_CSN_L();                             //ʹ��SPI����
	status=SPI_ReadWriteByte(reg);       //���ͼĴ���ֵ(λ��),����ȡ״ֵ̬ 

	for(u8_ctr=0;u8_ctr<len;u8_ctr++)
		pBuf[u8_ctr]=SPI_ReadWriteByte(0XFF);//��������
	NRF24L01_CSN_H();                             //��ֹSPI����
	return status;                       //���ض�����״ֵ̬
}
u8 NRF24L01_Write_Byte(u8 reg, u8 pBuf)
{
	u8 status;    
	NRF24L01_CSN_L();                        //ʹ��SPI����
	status = SPI_ReadWriteByte(reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
	SPI_ReadWriteByte(pBuf);        //д������  
	NRF24L01_CSN_H();                        //�ر�SPI����
	return status;                  //���ض�����״ֵ̬
}

u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
	u8 status,u8_ctr;    
	NRF24L01_CSN_L();                        //ʹ��SPI����
	status = SPI_ReadWriteByte(reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
	for(u8_ctr=0; u8_ctr<len; u8_ctr++)
		SPI_ReadWriteByte(*pBuf++);     //д������  
	NRF24L01_CSN_H();                        //�ر�SPI����
	return status;                  //���ض�����״ֵ̬
}

  
u8 NRF24L01_TxPacket(u8 *txbuf)
{
	u8 sta;
	u16 Time_out_count=0;
	SPI2_SetSpeed(SPI_SPEED_8);//spi�ٶ�Ϊ9Mhz��24L01�����SPIʱ��Ϊ10Mhz��   
	NRF24L01_CE_L();
	NRF24L01_Write_Buf(NRF24L01_WR_TX_PLOAD,txbuf,NRF24L01_TX_PLOAD_WIDTH);//д���ݵ�TX BUF  32���ֽ�
	NRF24L01_CE_H();                 //��������   
	while(GPIOB->IDR&1<<11) //�ȴ��������
	{
		++Time_out_count;
		if(Time_out_count>=65535)
			return 0xfe;//time out 
	}
	sta=NRF24L01_Read_Reg(NRF24L01_STATUS);  //��ȡ״̬�Ĵ�����ֵ   
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG+NRF24L01_STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	if(sta&NRF24L01_MAX_TX)//�ﵽ����ط�����
	{
		NRF24L01_Write_Reg(NRF24L01_FLUSH_TX,0xff);//���TX FIFO�Ĵ��� 
		return NRF24L01_MAX_TX; 
	}
	if(sta&NRF24L01_TX_OK)//�������
	{
		return NRF24L01_TX_OK;
	}
	return 0xff;//����ԭ����ʧ��
}

////////////////////
///
///ע�⣺ִ������������ģʽ�����������һ�ν��գ�����ʹ��CE����������ʹ��NRF24L01_RX_Mode();�������ģʽ
//////////////////////
u8 NRF24L01_RxPacket(u8 *rxbuf)
{
	u8 sta;
	NRF24L01_CE_L();	//�������ģʽ
	SPI2_SetSpeed(SPI_SPEED_8); //spi�ٶ�Ϊ9Mhz��24L01�����SPIʱ��Ϊ10Mhz��   
	sta=NRF24L01_Read_Reg(NRF24L01_STATUS);  //��ȡ״̬�Ĵ�����ֵ      
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG+NRF24L01_STATUS,sta); //���TX_DS��MAX_RT�жϱ�־	
	if(sta&NRF24L01_RX_OK)//���յ�����
	{
		NRF24L01_Read_Buf(NRF24L01_RD_RX_PLOAD,rxbuf,NRF24L01_RX_PLOAD_WIDTH);//��ȡ����
		NRF24L01_Write_Reg(NRF24L01_FLUSH_RX,0xff);//���RX FIFO�Ĵ��� 
		return NRF24L01_RX_OK; 
	}
	
	return 0;//û�յ��κ�����
}    
  
void NRF24L01_RX_Mode(void)
{
	NRF24L01_CE_L();  
	NRF24L01_Write_Buf(NRF24L01_WRITE_REG+NRF24L01_RX_ADDR_P0,(u8*)RX_ADDRESS,NRF24L01_RX_ADR_WIDTH);//дRX�ڵ��ַ

	NRF24L01_Write_Reg(NRF24L01_WRITE_REG+NRF24L01_EN_AA,0x01);             //ʹ��ͨ��0���Զ�Ӧ��    
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG+NRF24L01_EN_RXADDR,0x01);         //ʹ��ͨ��0�Ľ��յ�ַ    
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG+NRF24L01_RF_CH,40);              //����RFͨ��Ƶ��  
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG+NRF24L01_RX_PW_P0,NRF24L01_RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ��    
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG+NRF24L01_RF_SETUP,0x0f);          //����TX�������,0db����,2Mbps,���������濪��   
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG+NRF24L01_CONFIG, 0x0f);           //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 
	NRF24L01_CE_H();    //CEΪ��,�������ģʽ 
	delay_us(135);  //CEҪ����һ��ʱ��Ž��뷢��ģʽ  Power Down -> Standby mode:0~1.5ms  Standby modes -> TX/RX mode  0~130us  For start up time see Table 13 at nrf24l01 product specification
}  
 
void NRF24L01_TX_Mode(void)
{  
	NRF24L01_CE_L();    
	NRF24L01_Write_Buf(NRF24L01_WRITE_REG+NRF24L01_TX_ADDR,(u8*)TX_ADDRESS,NRF24L01_TX_ADR_WIDTH);//дTX�ڵ��ַ 
	NRF24L01_Write_Buf(NRF24L01_WRITE_REG+NRF24L01_RX_ADDR_P0,(u8*)TX_ADDRESS,NRF24L01_RX_ADR_WIDTH); //����RX_P0�ڵ��ַ���뷢�͵�ַ��ͬ,Ϊ�˽���ACK�ź�

	NRF24L01_Write_Reg(NRF24L01_WRITE_REG+NRF24L01_EN_AA,0x01);     //ʹ��ͨ��0���Զ�Ӧ��    
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG+NRF24L01_EN_RXADDR,0x01); //ʹ��ͨ��0�Ľ��յ�ַ  
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG+NRF24L01_SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG+NRF24L01_RF_CH,40);       //����RFͨ��Ϊ40
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG+NRF24L01_RF_SETUP,0x0f);  //����TX�������,0db����,2Mbps,���������濪��   
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG+NRF24L01_RX_PW_P0,NRF24L01_RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ��    
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG+NRF24L01_CONFIG,0x0e);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
	NRF24L01_CE_H(); //CEΪ��,10us����������
	delay_us(135);  //CEҪ����һ��ʱ��Ž��뷢��ģʽ  Power Down -> Standby mode:0~1.5ms  Standby modes -> TX/RX mode  0~130us  For start up time see Table 13 at nrf24l01 product specification
}  

