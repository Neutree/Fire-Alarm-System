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
	NRF24L01_Init();//2.4G无线初始化
	if(NRF24L01_Check())
		printf("connect to nrf24l01 success!\n");
	else
		printf("connect to nrf24l01 failed!\n");
//Sensor initialization		
	Sensor_Init();
	

	
}

u8 sendMessage_Door_Info[6]={0,0,0,0,1,0x22};//门锁信息格式，高位~低位：-5:0x22 代表门锁信息  -4：门锁编号  -3~0:需要发送的信息（状态信息）
u8 Door_Status[2]={0,0};//门状态记录
char dataTemp[MAX_QUEUE_LEN];//存放临时数据（接收用户传输过来的数据）


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
///@brief 对接收到的数据进行处理
///  数据存放在 dataTemp 中 dataTemp[0]为客户端ID  ,dataTemp[1]为数据长度  后面为数据
///
//////////////////////////////////////////
void Data_Open_Door_Cope(void)
{
	//校验  格式： 字节高~低含义：-10：起始位：0x55  -9：类型，0x22代表开锁  -8~7：用户名 -6~1:6位密码  -0; 和校验位 
	int i=0,j=0;
	u8 temp=0;
	if(dataTemp[1]>=11)//达到数据长度 
	{
		for(i=0;i<dataTemp[1];++i)//查找起始位
		{
			if(dataTemp[i+2]==0x55)
			{
				if((dataTemp[1]-i) >= 11 )//有效数据长度足够
				{
					for(j=0;j<10;++j)
						temp=temp+dataTemp[i+2+j];
					if(temp==dataTemp[12+i])//校验正确
					{
						if(dataTemp[i+3]==0x22)//开锁
						{
							if( (((u16)(dataTemp[i+4]<<8))|dataTemp[i+5]) ==1)//第一道锁
							{
								if(FirstLockSecret[0]==dataTemp[i+6] && FirstLockSecret[1]==dataTemp[i+7] && FirstLockSecret[2]==dataTemp[i+8] && FirstLockSecret[3]==dataTemp[i+9] && FirstLockSecret[4]==dataTemp[i+10] && FirstLockSecret[5]==dataTemp[i+11])
								{
									initTimeCount();//超时初始化
									sendMessage_Door_Info[0]=1;//状态信息 3表示非法尝试开启
									sendMessage_Door_Info[4]=1;//门锁编号为1
									UNLOCK_FIRST_DOOR();//开门
									NRF24L01_TX_Mode();//发送模式
									NRF24L01_TxPacket(sendMessage_Door_Info);//向监控端发送信息
									Door_Status[0]=1;
								}
								else//密码错误
								{
									sendMessage_Door_Info[0]=3;//状态信息 3表示非法尝试开启
									sendMessage_Door_Info[4]=1;//门锁编号为1
									LOCK_FIRST_DOOR();//关门
									NRF24L01_TX_Mode();//发送模式
									NRF24L01_TxPacket(sendMessage_Door_Info);//向监控端发送信息
									Door_Status[0]=0;
								}
							}
							else if((((u16)(dataTemp[i+4]<<8))|dataTemp[i+5]) ==2)//第二道锁
							{
								if(SecondLockSecret[0]==dataTemp[i+6] && SecondLockSecret[1]==dataTemp[i+7] && SecondLockSecret[2]==dataTemp[i+8] && SecondLockSecret[3]==dataTemp[i+9] && SecondLockSecret[4]==dataTemp[i+10] && SecondLockSecret[5]==dataTemp[i+11])
								{
									initTimeCount();//超时初始化
									sendMessage_Door_Info[0]=1;//状态信息 3表示非法尝试开启
									sendMessage_Door_Info[4]=2;//门锁编号为2
									UNLOCK_SECOND_DOOR();//开门
									NRF24L01_TX_Mode();//发送模式
									NRF24L01_TxPacket(sendMessage_Door_Info);//向监控端发送信息
									Door_Status[1]=1;
								}
								else//密码错误
								{
									sendMessage_Door_Info[0]=3;//状态信息 3表示非法尝试开启
									sendMessage_Door_Info[4]=2;//门锁编号为2
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
	//判断命令，执行动作
	
}
////////////////////////////////////////////////
///@brief 监测接收到的数据
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
			if(DelQueue(&USART1Buffer)=='+')//找到起始标志
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
									if(dataTemp[j]==':')//开始标志结束
									{
										if(dataTemp[j-2]!=',')//长度为两位数
										{
											dataTemp[1]=(dataTemp[j-2]-'0')*10 + (dataTemp[j-1]-'0');//长度
										}
										else if(dataTemp[j-2]==',')
										{
											dataTemp[1]= (dataTemp[j-1]-'0');//长度
										}
										time_out=0;
										while(dataTemp[1]>GetQueueSize(&USART1Buffer))//数据未接收完,等待数据接收完成
										{
											++time_out;
											if(time_out>=65535)
												return;
										}
										for(i=0;i<dataTemp[1];++i)//有效数据 保存
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
			//门状态检查，若超时，自动关闭
			if(Door_Status[0]||Door_Status[1])//有门开着
			{
				if(time_s>60)
				{					
					LOCK_SECOND_DOOR();
					LOCK_FIRST_DOOR();//关门
					Door_Status[0]=0;
					Door_Status[1]=0;
					sendMessage_Door_Info[0]=2;//状态信息 2表示关闭
					sendMessage_Door_Info[4]=1;//门锁编号为1
					NRF24L01_TX_Mode();//发送模式
					NRF24L01_TxPacket(sendMessage_Door_Info);//向监控端发送信息
					sendMessage_Door_Info[0]=2;//状态信息 2表示关闭
					sendMessage_Door_Info[4]=2;//门锁编号为2
					NRF24L01_TX_Mode();//发送模式
					NRF24L01_TxPacket(sendMessage_Door_Info);//向监控端发送信息
				}
			}
		}
}

