/**
*@file    esp8266.h
*@version 1.0
*@author  
*@date    2015-07-27
*@brief   The definition and function of ESP8266
*@par Copyright:
*Copyright? 2015 Neutree
*@bug  not complete yet , but some of them is useful
*@Description:
*
*@history
		
*/


# include "esp8266.h"

		char data[MAX_QUEUE_LEN]="";//Received data
		
		
    /* 
     * Empty the buffer or UART RX.
     */
    void rx_empty(void)
		{
			InitQueue(&ESPRECEV_BUFFER);//Empty USART buffer by init function
		}
		
    /* 
     * Recvive data from uart. Return all received data if target found or timeout. 
     */
    char* recvString(char* target, uint32_t timeout)
		{
		//	u8 i;
			char temp[2]={'0','\0'};
			strcpy(data,"");
			initTimeCount();//Tnit time counter
			while((time_s*10+time_100ms)<timeout)
			{
				if(GetQueueSize(&ESPRECEV_BUFFER)>0)//Data buffer exists data
				{
					////////////////display data buffer
//					USARTNum=2;
//					printf("size:%d\n",GetQueueSize(&ESPRECEV_BUFFER));
//					if(ESPRECEV_BUFFER.MemRear>=ESPRECEV_BUFFER.MemFront)
//						for(i=ESPRECEV_BUFFER.MemFront+1;i<=ESPRECEV_BUFFER.MemRear;++i)
//							printf("i:%d---%c---\n",i,USART1Buffer.MemDataBuf[i]);
//					else
//					{
//						for(i=ESPRECEV_BUFFER.MemFront+1;i<MAX_QUEUE_LEN;++i)
//							printf("i:%d---%c---\n",i,USART1Buffer.MemDataBuf[i]);
//						for(i=0;i<=ESPRECEV_BUFFER.MemRear;++i)
//							printf("i:%d---%c---\n",i,USART1Buffer.MemDataBuf[i]);
//					}
					temp[0]=DelQueue(&ESPRECEV_BUFFER);
					if(temp[0]=='\0')
						continue;
					strcat(data,temp);
				}
				if(strstr(data,target))//Find out target data, return, or continue(If timeout permit or return current data)
					break;
			}
			return data;
		}
    
    /* 
     * Recvive data from uart. Return all received data if one of target1 and target2 found or timeout. 
     */
    char* recvString2(char* target1, char* target2, uint32_t timeout)
		{
//			u8 i;
			char temp[2]={'0','\0'};
			strcpy(data,"");
			initTimeCount();//Tnit time counter
			while((time_s*10+time_100ms)<timeout)
			{
				if(GetQueueSize(&ESPRECEV_BUFFER)>0)//Data buffer exists data
				{
					temp[0]=DelQueue(&ESPRECEV_BUFFER);
					if(temp[0]=='\0')
						continue;
					strcat(data,temp);
				}
				if(strstr(data,target1) || strstr(data,target2))//Find out target data, return, or continue(If timeout permit or return current data)
					break;
			}
//			USARTNum=2;//select usart
//			printf("time : %d\n",(time_s*10+time_100ms));
//			USARTNum=1;//select usart
			return data;
		}
    
    /* 
     * Recvive data from uart. Return all received data if one of target1, target2 and target3 found or timeout. 
     */
    char* recvString3(char* target1, char* target2, char* target3, uint32_t timeout )
		{
//			u8 i;
			char temp[2]={'0','\0'};
			strcpy(data,"");
			initTimeCount();//Tnit time counter
			while((time_s*10+time_100ms)<timeout)
			{
				if(GetQueueSize(&ESPRECEV_BUFFER)>0)//Data buffer exists data
				{
					temp[0]=DelQueue(&ESPRECEV_BUFFER);
					if(temp[0]=='\0')
						continue;
					strcat(data,temp);
				}
				if(strstr(data,target1) || strstr(data,target2) || strstr(data,target3))//Find out target data, return, or continue(If timeout permit or return current data)
					break;
			}
			return data;
		}
    
    /* 
     * Recvive data from uart and search first target. Return true if target found, false for timeout.
     */
    u8 recvFind(char* target, uint32_t timeout)
		{
			char* data_tmp = recvString(target, timeout);
			if (strstr(data_tmp,target)) {
				return 1;
			}
			return 0;
		}
    
    /* 
     * Recvive data from uart and search first target and cut out the subchar* between begin and end(excluding begin and end self). 
     * Return true if target found, false for timeout.
     */
    u8 recvFindAndFilter(char* target, char* begin, char* end, char* data, uint32_t timeout)
		{
			char* data_tmp;
			char* p=0;
			char temp[2]={'0','\0'};
			strcpy(data,"");
			data_tmp = recvString(target, timeout);
			if (strstr(data_tmp,target)) {
					if (strstr(data_tmp,begin) && strstr(data_tmp,end)) {
						for(p=strstr(data_tmp,begin)+(u16)strlen(begin);p<strstr(data_tmp,end);++p)
							{
								temp[0]=*p;
								strcat(data,temp);
							}
							return 1; 
					}
			}
			strcpy(data,data_tmp);
			return 0;
		}
    
    /*
     * Receive a package from uart. 
     *
     * @param buffer - the buffer storing data. 
     * @param buffer_size - guess what!
     * @param data_len - the length of data actually received(maybe more than buffer_size, the remained data will be abandoned).
     * @param timeout - the duration waitting data comming.
     * @param coming_mux_id - in single connection mode, should be NULL and not NULL in multiple. 
     */
    uint32_t recvPkg(uint8_t *buffer, uint32_t buffer_size, uint32_t *data_len, uint32_t timeout, uint8_t *coming_mux_id)
		{
			return 1;
		}
		
		
		u8 eAT(void)
		{
			USARTNum=USARTNUMBER;//select usart
			rx_empty();
			printf("AT\r\n");
			return recvFind("OK",DEFAULT_TIMEOUT);
		}
    u8 eATRST(void)
		{
			USARTNum=USARTNUMBER;//select usart
			rx_empty();
			printf("AT+RST\r\n");
			return recvFind("OK",DEFAULT_TIMEOUT);
		}
    u8 eATGMR(char* version)
		{
			USARTNum=USARTNUMBER;//select usart
			rx_empty();
			printf("AT+GMR\r\n");
			return recvFindAndFilter("OK", "\r\r\n", "\r\n\r\nOK", version,DEFAULT_TIMEOUT);
		}
    u8 eATGSLP(uint32_t time)
		{
			USARTNum=USARTNUMBER;//select usart
			rx_empty();
			printf("AT+GSLP=%d\r\n",time);
			return recvFind("OK",DEFAULT_TIMEOUT);
		}
    u8 eATE(uint8_t mode)
		{
			USARTNum=USARTNUMBER;//select usart
			rx_empty();
			printf("ATE%d\r\n",mode);
			return recvFind("OK",DEFAULT_TIMEOUT);
		}
    u8 eATRESTORE(void)
		{
			USARTNum=USARTNUMBER;//select usart
			rx_empty();
			printf("AT+RESTORE\r\n");
			return recvFind("OK",DEFAULT_TIMEOUT);
		}
    u8 eATSETUART(uint32_t baudrate,uint8_t pattern)
		{
			USARTNum=USARTNUMBER;//select usart
			rx_empty();
			if(pattern>3||pattern<1){
				return 0;
			}
			switch(pattern){
					case 3:
							printf("AT+UART=");
							break;
					case 2:
							printf("AT+UART_CUR=");
							break;
					case 1:
							printf("AT+UART_DEF=");
							 break;    
			}
			printf("%d,8,1,0,0\r\n",baudrate);
			if( recvFind("OK",DEFAULT_TIMEOUT))
			{
				//change MCU baudrate
				return 1;
			}
			else 
				return 0;
		}
    
    u8 qATCWMODE(uint8_t *mode,uint8_t pattern)
		{
			u8 ret=0;
			char str_mode[5];
			USARTNum=USARTNUMBER;//select usart
				if (!mode||!pattern) {
					return 0;
			}
			rx_empty();
			 switch(pattern)
			{
					case 1 :
							printf("AT+CWMODE_DEF?\r\n");
							break;
					case 2:
							printf("AT+CWMODE_CUR?\r\n");
							break;
					default:
							printf("AT+CWMODE?\r\n");
			}
			ret = recvFindAndFilter("OK", ":", "\r\n\r\nOK", str_mode,DEFAULT_TIMEOUT); 
			if (ret) {
					*mode = (uint8_t)(str_mode[0]-48);       
					return 1;
			} else {
					return 0;
			}
		}
    u8 eATCWMODE(char* list) 
		{
			USARTNum=USARTNUMBER;//select usart
			rx_empty();
			printf("AT+CWMODE=?\r\n");
			return recvFindAndFilter("OK", "+CWMODE:(", ")\r\n\r\nOK", list,DEFAULT_TIMEOUT);
		}
    u8 sATCWMODE(uint8_t mode,uint8_t pattern)
		{
			USARTNum=USARTNUMBER;//select usart
			if (!pattern) 
			{
					return 0;
			}
			rx_empty();
			 switch(pattern)
			{
					case 1:
							printf("AT+CWMODE_DEF=3\r\n");
							
							break;
					case 2:
							printf("AT+CWMODE_CUR=%d\r\n",mode);
							break;
					default:
							printf("AT+CWMODE=%d\r\n",mode);
							break;
			}
			return recvFind("OK",DEFAULT_TIMEOUT);
			
		}
    u8 qATCWJAP(char* ssid,uint8_t pattern)
		{
			USARTNum=USARTNUMBER;//select usart
			if (!pattern) 
			{
					return 0;
			}
			rx_empty();
			 switch(pattern)
			{
					case 1 :
							printf("AT+CWJAP_DEF?\r\n");
							break;
					case 2:
							printf("AT+CWJAP_CUR?\r\n");
							break;
					default:
							printf("AT+CWJAP?\r\n");
			}
			ssid = recvString2("OK", "No Ap",DEFAULT_TIMEOUT);
			if(strstr(ssid,"OK") || strstr(ssid,"No Ap"))
				return 1;
			return 0;
			
		}
    u8 sATCWJAP(char* ssid, char* pwd,uint8_t pattern)
	{
		char* data_temp;
		USARTNum=USARTNUMBER;//select usart
		if (!pattern) 
		{
				return 0;
		}
		rx_empty();
		 switch(pattern)
		{
				case 1 :
						printf("AT+CWJAP_DEF=");
						break;
				case 2:
						printf("AT+CWJAP_CUR=");
						break;
				default:
						printf("AT+CWJAP=");
		}
		printf("\"%s\",\"%s\"\r\n",ssid,pwd);
//		USARTNum=2;//select usart
//		printf("join ap :");
//		USARTNum=1;//select usart
		data_temp = recvString2("OK", "ERROR",DEFAULT_TIMEOUT);
		if(strstr(data_temp,"OK"))
			return 1;
		else//strstr(ssid,"FAIL")
			return 0;
	}
    u8 eATCWLAP(char* list);
    u8 eATCWQAP(void);
    u8 qATCWSAP(char* List,uint8_t pattern); 
    u8 sATCWSAP(char* ssid, char* pwd, uint8_t chl, uint8_t ecn,uint8_t pattern)
	{
		char* data_temp;
		USARTNum=USARTNUMBER;//select usart
		if (!pattern) 
		{
				return 0;
		}
		rx_empty();
		 switch(pattern)
		{
				case 1 :
						printf("AT+CWSAP_DEF=");
						break;
				case 2:
						printf("AT+CWSAP_CUR=");
						break;
				default:
						printf("AT+CWSAP=");
		}
		printf("\"%s\",\"%s\",%d,%d\r\n",ssid,pwd,chl,ecn);
//		USARTNum=2;//select usart
//		printf("set soft ap :");
//		USARTNum=1;//select usart
		data_temp = recvString2("OK", "ERROR",DEFAULT_TIMEOUT);
		if(strstr(data_temp,"OK"))
			return 1;
		else//strstr(ssid,"FAIL")
			return 0;
	}
    u8 eATCWLIF(char* list);
    u8 qATCWDHCP(char* List,uint8_t pattern); 
    u8 sATCWDHCP(uint8_t mode, uint8_t en, uint8_t pattern)
	{
		char* data_temp;
		USARTNum=USARTNUMBER;//select usart
		if (!pattern) 
		{
				return 0;
		}
		rx_empty();
		 switch(pattern)
		{
				case 1 :
						printf("AT+CWDHCP_DEF=");
						break;
				case 2:
						printf("AT+CWDHCP_CUR=");
						break;
				default:
						printf("AT+CWDHCP=");
		}
		printf("%d,%d\r\n",mode,en);
		data_temp = recvString2("OK", "ERROR",DEFAULT_TIMEOUT);
		if(strstr(data_temp,"OK"))
			return 1;
		else//strstr(ssid,"FAIL")
			return 0;
	}
    u8 eATCWAUTOCONN(uint8_t en);
    u8 qATCIPSTAMAC(char* mac,uint8_t pattern);
    u8 eATCIPSTAMAC(char* mac,uint8_t pattern)
	{
		USARTNum=USARTNUMBER;//select usart
		if (!pattern) 
		{
				return 0;
		}
		rx_empty();
		 switch(pattern)
		{
				case 1 :
						printf("AT+CIPSTAMAC_DEF=");
						break;
				case 2:
						printf("AT+CIPSTAMAC_CUR=");
						break;
				default:
						printf("AT+CIPSTAMAC=");
		}
		printf("\"%s\"\r\n",mac);
		return recvFind("OK",DEFAULT_TIMEOUT);
	}
    u8 qATCIPSTAIP(char* ip,uint8_t pattern);
    u8 eATCIPSTAIP(char* ip,char* gateway,char* netmask,uint8_t pattern);
    u8 qATCIPAP(char* ip,uint8_t pattern);
    u8 eATCIPAP(char* ip,uint8_t pattern);
    u8 eCWSTARTSMART(uint8_t type);
    u8 eCWSTOPSMART(void);

  
    u8 eATCIPSTATUS(char* list);
    u8 sATCIPSTARTSingle(char* type, char* addr, uint32_t port);
    u8 sATCIPSTARTMultiple(uint8_t mux_id, char* type, char* addr, uint32_t port);
    u8 sATCIPSENDSingle(const uint8_t *buffer, uint32_t len);
    u8 sATCIPSENDMultiple(uint8_t mux_id, const uint8_t *buffer, uint32_t len);
    u8 sATCIPSENDSingleFromFlash(const uint8_t *buffer, uint32_t len);
    u8 sATCIPSENDMultipleFromFlash(uint8_t mux_id, const uint8_t *buffer, uint32_t len);
    u8 sATCIPCLOSEMulitple(uint8_t mux_id);
    u8 eATCIPCLOSESingle(void);
    char* eATCIFSR()
	{
		USARTNum=USARTNUMBER;//select usart
		rx_empty();
		printf("AT+ CIFSR\r\n");
		return recvString("OK", DEFAULT_TIMEOUT);
		
	}
    u8 sATCIPMUX(uint8_t mode)
	{
		USARTNum=USARTNUMBER;//select usart
		rx_empty();
		printf("AT+CIPMUX=%d\r\n",mode);
		return recvFind("OK",DEFAULT_TIMEOUT);
	}
    u8 sATCIPSERVER(uint8_t mode, uint32_t port)
	{
		USARTNum=USARTNUMBER;//select usart
		rx_empty();
		printf("AT+CIPSERVER=%d,%d\r\n",mode,port);
		return recvFind("OK",DEFAULT_TIMEOUT);
	}
    u8 sATCIPMODE(uint8_t mode);
    u8 eATSAVETRANSLINK(uint8_t mode,char* ip,uint32_t port);
    u8 eATPING(char* ip);
    u8 sATCIPSTO(uint32_t timeout)
	{
		USARTNum=USARTNUMBER;//select usart
		rx_empty();
		printf("AT+CIPSTO=%d\r\n",timeout);
		return recvFind("OK",DEFAULT_TIMEOUT);
	}
    /*
     * +IPD,len:data
     * +IPD,id,len:data
     */
		 
		 
		 
		
		
		
	/** 
	 * Verify ESP8266 whether live or not. 
	 *
	 * Actually, this method will send command "AT" to ESP8266 and waiting for "OK". 
	 * 
	 * @retval 1 - alive.
	 * @retval 0 - dead.
	 */
	u8 kick(void)
	{
		return eAT();
	}
	/**
	 * Restart ESP8266 by "AT+RST". 
	 *
	 * This method will take 3 seconds or more. 
	 *
	 * @retval true - success.
	 * @retval false - failure.
	 */
	u8 restart(void)
	{
		u16 temp=0;
		if(eATRST())
		{
			delay_s(2);
			while(!eAT())
			{
				delay_ms(500);
				++temp;
				if(temp>10)
					return 0;
			}
			return 1;
		}
		return 0;
	}
	/**
	 * Get the version of AT Command Set.  
	 * 
	 * @return the char* of version.  
	 */
	u8 getVersion(char* version)
	{
		return eATGMR(version);
	}
	/**
	 * Start function of deep sleep.  
	 * 
	 *  @param time - the sleep time. 
	 *  @retval true - success.
	 *  @retval false - failure.
	 *  @note the feature requires hardware support. 
	 */
	u8 deepSleep(uint32_t time)
	{
		return eATGSLP(time);
	}
	 /**
	 * Switch the echo function.    
	 * 
	 *  @param mode - 1 start echo -0 stop echo
	 *  @retval true - success. 
	 *  @retval false - failure. 
	 *  
	 */
	u8 setEcho(uint8_t mode)
	{
		return eATE(mode);
	}
	/**
		*  Restore factory.   
		*  @retval true - success.  
		*  @retval false - failure.  
		*  @note  The operation can lead to restart the machine.  
		*/
	u8 restore(void)
	{
		return eATRESTORE();
	}
	/**
	 * Set up a serial port configuration.  
	 *
	 * @param pattern -1 send "AT+UART=", -2 send "AT+UART_CUR=", -3 send "AT+UART_DEF=". 
	 * @param baudrate - the uart baudrate. 
	 * @retval true - success. 
	 * @retval false - failure. 
	 * @note  Only allows baud rate design, for the other parameters:databits- 8,stopbits -1,parity -0,flow control -0 . 
	 */
	u8 setUart(uint32_t baudrate,uint8_t pattern)
	{
		return eATSETUART(baudrate,pattern);
	}
    /**
     * Set operation mode to station. 
     *
     * @param   pattern1 -1, send "AT+CWMODE_DEF?",-2,send "AT+CWMODE_CUR?",-3,send "AT+CWMODE?". 
     * @param   pattern2 -1, send "AT+CWMODE_DEF=",-2,send "AT+CWMODE_CUR=",-3,send "AT+CWMODE=". 
     * @retval true - success.
     * @retval false - failure.
     * 
     */
	 u8 setOprToStation(uint8_t pattern1,uint8_t pattern2)
	{
		return sATCWMODE(1,pattern2);
	}
    
    /**
     * Get the model values list.  
     * 
     * @return the list of model.  
     */ 
    char* getWifiModeList(void);
    
    /**
     * Set operation mode to softap.  
     * @param   pattern1 -1, send "AT+CWMODE_DEF?",-2,send "AT+CWMODE_CUR?",-3,send "AT+CWMODE?". 
     * @param   pattern2 -1, send "AT+CWMODE_DEF=",-2,send "AT+CWMODE_CUR=",-3,send "AT+CWMODE=". 
     * 
     * @retval true - success. 
     * @retval false - failure. 
     */
    u8 setOprToSoftAP(uint8_t pattern1,uint8_t pattern2)
	{
		return sATCWMODE(2,pattern2);
	}
   
    /**
     * Set operation mode to station + softap.  
     * @param   pattern1 -1, send "AT+CWMODE_DEF?",-2,send  "AT+CWMODE_CUR?",-3,send "AT+CWMODE?". 
     * @param   pattern2 -1, send "AT+CWMODE_DEF=",-2,send "AT+CWMODE_CUR=",-3,send "AT+CWMODE=". 
     * 
     * @retval true - success. 
     * @retval false - failure. 
     */
    u8 setOprToStationSoftAP(uint8_t pattern1,uint8_t pattern2)
	{
		return sATCWMODE(3,pattern2);
	}
    
    /**
     * Get the operation mode.  
     * @param   pattern1 -1, send "AT+CWMODE_DEF?",-2,send  "AT+CWMODE_CUR?",-3,send "AT+CWMODE?". 
     * 
     * @retval 0 - failure.
     * @retval 1 - mode Station.
     * @retval 2 - mode AP. 
     * @retval 3 - mode AP + station. 
     */
    uint8_t getOprMode(uint8_t pattern1);
    
    /**
     * Search available AP list and return it.
     * 
     * @return the list of available APs. 
     * @note This method will occupy a lot of memeory(hundreds of Bytes to a couple of KBytes). 
     *  Do not call this method unless you must and ensure that your board has enough memery left.
     */
    char* getAPList(void);
    
    /**
     * Search and returns the current connect AP. 
     * 
     * @param pattern -1, send "AT+CWJAP_DEF?",-2,send "AT+CWJAP_CUR?",-3,send "AT+CWJAP?". 
     * @return the ssid of AP connected now. 
     */ 
    char* getNowConecAp(uint8_t pattern);
    
    /**
     * Join in AP. 
     *
     * @param pattern -1 send "AT+CWJAP_DEF=" -2 send "AT+CWJAP_CUR=" -3 send "AT+CWJAP=". 
     * @param ssid - SSID of AP to join in. 
     * @param pwd - Password of AP to join in. 
     * @retval true - success.
     * @retval false - failure.
     * @note This method will take a couple of seconds. 
     */
    u8 joinAP(char* ssid, char* pwd,uint8_t pattern)
	{
		return sATCWJAP(ssid,pwd,pattern);
	}
    
    /**
     * Leave AP joined before. 
     *
     * @retval true - success.
     * @retval false - failure.
     */
    u8 leaveAP(void);
    
    /**
     * Set SoftAP parameters. 
     * 
     * @param pattern -1 send "AT+CWSAP_DEF=" -2 send "AT+CWSAP_CUR=" -3 send "AT+CWSAP=". 
     * @param ssid - SSID of SoftAP. 
     * @param pwd - PASSWORD of SoftAP. 
     * @param chl - the channel (1 - 13, default: 7). 
     * @param ecn - the way of encrypstion (0 - OPEN, 1 - WEP, 
     *  2 - WPA_PSK, 3 - WPA2_PSK, 4 - WPA_WPA2_PSK, default: 4). 
     * @retval true - success.
     * @retval false - failure.
     * @note This method should not be called when station mode. 
     */
    u8 setSoftAPParam(char* ssid, char* pwd, uint8_t chl , uint8_t ecn,uint8_t pattern)
	{
		return sATCWSAP(ssid,pwd,chl,ecn,pattern);
	}
    
    /**
     * get SoftAP parameters. 
     * 
     * @param pattern -1 send "AT+CWSAP_DEF?" -2 send "AT+CWSAP_CUR?" -3 send "AT+CWSAP?". 
     * @note This method should not be called when station mode. 
     */
    char* getSoftAPParam(uint8_t pattern);
    
    /**
     * Get the IP list of devices connected to SoftAP. 
     * 
     * @return the list of IP.
     * @note This method should not be called when station mode. 
     */
    char* getJoinedDeviceIP(void);
    
    /**
     * Get the current state of DHCP. 
     * 
     * @param pattern -1 send "AT+CWDHCP_DEF?" -2 send "AT+CWDHCP_CUR?"  -3 send "AT+CWDHCP?". 
     * @return the state of DHCP.
     * 
     */
    char* getDHCP(uint8_t pattern);
    
     /**
     * Set the  state of DHCP. 
     * @param pattern -1 send "AT+CWDHCP_DEF=" -2 send "AT+CWDHCP_CUR=" -3 send "AT+CWDHCP=". 
     * @param mode - set ap or set station or set ap + station. 
     * @param en - 0 disable DHCP  - 1 enable DHCP. 
     * @retval true - success.
     * @retval false - failure.
     */
     u8 setDHCP(uint8_t mode, uint8_t en, uint8_t pattern)
	 {
		 return sATCWDHCP(mode,en,pattern);
	 }
     
     /**
     * make boot automatically connected. 
     * @param en -1 enable  -0 disable. 
     * @retval true - success.
     * @retval false - failure.
     */
     u8 setAutoConnect(uint8_t en);
     
     /**
     * Get the station's MAC address. 
     * @param pattern -1 send "AT+CIPSTAMAC_DEF?=" -2 send "AT+CIPSTAMAC_CUR?" -3 send "AT+CIPSTAMAC?". 
     * @return mac address. 
     * @note This method should not be called when ap mode. 
     */
     char* getStationMac(uint8_t pattern);
     
     /**
     * Set the station's MAC address. 
     * @param pattern -1 send "AT+CIPSTAMAC_DEF=" -2 send "AT+CIPSTAMAC_CUR=" -3 send "AT+CIPSTAMAC=". 
     * @param mac - the mac address of station. 
     * @retval true - success.
     * @retval false - failure.
     */
     u8 setStationMac(char* mac,uint8_t pattern)
	 {
		 
		 return eATCIPSTAMAC(mac,pattern);
	 }
     
     /**
     * Get the station's IP. 
     * @param pattern -1 send "AT+CIPSTA_DEF?" -2 send "AT+CIPSTA_CUR?" -3 send "AT+CIPSTA?". 
     * @return the station's IP. 
     * @note This method should not be called when ap mode. 
     */
     char* getStationIp(uint8_t pattern);
     
      /**
     * Set the station's IP. 
     * @param pattern -1 send "AT+CIPSTA_DEF=" -2 send "AT+CIPSTA_CUR=" -3 send "AT+CIPSTA=". 
     * @param ip - the ip of station. 
     * @param gateway -the gateway of station. 
     * @param netmask -the netmask of station.  
     * @retval true - success.
     * @retval false - failure.
     * @note This method should not be called when ap mode. 
     */
     u8 setStationIp(char* ip,char* gateway,char* netmask,uint8_t pattern);
     
     /**
     * Get the AP's IP. 
     * @param pattern -1 send "AT+CIPAP_DEF?" -2 send "AT+CIPAP_CUR?" -3 send "AT+CIPAP?". 
     * @return ap's ip. 
     * @note This method should not be called when station mode. 
     * 
     */
     char* getAPIp(uint8_t pattern);
     
     /**
     * Set the AP IP. 
     * @param pattern -1 send "AT+CIPAP_DEF=" -2 send "AT+CIPAP_CUR=" -3 send "AT+CIPAP=". 
     * @param ip - the ip of AP. 
     * @retval true - success.
     * @retval false - failure.
     * @note This method should not be called when station mode.
     */
     u8 setAPIp(char* ip,uint8_t pattern);
     
     /**
     * start smartconfig. 
     * @param type -1:ESP_TOUCH  -2:AirKiss. 
     * @retval true - success.
     * @retval false - failure.
     */
     u8 startSmartConfig(uint8_t type);
     
     /**
     * stop smartconfig. 
     * 
     * @retval true - success.
     * @retval false - failure.
     */
     u8 stopSmartConfig(void); 
    
    /**
     * Get the current status of connection(UDP and TCP). 
     * 
     * @return the status. 
     */
    char* getIPStatus(void);
    
    /**
     * Get the IP address of ESP8266. 
     *
     * @return the IP list. 
     */
    char* getLocalIP(void);
    
    /**
     * Enable IP MUX(multiple connection mode). 
     *
     * In multiple connection mode, a couple of TCP and UDP communication can be builded. 
     * They can be distinguished by the identifier of TCP or UDP named mux_id. 
     * 
     * @retval true - success.
     * @retval false - failure.
     */
    u8 enableMUX(void)
	{
		return sATCIPMUX(1);
	}
    
    /**
     * Disable IP MUX(single connection mode). 
     *
     * In single connection mode, only one TCP or UDP communication can be builded. 
     * 
     * @retval true - success.
     * @retval false - failure.
     */
    u8 disableMUX(void)
	{
		return sATCIPMUX(0);
	}
    
    /**
     * Create TCP connection in single mode. 
     * 
     * @param addr - the IP or domain name of the target host. 
     * @param port - the port number of the target host. 
     * @retval true - success.
     * @retval false - failure.
     */
    u8 createTCP(char* addr, uint32_t port);
    
    /**
     * Release TCP connection in single mode. 
     * 
     * @retval true - success.
     * @retval false - failure.
     */
    u8 releaseTCP(void);
    
    /**
     * Register UDP port number in single mode.
     * 
     * @param addr - the IP or domain name of the target host. 
     * @param port - the port number of the target host. 
     * @retval true - success.
     * @retval false - failure.
     */
    u8 registerUDP(char* addr, uint32_t port);
    
    /**
     * Unregister UDP port number in single mode. 
     * 
     * @retval true - success.
     * @retval false - failure.
     */
    u8 unregisterUDP(void);
  
    /**
     * Create TCP connection in multiple mode. 
     * 
     * @param mux_id - the identifier of this TCP(available value: 0 - 4). 
     * @param addr - the IP or domain name of the target host. 
     * @param port - the port number of the target host. 
     * @retval true - success.
     * @retval false - failure.
     */
    u8 createTCPMutipleMode(uint8_t mux_id, char* addr, uint32_t port);
    
    /**
     * Release TCP connection in multiple mode. 
     * 
     * @param mux_id - the identifier of this TCP(available value: 0 - 4). 
     * @retval true - success.
     * @retval false - failure.
     */
    u8 releaseTCPMutipleMode(uint8_t mux_id);
    
    /**
     * Register UDP port number in multiple mode.
     * 
     * @param mux_id - the identifier of this TCP(available value: 0 - 4). 
     * @param addr - the IP or domain name of the target host. 
     * @param port - the port number of the target host. 
     * @retval true - success.
     * @retval false - failure.
     */
    u8 registerUDPMutipleMode(uint8_t mux_id, char* addr, uint32_t port);
    
    /**
     * Unregister UDP port number in multiple mode. 
     * 
     * @param mux_id - the identifier of this TCP(available value: 0 - 4). 
     * @retval true - success.
     * @retval false - failure.
     */
    u8 unregisterUDPMutipleMode(uint8_t mux_id);

    /**
     * Set the timeout of TCP Server. 
     * 
     * @param timeout - the duration for timeout by second(0 ~ 28800, default:180). 
     * @retval true - success.
     * @retval false - failure.
     */
    u8 setTCPServerTimeout(uint32_t timeout)
	{
		return sATCIPSTO(timeout);
	}
    
    /**
     * Start TCP Server(Only in multiple mode). 
     * 
     * After started, user should call method: getIPStatus to know the status of TCP connections. 
     * The methods of receiving data can be called for user's any purpose. After communication, 
     * release the TCP connection is needed by calling method: releaseTCP with mux_id. 
     *
     * @param port - the port number to listen(default: 333).
     * @retval true - success.
     * @retval false - failure.
     *
     * @see char* getIPStatus(void);
     * @see uint32_t recv(uint8_t *coming_mux_id, uint8_t *buffer, uint32_t len, uint32_t timeout);
     * @see u8 releaseTCP(uint8_t mux_id);
     */
    u8 startTCPServer(uint32_t port)
	{
		return sATCIPSERVER(1,port);
	}

    /**
     * Stop TCP Server(Only in multiple mode). 
     * 
     * @retval true - success.
     * @retval false - failure.
     */
    u8 stopTCPServer(void);
    
    /**
     *Set the module transfer mode
     * 
     * @retval true - success.
     * @retval false - failure.
     */
    u8 setCIPMODE(uint8_t mode);
    
    /**
     * Start Server(Only in multiple mode). 
     * 
     * @param port - the port number to listen(default: 333).
     * @retval true - success.
     * @retval false - failure.
     *
     * @see char* getIPStatus(void);
     * @see uint32_t recv(uint8_t *coming_mux_id, uint8_t *buffer, uint32_t len, uint32_t timeout);
     */
    u8 startServer(uint32_t port);

    /**
     * Stop Server(Only in multiple mode). 
     * 
     * @retval true - success.
     * @retval false - failure.
     */
    u8 stopServer(void);
    /**
     * Save the passthrough links
     * 
     * @retval true - success.
     * @retval false - failure.
     */
    u8 saveTransLink (uint8_t mode,char* ip,uint32_t port);
    
   /**
    * PING COMMAND. 
    * 
    * @retval true - success.
    * @retval false - failure.
    */
    u8 setPing(char* ip);

    /**
     * Send data based on TCP or UDP builded already in single mode. 
     * 
     * @param buffer - the buffer of data to send. 
     * @param len - the length of data to send. 
     * @retval true - success.
     * @retval false - failure.
     */
    u8 send(const uint8_t *buffer, uint32_t len);
            
    /**
     * Send data based on one of TCP or UDP builded already in multiple mode. 
     * 
     * @param mux_id - the identifier of this TCP(available value: 0 - 4). 
     * @param buffer - the buffer of data to send. 
     * @param len - the length of data to send. 
     * @retval true - success.
     * @retval false - failure.
     */
    u8 sendMultipleMode(uint8_t mux_id, const uint8_t *buffer, uint32_t len);
    
    /**
     * Send data based on TCP or UDP builded already in single mode. 
     * 
     * @param buffer - the buffer of data to send from flash memeory. 
     * @param len - the length of data to send. 
     * @retval true - success.
     * @retval false - failure.
     */
    u8 sendFromFlash(const uint8_t *buffer, uint32_t len);
            
    /**
     * Send data based on one of TCP or UDP builded already in multiple mode. 
     * 
     * @param mux_id - the identifier of this TCP(available value: 0 - 4). 
     * @param buffer - the buffer of data to send from flash memeory. 
     * @param len - the length of data to send. 
     * @retval true - success.
     * @retval false - failure.
     */
    u8 sendFromFlashMultipleMode(uint8_t mux_id, const uint8_t *buffer, uint32_t len);
    
    /**
     * Receive data from TCP or UDP builded already in single mode. 
     *
     * @param buffer - the buffer for storing data. 
     * @param buffer_size - the length of the buffer. 
     * @param timeout - the time waiting data. 
     * @return the length of data received actually. 
     */
    uint32_t recv(uint8_t *buffer, uint32_t buffer_size, uint32_t timeout);
    
    /**
     * Receive data from one of TCP or UDP builded already in multiple mode. 
     *
     * @param mux_id - the identifier of this TCP(available value: 0 - 4). 
     * @param buffer - the buffer for storing data. 
     * @param buffer_size - the length of the buffer. 
     * @param timeout - the time waiting data. 
     * @return the length of data received actually. 
     */
    uint32_t recvMultipleMode(uint8_t mux_id, uint8_t *buffer, uint32_t buffer_size, uint32_t timeout);
    
    /**
     * Receive data from all of TCP or UDP builded already in multiple mode. 
     *
     * After return, coming_mux_id store the id of TCP or UDP from which data coming. 
     * User should read the value of coming_mux_id and decide what next to do. 
     * 
     * @param coming_mux_id - the identifier of TCP or UDP. 
     * @param buffer - the buffer for storing data. 
     * @param buffer_size - the length of the buffer. 
     * @param timeout - the time waiting data. 
     * @return the length of data received actually. 
     */
    uint32_t recvMultipleModeComingmux_id(uint8_t *coming_mux_id, uint8_t *buffer, uint32_t buffer_size, uint32_t timeout)
		{
			return 0;
		}
		
	/**
	*
	*
	*
	*
	*/
	u8 esp8266_Init(void)
	{
		if(kick())//检测到esp8266
		{
			setEcho(0);//关闭回显
		}
		else
		{
			USARTNum=2;//select usart
			printf("can not detected esp8266!\n");
			return 0;
		}
		USARTNum=2;
		printf("now usart init...\n");
		setUart(115200,1);//set baud rate to 115200 definition
		USARTNum=2;
		printf("usart init success, now mode init...\n");
		setOprToStationSoftAP(1,1);//set to station & soft ap mode  definition
		USARTNum=2;
		printf("mode init success, now join ap ...\n");
		setStationMac("12:08:07:72:07:01",1);//set station mac address
		setDHCP(2,1,1);//enable softap and station DHCP
		if(!joinAP("Neutree","2.4G1208077207",1))//join ap 
		{
			printf("join ap failed just now!\n");
		}
		USARTNum=2;
		printf("join ap success, now setting softap...\n");
		setSoftAPParam("LibLock1","123456789",5,3,1);//set station param
		enableMUX();//enable Mux  允许多连接
		startTCPServer(345);// config server port
		setTCPServerTimeout(500);//set time out (s)  设置超时自动断开连接时间  单位：s（秒）
		USARTNum=2;
		printf("Wifi Configuration success!\n");
		InitQueue(&USART1Buffer);//清空缓冲区，等待用户数据到来
		return 1;
	}
		
		
	
