/**
*
* @file LCD1602.h
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



# ifndef _LCD1602_H
# define _LCD1602_H

////////////////////////////////////////////
///@brief include file
////////////////////////////////////////////
# include "stm32f10x.h"
# include "delay.h"

//////////////////////////////////////////////
///@brief define the connection pin and clock with stm32
/////////////////////////////////////////////
//VL  Contrast Adjust --- can use pwm out , or use hardware connect  对比度调节，可以使用硬件连接，电阻分压方式，或者使用软件输出PWM来调节电压
#define LCD1602_VL_Pin   GPIO_Pin_6
#define LCD1602_VL_GPIO  GPIOB
#define LCD1602_VL_CLK   RCC_APB2Periph_GPIOB
#define LCD1602_VL_TIM   TIM4  
#define LCD1602_VL_TIM_CLK RCC_APB1Periph_TIM4
#define LCD1602_VL_USE_CHANNEL1    //Set the pwm output channel  the default value is channel 1   设定 PWM 输出通道，默认为通道一
//#define LCD1602_VL_USE_CHANNEL2
//#define LCD1602_VL_USE_CHANNEL3
//#define LCD1602_VL_USE_CHANNEL4
#define LCD1602_USE_VL_SOFT        //*********************define if use VL by output control  定义是否使用软件输出VL值 ************************

//RS   1DATA/0Command Select
#define LCD1602_RS_Pin   GPIO_Pin_14
#define LCD1602_RS_GPIO  GPIOC
#define LCD1602_RS_CLK   RCC_APB2Periph_GPIOC
//RW   1Read/0Write Select
#define LCD1602_RW_Pin   GPIO_Pin_15
#define LCD1602_RW_GPIO  GPIOC
#define LCD1602_RW_CLK   RCC_APB2Periph_GPIOC
//E    Enable Signal
#define LCD1602_EN_Pin    GPIO_Pin_0
#define LCD1602_EN_GPIO   GPIOB
#define LCD1602_EN_CLK    RCC_APB2Periph_GPIOB
//D0~D7 Data Transmission Pin
#define LCD1602_DATA0_Pin    GPIO_Pin_0
#define LCD1602_DATA0_GPIO   GPIOA
#define LCD1602_DATA0_CLK    RCC_APB2Periph_GPIOA
#define LCD1602_DATA1_Pin    GPIO_Pin_1
#define LCD1602_DATA1_GPIO   GPIOA
#define LCD1602_DATA1_CLK    RCC_APB2Periph_GPIOA
#define LCD1602_DATA2_Pin    GPIO_Pin_2
#define LCD1602_DATA2_GPIO   GPIOA
#define LCD1602_DATA2_CLK    RCC_APB2Periph_GPIOA
#define LCD1602_DATA3_Pin    GPIO_Pin_3
#define LCD1602_DATA3_GPIO   GPIOA
#define LCD1602_DATA3_CLK    RCC_APB2Periph_GPIOA
#define LCD1602_DATA4_Pin    GPIO_Pin_4
#define LCD1602_DATA4_GPIO   GPIOA
#define LCD1602_DATA4_CLK    RCC_APB2Periph_GPIOA
#define LCD1602_DATA5_Pin    GPIO_Pin_5
#define LCD1602_DATA5_GPIO   GPIOA
#define LCD1602_DATA5_CLK    RCC_APB2Periph_GPIOA
#define LCD1602_DATA6_Pin    GPIO_Pin_6
#define LCD1602_DATA6_GPIO   GPIOA
#define LCD1602_DATA6_CLK    RCC_APB2Periph_GPIOA
#define LCD1602_DATA7_Pin    GPIO_Pin_7
#define LCD1602_DATA7_GPIO   GPIOA
#define LCD1602_DATA7_CLK    RCC_APB2Periph_GPIOA
////BLA   Background LED Anode （+）  Control one of BlA and BLK is enough ,another Pin can connect to VDD or GND
//#define LCD1602_BLA_Pin  GPIO_Pin_1
//#define LCD1602_BLA_GPIO GPIOB
//#define LCD1602_BLA_CLK  RCC_APB2Periph_GPIOB
//BLK   Background LED Negative electrode ( - )
#define LCD1602_BL_Pin  GPIO_Pin_1
#define LCD1602_BL_GPIO GPIOB
#define LCD1602_BL_CLK  RCC_APB2Periph_GPIOB



////////////////////////////
///LCD1602 command
///////////////////////////
#define LCD1602_SCREEN_CLEAR 0x01             //1.64ms   Screen Clear, Set AC to 0 Cursor Reposition
#define LCD1602_CURSOR_RETURN 0x02            //1.64ms   DDRAM AD=0, Return,Content Changeless

#define LCD1602_INPUT_SET( INC_OR_DEC, SHIFT) (0x04|(INC_OR_DEC<<1)|SHIFT)
#define LCD1602_DIS_DIS_CURSOR_BLINK( isDisOn, isCursorOn, isBlinkOn) (0x08|(isDisOn<<2)|(isCursorOn<<1) | isBlinkOn) //40us Set display on/off,cursor on/off,blink on/off
#define LCD1602_SHIFT( SC, RL)    (0x10|(SC<<3)|(RL<<2))      //S/C=1: Display Shift; S/C=0: Cursor Shift R/L=1: Right Shift; R/L=0: Left Shift
#define LCD1602_FUNCTION_SET( DL, N, F) (0x20|(DL<<4)|(N<<3)|(F<<2))//Remove cursor and whole display,DDRAM changeless DL=1: 8D DL=0: 4D  N=1: 2R N=0: 1R  F=1: 5x10 Style; F=0: 5x7 Style
#define LCD1602_CGRAM_AD_SET( AD) (0x40|AD)
#define LCD1602_DDRAM_AD_SET( AD) (0x80|AD)      //line1
#define LCD1602_DDRAM_AD2_SET( AD) (0xc0|AD)    //line2
#define LCD1602_CGRAM_DDRAM_DATA_WRITE( wdata) wdata
//////////////////////////////////////////////////////////////////////////


#define LCD1602_RS_H()   GPIO_SetBits(LCD1602_RS_GPIO,LCD1602_RS_Pin);
#define LCD1602_RS_L()   GPIO_ResetBits(LCD1602_RS_GPIO,LCD1602_RS_Pin);

#define LCD1602_RW_H()   GPIO_SetBits(LCD1602_RW_GPIO,LCD1602_RW_Pin);
#define LCD1602_RW_L()   GPIO_ResetBits(LCD1602_RW_GPIO,LCD1602_RW_Pin);

#define LCD1602_EN_H()    GPIO_SetBits(LCD1602_EN_GPIO,LCD1602_EN_Pin);
#define LCD1602_EN_L()    GPIO_ResetBits(LCD1602_EN_GPIO,LCD1602_EN_Pin);

#define LCD1602_BL_ON()      GPIO_SetBits(LCD1602_BL_GPIO,LCD1602_BL_Pin);
#define LCD1602_BL_OFF()	 GPIO_ResetBits(LCD1602_BL_GPIO,LCD1602_BL_Pin);


////////////////////////////////////////////
///@brief Initialize the LCD1602 set up the stm32 clock and gpio and so on
////////////////////////////////////////////
void LCD1602_Init(void);
/////////////////////////////////////////////
///@brief  Set the VL value 
///@param[in] duty The percentage of full scale value: from 0 to 100
/////////////////////////////////////////////
void LCD1602_Set_VL(u8 duty);
////////////////////////////////////////////
///@brief Write command to LCD1602
///@param[in] cmd the command data
///@param[in] IsCheck Check LCD1602 is busy or not
///@return return 0 if Check LCD1602 busy failed , or return 1
////////////////////////////////////////////
u8 LCD1602_Wcmd(unsigned char cmd, u8 IsCheck);
////////////////////////////////////////////
///@brief Write data to LCD1602
///@param[in] data The data will write to LCD1602
///@return return 0 if Check LCD1602 busy failed , or return 1
////////////////////////////////////////////
u8 LCD1602_Wdata(unsigned char data);
////////////////////////////////////////////
///@brief Set the cursor to position(xpos,ypos)
///@param[in] xpos the x aixs position
///@param[in] ypos the y aixs position
////////////////////////////////////////////
void LCD1602_Pos(unsigned char xpos, unsigned char ypos);
////////////////////////////////////////////
///@brief Write string to the LCD1602 without set position , write from the last position
///@param[in] str The string will write to LCD1602
////////////////////////////////////////////
void LCD1602_WriteString(unsigned char str[]);
////////////////////////////////////////////
///@brief Write string to the LCD1602 with Set the cursor to position(xpos,ypos) , write from the cusor position after set position(xpos,ypos)
///@param[in] xpos the x aixs position
///@param[in] ypos the y aixs position
///@param[in] str The string will write to LCD1602
////////////////////////////////////////////
void LCD1602_WriteString_At_Pos(unsigned char xpos, unsigned char ypos,unsigned char str[]);
////////////////////////////////////////////
///@brief Write ulong data to the LCD1602 with Set the cursor to position(xpos,ypos) , write from the cusor position after set position(xpos,ypos)
///@param[in] xpos the x aixs position
///@param[in] ypos the y aixs position
///@param[in] w_data The ulong data will write to LCD1602
////////////////////////////////////////////
void LCD1602_Write_Ulong_At_Pos(unsigned char xpos, unsigned char ypos,unsigned long w_data);//在第1行或第2行写入unsigned long型数据

#endif

