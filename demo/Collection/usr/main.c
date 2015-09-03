# include "stm32f10x.h"
# include "delay.h"
# include "usart.h"
# include "esp8266.h"
# include "time.h"
# include "nrf24l01.h"
# include "sensor.h"
void Lock_Control_Init()
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
	Lock_Control_Init();	
	Usart_Configuration();
	Usart_NVIC_Configuration();
	TIM3_Init();
//esp8266 initialization
	delay_s(2);
	esp8266_Init();
//nrf24l01 detect
	NRF24L01_Init();//2.4G���߳�ʼ��
	if(NRF24L01_Check())
		printf("connect to nrf24l01 success!\n");
	else
		printf("connect to nrf24l01 failed!\n");
//Sensor initialization		
	Sensor_Init();
	

	
}

u8 sendMessage_Door_Info[6]={0,0,0,0,1,0x22};//������Ϣ��ʽ����λ~��λ��-5:0x22 ����������Ϣ  -4���������  -3~0:��Ҫ���͵���Ϣ��״̬��Ϣ��
u8 Door_Status[2]={0,0};//��״̬��¼
char dataTemp[MAX_QUEUE_LEN];//�����ʱ���ݣ������û�������������ݣ�


const char FirstLockSecret[6]={'1','2','3','4','5','6'};
const char SecondLockSecret[6]={'6','5','4','3','2','1'};

