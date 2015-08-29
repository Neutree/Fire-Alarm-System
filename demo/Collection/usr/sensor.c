/**
* @file sensor.c
* @autor Neutree
* @bug 
* @date 2015-08-28
*
*
* @copyright 
*/


#include "sensor.h"


/////////////////////////////////////
///@brief Initialize stm32 pin and sensor  !!!!!!!!******The EXTI IRQ is in EXTI.c********!!!!!
/////////////////////////////////////
void Sensor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	RCC_APB2PeriphClockCmd(SENSOR_LIGHT_CLK | SENSOR_GAS_CLK | SENSOR_MOV_CLK | SENSOR_FIRE_CLK | RCC_APB2Periph_AFIO,ENABLE);
	
	//
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	
	GPIO_InitStructure.GPIO_Pin=SENSOR_MOV_PIN;
	GPIO_Init(SENSOR_MOV_GPIO,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=SENSOR_FIRE_PIN;
	GPIO_Init(SENSOR_FIRE_GPIO,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=SENSOR_LIGHT_PIN;
	GPIO_Init(SENSOR_LIGHT_GPIO,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=SENSOR_GAS_PIN;
	GPIO_Init(SENSOR_GAS_GPIO,&GPIO_InitStructure);
	
	

	
	EXTI_ClearITPendingBit(EXTI_Line(SENSOR_GAS_PIN_NUM));//����жϱ�־
	GPIO_EXTILineConfig( GPIO_PortSourceGPIOC, GPIO_PinSource(SENSOR_GAS_PIN_NUM)); //����ӳ�䵽�ⲿ�ж�����
	EXTI_ClearITPendingBit(EXTI_Line(SENSOR_FIRE_PIN_NUM));							//����жϱ�־
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource(SENSOR_FIRE_PIN_NUM)); //����ӳ�䵽�ⲿ�ж�����
	EXTI_ClearITPendingBit(EXTI_Line(SENSOR_LIGHT_PIN_NUM));							//����жϱ�־
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource(SENSOR_LIGHT_PIN_NUM)); //����ӳ�䵽�ⲿ�ж�����
	EXTI_ClearITPendingBit(EXTI_Line(SENSOR_MOV_PIN_NUM));							//����жϱ�־
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource(SENSOR_MOV_PIN_NUM)); //����ӳ�䵽�ⲿ�ж�����
	
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;		//ģʽΪ�ж�ģʽ  ��һ�����¼�
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising_Falling; 
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line(SENSOR_MOV_PIN_NUM);
	EXTI_Init(&EXTI_InitStructure);
	EXTI_InitStructure.EXTI_Line=EXTI_Line(SENSOR_FIRE_PIN_NUM);
	EXTI_Init(&EXTI_InitStructure);
	EXTI_InitStructure.EXTI_Line=EXTI_Line(SENSOR_LIGHT_PIN_NUM);
	EXTI_Init(&EXTI_InitStructure);
	EXTI_InitStructure.EXTI_Line=EXTI_Line(SENSOR_GAS_PIN_NUM);
	EXTI_Init(&EXTI_InitStructure);
	

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //�ж�������λ��ʾ����0����ռʽ���ȼ�0λ�������ȼ�4λ����������������4����ռʽ���ȼ�4λ�������ȼ�0λ\
// //	ȡֵ������±�
//	 The table below gives the allowed values of the pre-emption priority and subpriority according
// to the Priority Grouping configuration performed by NVIC_PriorityGroupConfig function
//  ============================================================================================================================
//    NVIC_PriorityGroup   | NVIC_IRQChannelPreemptionPriority | NVIC_IRQChannelSubPriority  | Description
//  ============================================================================================================================
//   NVIC_PriorityGroup_0  |                0                  |            0-15             |   0 bits for pre-emption priority
//                         |                                   |                             |   4 bits for subpriority
//  ----------------------------------------------------------------------------------------------------------------------------
//   NVIC_PriorityGroup_1  |                0-1                |            0-7              |   1 bits for pre-emption priority
//                         |                                   |                             |   3 bits for subpriority
//  ----------------------------------------------------------------------------------------------------------------------------    
//   NVIC_PriorityGroup_2  |                0-3                |            0-3              |   2 bits for pre-emption priority
//                         |                                   |                             |   2 bits for subpriority
//  ----------------------------------------------------------------------------------------------------------------------------    
//   NVIC_PriorityGroup_3  |                0-7                |            0-1              |   3 bits for pre-emption priority
//                         |                                   |                             |   1 bits for subpriority
//  ----------------------------------------------------------------------------------------------------------------------------    
//   NVIC_PriorityGroup_4  |                0-15               |            0                |   4 bits for pre-emption priority
//                         |                                   |                             |   0 bits for subpriority                       
//  ============================================================================================================================
//*/
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; //ע�⣺�ж���5~15���жϺ������£���1~4��ͬ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn; //ע�⣺�ж���5~15���жϺ������£���1~4��ͬ
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn; //ע�⣺�ж���5~15���жϺ������£���1~4��ͬ
	NVIC_Init(&NVIC_InitStructure);
}
////////////////////////////////////
///@brief Get sensor status
///@param[in] sensorNumber  the number of sensor that you want check status
////////////////////////////////////
u8 Sensor_Check_Status(u8 sensorNumber)
{
	u8 status;
	if(SENSOR_GAS_NUMBER==sensorNumber)
		status=SENSOR_GAS_GPIO->IDR&1<<SENSOR_GAS_PIN_NUM;
	else if(SENSOR_MOV_NUMBER==sensorNumber)
		status=SENSOR_MOV_GPIO->IDR&1<<SENSOR_MOV_PIN_NUM;
	else if(SENSOR_LIGHT_NUMBER==sensorNumber)
		status=SENSOR_LIGHT_GPIO->IDR&1<<SENSOR_LIGHT_PIN_NUM;
	else if(SENSOR_FIRE_NUMBER==sensorNumber)
		status=SENSOR_FIRE_GPIO->IDR&1<<SENSOR_FIRE_PIN_NUM;
	return status;
}
