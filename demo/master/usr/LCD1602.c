/**
*
* @file LCD1602.c
* @author Chenzhangdi
* @version v1.0
* @date 2015-08-28
* @pre First initialize the SystemClock eg:72M 
* @brief LCD1602 driver for stm32f10x 
*
* @bug
* @warning   
* @copyright 
* @attention 
*/
//////////////////////////////////
///@brief include file
/////////////////////////////////
# include "LCD1602.h"

////////////////////////////////////////////
///@brief initialize the stm32 clock and gpio and timer and so on
////////////////////////////////////////////
void LCD1602_Pin_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(LCD1602_DATA0_CLK | LCD1602_DATA1_CLK | LCD1602_DATA2_CLK | LCD1602_DATA3_CLK | LCD1602_DATA4_CLK | LCD1602_DATA5_CLK | LCD1602_DATA6_CLK,ENABLE);
	RCC_APB2PeriphClockCmd(LCD1602_EN_CLK | LCD1602_RW_CLK | LCD1602_RS_CLK | LCD1602_BL_CLK,ENABLE);
	#ifdef LCD1602_USE_VL_SOFT
	//init VL pin output mode    eg:PWM output
		RCC_APB2PeriphClockCmd(LCD1602_VL_CLK | RCC_APB2Periph_AFIO,ENABLE);
		if(LCD1602_VL_TIM==TIM1||LCD1602_VL_TIM==TIM8)
			RCC_APB2PeriphClockCmd(LCD1602_VL_TIM_CLK,ENABLE);
		else
		{
			RCC_APB1PeriphClockCmd(LCD1602_VL_TIM_CLK,ENABLE);
		}
		GPIO_InitStructure.GPIO_Pin=LCD1602_VL_Pin;
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
		GPIO_Init(LCD1602_VL_GPIO,&GPIO_InitStructure);

		//GPIO_PinRemapConfig(GPIO_Remap_TIM4,ENABLE);//复用功能重映像
		//定时器初始化  输出pwm频率为7200HZ
		TIM_TimeBaseStruct.TIM_Period=999;
		TIM_TimeBaseStruct.TIM_Prescaler=9;
		TIM_TimeBaseStruct.TIM_CounterMode=TIM_CounterMode_Up;
		TIM_TimeBaseStruct.TIM_ClockDivision=0;
		TIM_TimeBaseStruct.TIM_RepetitionCounter=0;

		TIM_TimeBaseInit(LCD1602_VL_TIM,&TIM_TimeBaseStruct);
		
		//pwm初始化
		TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
		TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
		TIM_OCInitStructure.TIM_Pulse=0;  //初始占空比为	TIM_SetCompare2(TIM4,80); TIM4->CCR2=80; 均可设置
		#ifdef LCD1602_VL_USE_CHANNEL1
			TIM_OC1Init(LCD1602_VL_TIM,&TIM_OCInitStructure);//通道一初始化
			TIM_OC1PreloadConfig(LCD1602_VL_TIM,TIM_OCPreload_Enable);//设置预装值
		#endif
		#ifdef LCD1602_VL_USE_CHANNEL2
			TIM_OC2Init(LCD1602_VL_TIM,&TIM_OCInitStructure);//通道二初始化
			TIM_OC2PreloadConfig(LCD1602_VL_TIM,TIM_OCPreload_Enable);//设置预装值
		#endif
		#ifdef LCD1602_VL_USE_CHANNEL3
			TIM_OC3Init(LCD1602_VL_TIM,&TIM_OCInitStructure);//通道二初始化
			TIM_OC3PreloadConfig(LCD1602_VL_TIM,TIM_OCPreload_Enable);//设置预装值
		#endif
		#ifdef LCD1602_VL_USE_CHANNEL4
			TIM_OC4Init(LCD1602_VL_TIM,&TIM_OCInitStructure);//通道二初始化
			TIM_OC4PreloadConfig(LCD1602_VL_TIM,TIM_OCPreload_Enable);//设置预装值
		#endif
		//TIM_ARRPreloadConfig(TIM2, ENABLE);
		TIM_Cmd(LCD1602_VL_TIM,ENABLE);//定时器使能
	#endif
	
	//Set Data SR RW EN BL to outPP mode
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//PA9时钟速度50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //
	
	GPIO_InitStructure.GPIO_Pin = LCD1602_DATA0_Pin;	
	GPIO_Init(LCD1602_DATA0_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD1602_DATA1_Pin;	
	GPIO_Init(LCD1602_DATA1_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD1602_DATA2_Pin;	
	GPIO_Init(LCD1602_DATA2_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD1602_DATA3_Pin;	
	GPIO_Init(LCD1602_DATA3_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD1602_DATA4_Pin;	
	GPIO_Init(LCD1602_DATA4_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD1602_DATA5_Pin;	
	GPIO_Init(LCD1602_DATA5_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD1602_DATA6_Pin;	
	GPIO_Init(LCD1602_DATA6_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD1602_DATA7_Pin;	
	GPIO_Init(LCD1602_DATA7_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD1602_RS_Pin;	
	GPIO_Init(LCD1602_RS_GPIO, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = LCD1602_RW_Pin;	
	GPIO_Init(LCD1602_RW_GPIO, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = LCD1602_EN_Pin;	
	GPIO_Init(LCD1602_EN_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD1602_BL_Pin;	
	GPIO_Init(LCD1602_BL_GPIO, &GPIO_InitStructure);
	
	GPIO_ResetBits(LCD1602_BL_GPIO,LCD1602_BL_Pin);  //turn off the background LED
}
////////////////////////////////////////////
///@brief Set data port to IPU mode
////////////////////////////////////////////
void LCD1602_Set_DataPort_In(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//Set Data port to IPU mode
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//PA9时钟速度50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //
	
	GPIO_InitStructure.GPIO_Pin = LCD1602_DATA0_Pin;	
	GPIO_Init(LCD1602_DATA0_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD1602_DATA1_Pin;	
	GPIO_Init(LCD1602_DATA1_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD1602_DATA2_Pin;	
	GPIO_Init(LCD1602_DATA2_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD1602_DATA3_Pin;	
	GPIO_Init(LCD1602_DATA3_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD1602_DATA4_Pin;	
	GPIO_Init(LCD1602_DATA4_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD1602_DATA5_Pin;	
	GPIO_Init(LCD1602_DATA5_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD1602_DATA6_Pin;	
	GPIO_Init(LCD1602_DATA6_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD1602_DATA7_Pin;	
	GPIO_Init(LCD1602_DATA7_GPIO, &GPIO_InitStructure);	
}
////////////////////////////////////////////
///@brief  set data port to out_PP mode
////////////////////////////////////////////
void LCD1602_Set_DataPort_Out(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//Set Data port to outPP mode
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//PA9时钟速度50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //
	
	GPIO_InitStructure.GPIO_Pin = LCD1602_DATA0_Pin;	
	GPIO_Init(LCD1602_DATA0_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD1602_DATA1_Pin;	
	GPIO_Init(LCD1602_DATA1_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD1602_DATA2_Pin;	
	GPIO_Init(LCD1602_DATA2_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD1602_DATA3_Pin;	
	GPIO_Init(LCD1602_DATA3_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD1602_DATA4_Pin;	
	GPIO_Init(LCD1602_DATA4_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD1602_DATA5_Pin;	
	GPIO_Init(LCD1602_DATA5_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD1602_DATA6_Pin;	
	GPIO_Init(LCD1602_DATA6_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD1602_DATA7_Pin;	
	GPIO_Init(LCD1602_DATA7_GPIO, &GPIO_InitStructure);
}
////////////////////////////////////////////
///@brief  read data from data port
////////////////////////////////////////////
u8 LCD1602_READ_DATA_PORT(void)
{
	LCD1602_Set_DataPort_In();//set port to input mode
	return ( (((u8)GPIO_ReadInputDataBit(LCD1602_DATA7_GPIO,LCD1602_DATA7_Pin))<<7)\
									| (((u8)GPIO_ReadInputDataBit(LCD1602_DATA6_GPIO,LCD1602_DATA6_Pin))<<6) | (((u8)GPIO_ReadInputDataBit(LCD1602_DATA5_GPIO,LCD1602_DATA5_Pin))<<5)\
									| (((u8)GPIO_ReadInputDataBit(LCD1602_DATA4_GPIO,LCD1602_DATA4_Pin))<<4) | (((u8)GPIO_ReadInputDataBit(LCD1602_DATA3_GPIO,LCD1602_DATA3_Pin))<<3)\
									| (((u8)GPIO_ReadInputDataBit(LCD1602_DATA2_GPIO,LCD1602_DATA2_Pin))<<2) | (((u8)GPIO_ReadInputDataBit(LCD1602_DATA1_GPIO,LCD1602_DATA1_Pin))<<1)\
									| (((u8)GPIO_ReadInputDataBit(LCD1602_DATA0_GPIO,LCD1602_DATA0_Pin))));
	
}	
////////////////////////////////////////////
///@brief write data to data port
////////////////////////////////////////////
void LCD1602_WRITE_DATA_PORT(u8 Write_Data)
{
	LCD1602_Set_DataPort_Out();//set port to output mode
	//bit7
	if(Write_Data&0x80)
		GPIO_SetBits(LCD1602_DATA7_GPIO,LCD1602_DATA7_Pin);
	else
		GPIO_ResetBits(LCD1602_DATA7_GPIO,LCD1602_DATA7_Pin);
	//bit6
	if(Write_Data&0x40)
		GPIO_SetBits(LCD1602_DATA6_GPIO,LCD1602_DATA6_Pin);
	else
		GPIO_ResetBits(LCD1602_DATA6_GPIO,LCD1602_DATA6_Pin);
	//bit5
	if(Write_Data&0x20)
		GPIO_SetBits(LCD1602_DATA5_GPIO,LCD1602_DATA5_Pin);
	else
		GPIO_ResetBits(LCD1602_DATA5_GPIO,LCD1602_DATA5_Pin);
	//bit4
	if(Write_Data&0x10)
		GPIO_SetBits(LCD1602_DATA4_GPIO,LCD1602_DATA4_Pin);
	else
		GPIO_ResetBits(LCD1602_DATA4_GPIO,LCD1602_DATA4_Pin);
	//bit3
	if(Write_Data&0x08)
		GPIO_SetBits(LCD1602_DATA3_GPIO,LCD1602_DATA3_Pin);
	else
		GPIO_ResetBits(LCD1602_DATA3_GPIO,LCD1602_DATA3_Pin);
	//bit2
	if(Write_Data&0x04)
		GPIO_SetBits(LCD1602_DATA2_GPIO,LCD1602_DATA2_Pin);
	else
		GPIO_ResetBits(LCD1602_DATA2_GPIO,LCD1602_DATA2_Pin);
	//bit1
	if(Write_Data&0x02)
		GPIO_SetBits(LCD1602_DATA1_GPIO,LCD1602_DATA1_Pin);
	else
		GPIO_ResetBits(LCD1602_DATA1_GPIO,LCD1602_DATA1_Pin);
	//bit0
	if(Write_Data&0x01)
		GPIO_SetBits(LCD1602_DATA0_GPIO,LCD1602_DATA0_Pin);
	else
		GPIO_ResetBits(LCD1602_DATA0_GPIO,LCD1602_DATA0_Pin);

}

/////////////////////////////////////////////
///@brief  Set the VL value 
///@param[in] duty The percentage of full scale value: from 0 to 100
/////////////////////////////////////////////
void LCD1602_Set_VL(u8 duty)
{
	#ifdef LCD1602_VL_USE_CHANNEL1
		LCD1602_VL_TIM->CCR1=duty*0.01*LCD1602_VL_TIM->ARR;
	#endif
	#ifdef LCD1602_VL_USE_CHANNEL2
		LCD1602_VL_TIM->CCR2=duty*0.01*LCD1602_VL_TIM->ARR;
	#endif
	#ifdef LCD1602_VL_USE_CHANNEL3
		LCD1602_VL_TIM->CCR3=duty*0.01*LCD1602_VL_TIM->ARR;
	#endif
	#ifdef LCD1602_VL_USE_CHANNEL4
		LCD1602_VL_TIM->CCR4=duty*0.01*LCD1602_VL_TIM->ARR;
	#endif
}
//////////////////////////////////////////////////
////LCD1602 initialization sequence
/////////////////////////////////////////////////

u8 LCD1602_Busy()
{
	u8 busy;
	u16 time_out=0;
	busy = 1;
	LCD1602_Set_DataPort_In();//set port to input mode
	while (busy)
	{
		LCD1602_RS_L();
		LCD1602_RW_H();
		LCD1602_EN_H();
		busy = (u8)GPIO_ReadInputDataBit(LCD1602_DATA7_GPIO,LCD1602_DATA7_Pin);
		delay_us(1);
		++time_out;
		if(time_out>65534)
			return 0;//time out return the wrong info
	}
	LCD1602_EN_L();
	return 1;// Busy Check is ok,now is free
}


////////////////////////////////////////////
///@brief Write command to LCD1602
///@param[in] cmd the command data
///@param[in] IsCheck Check LCD1602 is busy or not
///@return return 0 if Check LCD1602 busy failed , or return 1
////////////////////////////////////////////
u8 LCD1602_Wcmd(unsigned char cmd, u8 IsCheck)
{
	if (IsCheck)
	{
		if( !LCD1602_Busy())
			return 0;    //check failed time out
	}

	LCD1602_RS_L();
	LCD1602_RW_L();
	LCD1602_EN_H();

	LCD1602_WRITE_DATA_PORT(cmd);

	delay_us(1);
	LCD1602_EN_L();
	return 1;
}
////////////////////////////////////////////
///@brief Write data to LCD1602
///@param[in] data The data will write to LCD1602
///@return return 0 if Check LCD1602 busy failed , or return 1
////////////////////////////////////////////
u8 LCD1602_Wdata(unsigned char data)
{
	if( !LCD1602_Busy())
		return 0;    //check failed time out

	LCD1602_RS_H();
	LCD1602_RW_L();
	LCD1602_EN_H();

	LCD1602_WRITE_DATA_PORT(data);
	delay_us(1);
	LCD1602_EN_L();
	return 1;
}
////////////////////////////////////////////
///@brief Initialize the LCD1602 set up the stm32 clock and gpio and so on
////////////////////////////////////////////
void LCD1602_Init(void)
{
	LCD1602_Pin_Init();
	LCD1602_Set_VL(55);
//	LCD1602_Power_On();
	delay_ms(16);//wait time > 15ms , after VDD>4.5
	LCD1602_Wcmd(LCD1602_FUNCTION_SET(1,1,0), 0);
	delay_ms(5);//wait time > 4.1ms 
	LCD1602_Wcmd(LCD1602_FUNCTION_SET(1,1,0), 0);
	delay_ms(5); //wait time > 100us
	LCD1602_Wcmd(LCD1602_FUNCTION_SET(1,1,0), 0);
	delay_ms(5);

	LCD1602_Wcmd(LCD1602_FUNCTION_SET(1,1,0), 1);
	delay_ms(5);
	LCD1602_Wcmd(LCD1602_DIS_DIS_CURSOR_BLINK(1,0,0), 1);//显示开，关光标,关闪烁
	delay_ms(5);
	LCD1602_Wcmd(LCD1602_INPUT_SET(1,0), 1);//移动光标
	delay_ms(5);
	LCD1602_Wcmd(LCD1602_SCREEN_CLEAR, 1);//清除lcd的显示内容
	delay_ms(5);
}
////////////////////////////////////////////
///@brief Set the cursor to position(xpos,ypos)
///@param[in] xpos the x aixs position
///@param[in] ypos the y aixs position
////////////////////////////////////////////
void LCD1602_Pos(unsigned char xpos, unsigned char ypos)
{
  if (ypos == 0x01)
    LCD1602_Wcmd(LCD1602_DDRAM_AD_SET(xpos), 1);
  if (ypos == 0x02)
    LCD1602_Wcmd(LCD1602_DDRAM_AD2_SET(xpos), 1);
}

////////////////////////////////////////////
///@brief Write string to the LCD1602 without set position , write from the last position
///@param[in] str The string will write to LCD1602
////////////////////////////////////////////
void LCD1602_WriteString(unsigned char str[])
{
  unsigned char num = 0;

  while (str[num])
  {
    LCD1602_Wdata(str[num++]);
    delay_ms(1);
  }
}
////////////////////////////////////////////
///@brief Write string to the LCD1602 with Set the cursor to position(xpos,ypos) , write from the cusor position after set position(xpos,ypos)
///@param[in] xpos the x aixs position
///@param[in] ypos the y aixs position
///@param[in] str The string will write to LCD1602
////////////////////////////////////////////
void LCD1602_WriteString_At_Pos(unsigned char xpos, unsigned char ypos,unsigned char str[])
{
	LCD1602_Pos(xpos,ypos);
	LCD1602_WriteString(str);
}

////////////////////////////////////////////
///@brief Write ulong data to the LCD1602 with Set the cursor to position(xpos,ypos) , write from the cusor position after set position(xpos,ypos)
///@param[in] xpos the x aixs position
///@param[in] ypos the y aixs position
///@param[in] w_data The ulong data will write to LCD1602
////////////////////////////////////////////
void LCD1602_Write_Ulong_At_Pos(unsigned char xpos, unsigned char ypos,unsigned long w_data)//在第1行或第2行写入unsigned long型数据
{
	unsigned char temp[9]={0,0,0,0,0,0,0,0,0};
	LCD1602_Pos(xpos,ypos);
	temp[7]=w_data%10+'0';
	temp[6]=w_data%100/10+'0';
	temp[5]=w_data%1000/100+'0';
	temp[4]=w_data%10000/1000+'0';
	temp[3]=w_data%100000/10000+'0';
	temp[2]=w_data%1000000/100000+'0';
	temp[1]=w_data%10000000/1000000+'0';
	temp[0]=w_data%100000000/10000000+'0';
	LCD1602_WriteString(temp);
}