#define UNLOCK_FIRST_DOOR()    GPIO_SetBits(GPIOB, GPIO_Pin_8)
#define LOCK_FIRST_DOOR()      GPIO_ResetBits(GPIOB, GPIO_Pin_8)
#define UNLOCK_SECOND_DOOR()   GPIO_SetBits(GPIOB, GPIO_Pin_9)
#define LOCK_SECOND_DOOR()     GPIO_ResetBits(GPIOB, GPIO_Pin_9)
/////////////////////////////////////////////////////
////
///
///
//////////////////////////////////////////////////////
///////////////////////////////////////////
///@brief �Խ��յ������ݽ��д���
///  ���ݴ���� dataTemp �� dataTemp[0]Ϊ�ͻ���ID  ,dataTemp[1]Ϊ���ݳ���  ����Ϊ����
///
//////////////////////////////////////////
void Data_Open_Door_Cope(void)
{
	//У��  ��ʽ�� �ֽڸ�~�ͺ��壺-10����ʼλ��0x55  -9�����ͣ�0x22������  -8~7���û��� -6~1:6λ����  -0; ��У��λ 
	int i=0,j=0;
	u8 temp=0;
	if(dataTemp[1]>=11)//�ﵽ���ݳ��� 
	{
		for(i=0;i<dataTemp[1];++i)//������ʼλ
		{
			if(dataTemp[i+2]==0x55)
			{
				if((dataTemp[1]-i) >= 11 )//��Ч���ݳ����㹻
				{
					for(j=0;j<10;++j)
						temp=temp+dataTemp[i+2+j];
					if(temp==dataTemp[12+i])//У����ȷ
					{
						if(dataTemp[i+3]==0x22)//����
						{
							if( (((u16)(dataTemp[i+4]<<8))|dataTemp[i+5]) ==1)//��һ����
							{
								if(FirstLockSecret[0]==dataTemp[i+6] && FirstLockSecret[1]==dataTemp[i+7] && FirstLockSecret[2]==dataTemp[i+8] && FirstLockSecret[3]==dataTemp[i+9] && FirstLockSecret[4]==dataTemp[i+10] && FirstLockSecret[5]==dataTemp[i+11])
								{
									initTimeCount();//��ʱ��ʼ��
									sendMessage_Door_Info[0]=1;//״̬��Ϣ 3��ʾ�Ƿ����Կ���
									sendMessage_Door_Info[4]=1;//�������Ϊ1
									UNLOCK_FIRST_DOOR();//����
									NRF24L01_TX_Mode();//����ģʽ
									NRF24L01_TxPacket(sendMessage_Door_Info);//���ض˷�����Ϣ
									Door_Status[0]=1;
								}
								else//�������
								{
									sendMessage_Door_Info[0]=3;//״̬��Ϣ 3��ʾ�Ƿ����Կ���
									sendMessage_Door_Info[4]=1;//�������Ϊ1
									LOCK_FIRST_DOOR();//����
									NRF24L01_TX_Mode();//����ģʽ
									NRF24L01_TxPacket(sendMessage_Door_Info);//���ض˷�����Ϣ
									Door_Status[0]=0;
								}
							}
							else if((((u16)(dataTemp[i+4]<<8))|dataTemp[i+5]) ==2)//�ڶ�����
							{
								if(SecondLockSecret[0]==dataTemp[i+6] && SecondLockSecret[1]==dataTemp[i+7] && SecondLockSecret[2]==dataTemp[i+8] && SecondLockSecret[3]==dataTemp[i+9] && SecondLockSecret[4]==dataTemp[i+10] && SecondLockSecret[5]==dataTemp[i+11])
								{
									initTimeCount();//��ʱ��ʼ��
									sendMessage_Door_Info[0]=1;//״̬��Ϣ 3��ʾ�Ƿ����Կ���
									sendMessage_Door_Info[4]=2;//�������Ϊ2
									UNLOCK_SECOND_DOOR();//����
									NRF24L01_TX_Mode();//����ģʽ
									NRF24L01_TxPacket(sendMessage_Door_Info);//���ض˷�����Ϣ
									Door_Status[1]=1;
								}
								else//�������
								{
									sendMessage_Door_Info[0]=3;//״̬��Ϣ 3��ʾ�Ƿ����Կ���
									sendMessage_Door_Info[4]=2;//�������Ϊ2
									LOCK_SECOND_DOOR();
									NRF24L01_TX_Mode();
									NRF24L01_TxPacket(sendMessage_Door_Info);
									Door_Status[1]=1;
								}
							}
						}
					}
				}
			}
		}
	}
	//�ж����ִ�ж���
	
}
////////////////////////////////////////////////
///@brief �����յ�������
///
///
////////////////////////////////////////////////
void Data_Open_Door_Detect()
{
	u8 i,j;
	u16 time_out;
	while(GetQueueSize(&USART1Buffer)>12)//10 is the tcp transport format eg: +IPD,0,3:123     the "123" is data, "3" is the length of data    "0" is the TCP client ID
		//maybe have data in buffer
	{
			if(DelQueue(&USART1Buffer)=='+')//�ҵ���ʼ��־
			{
				if(DelQueue(&USART1Buffer)=='I')
				{
					if(DelQueue(&USART1Buffer)=='P')
					{
						if(DelQueue(&USART1Buffer)=='D')
						{
							if(DelQueue(&USART1Buffer)==',')
							{
								for(j=0;j<8;++j)
								{
									dataTemp[j]=DelQueue(&USART1Buffer);
									if(dataTemp[j]==':')//��ʼ��־����
									{
										if(dataTemp[j-2]!=',')//����Ϊ��λ��
										{
											dataTemp[1]=(dataTemp[j-2]-'0')*10 + (dataTemp[j-1]-'0');//����
										}
										else if(dataTemp[j-2]==',')
										{
											dataTemp[1]= (dataTemp[j-1]-'0');//����
										}
										time_out=0;
										while(dataTemp[1]>GetQueueSize(&USART1Buffer))//����δ������,�ȴ����ݽ������
										{
											++time_out;
											if(time_out>=65535)
												return;
										}
										for(i=0;i<dataTemp[1];++i)//��Ч���� ����
										{
											dataTemp[i+2]=DelQueue(&USART1Buffer);//data
										}
										Data_Open_Door_Cope();
										break;
									}
								}
							}
						}
					}
				}
		}
	}
}
int main()
{
		init();
		NRF24L01_TX_Mode();
		while(1)
		{
			Data_Open_Door_Detect();
			//��״̬��飬����ʱ���Զ��ر�
			if(Door_Status[0]||Door_Status[1])//���ſ���
			{
				if(time_s>60)
				{					
					LOCK_SECOND_DOOR();
					LOCK_FIRST_DOOR();//����
					Door_Status[0]=0;
					Door_Status[1]=0;
					sendMessage_Door_Info[0]=2;//״̬��Ϣ 2��ʾ�ر�
					sendMessage_Door_Info[4]=1;//�������Ϊ1
					NRF24L01_TX_Mode();//����ģʽ
					NRF24L01_TxPacket(sendMessage_Door_Info);//���ض˷�����Ϣ
					sendMessage_Door_Info[0]=2;//״̬��Ϣ 2��ʾ�ر�
					sendMessage_Door_Info[4]=2;//�������Ϊ2
					NRF24L01_TX_Mode();//����ģʽ
					NRF24L01_TxPacket(sendMessage_Door_Info);//���ض˷�����Ϣ
				}
			}
		}
}

