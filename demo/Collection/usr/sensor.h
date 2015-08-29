/**
* @file sensor.h
* @autor Neutree
* @bug 
* @date 2015-08-28
*
*
* @copyright 
*/

#ifndef _SENSOR_H
#define _SENSOR_H
///////////////////////////////////
///include file
///////////////////////////////////
#include "stm32f10x.h"

////////////////////////////////////////
///@brief sensor connection pin define
////////////////////////////////////////
///movement sensor
#define SENSOR_MOV_PIN_NUM    1
#define SENSOR_MOV_PIN    GPIO_Pin_1
#define SENSOR_MOV_GPIO   GPIOA
#define SENSOR_MOV_CLK    RCC_APB2Periph_GPIOA
#define SENSOR_MOV_NUMBER 3
///fire sensor
#define SENSOR_FIRE_PIN_NUM    15
#define SENSOR_FIRE_PIN    GPIO_Pin_15
#define SENSOR_FIRE_GPIO   GPIOC
#define SENSOR_FIRE_CLK    RCC_APB2Periph_GPIOC
#define SENSOR_FIRE_NUMBER 1

///light sensor
#define SENSOR_LIGHT_PIN_NUM    0
#define SENSOR_LIGHT_PIN    GPIO_Pin_0
#define SENSOR_LIGHT_GPIO   GPIOA
#define SENSOR_LIGHT_CLK    RCC_APB2Periph_GPIOA
#define SENSOR_LIGHT_NUMBER 2
///gas sensor
#define SENSOR_GAS_PIN_NUM    14
#define SENSOR_GAS_PIN    GPIO_Pin_14
#define SENSOR_GAS_GPIO   GPIOC
#define SENSOR_GAS_CLK    RCC_APB2Periph_GPIOC
#define SENSOR_GAS_NUMBER 0


#define _EXTI_Line(pinNum) EXTI_Line##pinNum 
#define EXTI_Line(pinNum) _EXTI_Line(pinNum) // definition transform ºê×ª»»
#define _GPIO_PortSource(port) GPIO_PortSource##port
#define aGPIO_PortSource(port) _GPIO_PortSource(port)
#define _GPIO_PinSource(pinNum) GPIO_PinSource##pinNum
#define GPIO_PinSource(pinNum) _GPIO_PinSource(pinNum)// definition transform ºê×ª»»
/////////////////////////////////////
///@brief Initialize stm32 pin and sensor
/////////////////////////////////////
void Sensor_Init(void);
////////////////////////////////////
///@brief Get sensor status
///@param[in] sensorNumber  the number of sensor that you want check status
////////////////////////////////////
u8 Sensor_Check_Status(u8 sensorNumber);

#endif
