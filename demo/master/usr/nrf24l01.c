# include "nrf24l01.h"


const u8 TX_ADDRESS[NRF24L01_TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //发送地址
const u8 RX_ADDRESS[NRF24L01_RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //接收地址

// SPI总线速度设置 
#define SPI_SPEED_2   0
#define SPI_SPEED_8   1
#define SPI_SPEED_16  2
#define SPI_SPEED_256 3

void SPI2_SetSpeed(u8 SpeedSet)
{
   SPI2->CR1&=0XFFC7;//Fsck=Fcpu/256
   if(SpeedSet==SPI_SPEED_2)//二分频
{
SPI2->CR1|=0<<3;//Fsck=Fpclk/2=36Mhz
}else if(SpeedSet==SPI_SPEED_8)//八分频 
{
SPI2->CR1|=2<<3;//Fsck=Fpclk/8=9Mhz
}else if(SpeedSet==SPI_SPEED_16)//十六分频
{
SPI2->CR1|=3<<3;//Fsck=Fpclk/16=4.5Mhz
}else //256分频
{
SPI2->CR1|=7<<3; //Fsck=Fpclk/256=281.25Khz 低速模式
}
SPI2->CR1|=1<<6; //SPI设备使能
}

u8 SPI_ReadWriteByte(u8 data)
{
	u8 retry=0;  
	while((SPI2->SR&1<<1)==0)//等待发送区空//while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET)
	{
		retry++;
		if(retry>200)return 0;
	}  
	SPI2->DR=data;  //发送一个byte //SPI_I2S_SendData(SPI2,data);
	retry=0;
	while((SPI2->SR&1<<0)==0) //等待接收完一个byte  //while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET)
	{
		retry++;
		if(retry>200)return 0;
	}      
	return SPI2->DR;          //返回收到的数据 //SPI_I2S_ReceiveData(SPI2);
}

void SPI2_Init()
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//开启SPI所用引脚的时钟，开启复用时钟，开启CE 、CSN 引脚对应的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
	
	//配置 CE 和 CSN 引脚 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//PA9时钟速度50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//配置SCK MISO MOSI引脚 ，复用输出
	/*!< Configure SPI_FLASH_SPI pins: SCK MISO MOSI*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//配置IRQ引脚， 配置为上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	SPI_I2S_DeInit(SPI2);                          
	SPI_Cmd(SPI2, DISABLE); //必须先禁用,才能改变MODE
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //两线全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;       //主
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;      //8位
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;        //CPOL=0 时钟悬空低
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;       //CPHA=0 数据捕获第1个
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;        //软件NSS
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8 ; //8分频 9MHz
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;      //高位在前
	SPI_InitStructure.SPI_CRCPolynomial = 7;        //CRC7

	SPI_Init(SPI2, &SPI_InitStructure);
	SPI_Cmd(SPI2, ENABLE);
	SPI_ReadWriteByte(0xff);   //启动传输
}



void  NRF24L01_Init(void)
{
	SPI2_Init(); //初始化SPI
	NRF24L01_CE_L();          //使能24L01
	NRF24L01_CSN_H();        //SPI片选取消
	EXTI_Config();//配置中断
	//清除接收、发送缓冲区、标志
	NRF24L01_Write_Reg(NRF24L01_FLUSH_RX,0xff);//清除RX FIFO寄存器
	NRF24L01_Write_Reg(NRF24L01_FLUSH_TX,0xff);//清除TX FIFO寄存器 
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG+NRF24L01_STATUS,0xff); //清除TX_DS或MAX_RT中断标志	
}

u8 NRF24L01_Check(void)
{
	u8 buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	u8 i;
	SPI2_SetSpeed(SPI_SPEED_8); //spi速度为9Mhz（24L01的最大SPI时钟为10Mhz）    
	NRF24L01_Write_Buf(NRF24L01_WRITE_REG+NRF24L01_TX_ADDR,buf,5);//写入5个字节的地址.
	NRF24L01_Read_Buf(NRF24L01_TX_ADDR,buf,5); //读出写入的地址  
	for(i=0;i<5;i++)
	{
		if(buf[i]!=0XA5)
			break; 
	}		
	if(i==5)
		return 1;//检测到24L01
	return 0; //检测24L01错误
}  
   
 
u8 NRF24L01_Write_Reg(u8 reg,u8 value)
{
	u8 status;
	NRF24L01_CE_L();                            //待机模式
	NRF24L01_CSN_L();                          //使能SPI传输
	status=SPI_ReadWriteByte(reg);    //发送寄存器号 
	SPI_ReadWriteByte(value);         //写入寄存器的值
	NRF24L01_CSN_H();                          //禁止SPI传输
	return(status);    //返回状态值
}
u8 NRF24L01_Read_Reg(u8 reg)
{
	u8 reg_val;    
	NRF24L01_CSN_L();                        //使能SPI传输
	SPI_ReadWriteByte(reg);         //发送寄存器号
	reg_val=SPI_ReadWriteByte(0XFF);//读取寄存器内容
	NRF24L01_CSN_H();                        //禁止SPI传输    
	return(reg_val);                //返回状态值
}

u8 NRF24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len)
{
	u8 status,u8_ctr;       
	NRF24L01_CSN_L();                             //使能SPI传输
	status=SPI_ReadWriteByte(reg);       //发送寄存器值(位置),并读取状态值 

	for(u8_ctr=0;u8_ctr<len;u8_ctr++)
		pBuf[u8_ctr]=SPI_ReadWriteByte(0XFF);//读出数据
	NRF24L01_CSN_H();                             //禁止SPI传输
	return status;                       //返回读到的状态值
}
u8 NRF24L01_Write_Byte(u8 reg, u8 pBuf)
{
	u8 status;    
	NRF24L01_CSN_L();                        //使能SPI传输
	status = SPI_ReadWriteByte(reg);//发送寄存器值(位置),并读取状态值
	SPI_ReadWriteByte(pBuf);        //写入数据  
	NRF24L01_CSN_H();                        //关闭SPI传输
	return status;                  //返回读到的状态值
}

u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
	u8 status,u8_ctr;    
	NRF24L01_CSN_L();                        //使能SPI传输
	status = SPI_ReadWriteByte(reg);//发送寄存器值(位置),并读取状态值
	for(u8_ctr=0; u8_ctr<len; u8_ctr++)
		SPI_ReadWriteByte(*pBuf++);     //写入数据  
	NRF24L01_CSN_H();                        //关闭SPI传输
	return status;                  //返回读到的状态值
}

  
u8 NRF24L01_TxPacket(u8 *txbuf)
{
	u8 sta;
	u16 Time_out_count=0;
	SPI2_SetSpeed(SPI_SPEED_8);//spi速度为9Mhz（24L01的最大SPI时钟为10Mhz）   
	NRF24L01_CE_L();
	NRF24L01_Write_Buf(NRF24L01_WR_TX_PLOAD,txbuf,NRF24L01_TX_PLOAD_WIDTH);//写数据到TX BUF  32个字节
	NRF24L01_CE_H();                 //启动发送   
	while(GPIOB->IDR&1<<11) //等待发送完成
	{
		++Time_out_count;
		if(Time_out_count>=65535)
			return 0xfe;//time out 
	}
	sta=NRF24L01_Read_Reg(NRF24L01_STATUS);  //读取状态寄存器的值   
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG+NRF24L01_STATUS,sta); //清除TX_DS或MAX_RT中断标志
	if(sta&NRF24L01_MAX_TX)//达到最大重发次数
	{
		NRF24L01_Write_Reg(NRF24L01_FLUSH_TX,0xff);//清除TX FIFO寄存器 
		return NRF24L01_MAX_TX; 
	}
	if(sta&NRF24L01_TX_OK)//发送完成
	{
		return NRF24L01_TX_OK;
	}
	return 0xff;//其他原因发送失败
}

////////////////////
///
///注意：执行完后会进入待机模式，如需进行下一次接收，必须使能CE，或者重新使用NRF24L01_RX_Mode();进入接收模式
//////////////////////
u8 NRF24L01_RxPacket(u8 *rxbuf)
{
	u8 sta;
	NRF24L01_CE_L();	//进入待机模式
	SPI2_SetSpeed(SPI_SPEED_8); //spi速度为9Mhz（24L01的最大SPI时钟为10Mhz）   
	sta=NRF24L01_Read_Reg(NRF24L01_STATUS);  //读取状态寄存器的值      
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG+NRF24L01_STATUS,sta); //清除TX_DS或MAX_RT中断标志	
	if(sta&NRF24L01_RX_OK)//接收到数据
	{
		NRF24L01_Read_Buf(NRF24L01_RD_RX_PLOAD,rxbuf,NRF24L01_RX_PLOAD_WIDTH);//读取数据
		NRF24L01_Write_Reg(NRF24L01_FLUSH_RX,0xff);//清除RX FIFO寄存器 
		return NRF24L01_RX_OK; 
	}
	
	return 0;//没收到任何数据
}    
  
void NRF24L01_RX_Mode(void)
{
	NRF24L01_CE_L();  
	NRF24L01_Write_Buf(NRF24L01_WRITE_REG+NRF24L01_RX_ADDR_P0,(u8*)RX_ADDRESS,NRF24L01_RX_ADR_WIDTH);//写RX节点地址

	NRF24L01_Write_Reg(NRF24L01_WRITE_REG+NRF24L01_EN_AA,0x01);             //使能通道0的自动应答    
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG+NRF24L01_EN_RXADDR,0x01);         //使能通道0的接收地址    
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG+NRF24L01_RF_CH,40);              //设置RF通信频率  
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG+NRF24L01_RX_PW_P0,NRF24L01_RX_PLOAD_WIDTH);//选择通道0的有效数据宽度    
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG+NRF24L01_RF_SETUP,0x0f);          //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG+NRF24L01_CONFIG, 0x0f);           //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式 
	NRF24L01_CE_H();    //CE为高,进入接收模式 
	delay_us(135);  //CE要拉高一段时间才进入发送模式  Power Down -> Standby mode:0~1.5ms  Standby modes -> TX/RX mode  0~130us  For start up time see Table 13 at nrf24l01 product specification
}  
 
void NRF24L01_TX_Mode(void)
{  
	NRF24L01_CE_L();    
	NRF24L01_Write_Buf(NRF24L01_WRITE_REG+NRF24L01_TX_ADDR,(u8*)TX_ADDRESS,NRF24L01_TX_ADR_WIDTH);//写TX节点地址 
	NRF24L01_Write_Buf(NRF24L01_WRITE_REG+NRF24L01_RX_ADDR_P0,(u8*)TX_ADDRESS,NRF24L01_RX_ADR_WIDTH); //设置RX_P0节点地址，与发送地址相同,为了接收ACK信号

	NRF24L01_Write_Reg(NRF24L01_WRITE_REG+NRF24L01_EN_AA,0x01);     //使能通道0的自动应答    
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG+NRF24L01_EN_RXADDR,0x01); //使能通道0的接收地址  
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG+NRF24L01_SETUP_RETR,0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG+NRF24L01_RF_CH,40);       //设置RF通道为40
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG+NRF24L01_RF_SETUP,0x0f);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG+NRF24L01_RX_PW_P0,NRF24L01_RX_PLOAD_WIDTH);//选择通道0的有效数据宽度    
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG+NRF24L01_CONFIG,0x0e);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
	NRF24L01_CE_H(); //CE为高,10us后启动发送
	delay_us(135);  //CE要拉高一段时间才进入发送模式  Power Down -> Standby mode:0~1.5ms  Standby modes -> TX/RX mode  0~130us  For start up time see Table 13 at nrf24l01 product specification
}  

