/**
*@file    esp8266.h
*@version 1.0
*@author  Chen Zhangdi
*@date    2015-07-27
*@brief   The definition and function of ESP8266
*@par Copyright:
*Copyright© 2015 Neutree

*@Description:
*
*@history
  1:
		Date:2015-07-27
		Author:Chen Zhangdi
		Modification:		
*/

#ifndef _ESP8266_H
#define _ESP8266_H
//!!!!使用usart,具体使用哪个串口，在 USARTNum 中定义，为u8类型，与usart.h配合使用
//!!!!define which usart to use,USARTNum defined in usart.h


#include "usart.h"
#include "string.h"
#include "time.h"
#include "delay.h"

#define DEFAULT_PATTERN 3
#define USARTNUMBER 1         //use usart1
#define DEFAULT_TIMEOUT 50   //unit:100ms
#define DEFAULT_TIMEOUT_LONG 1200   //unit:100ms
#define ESPRECEV_BUFFER USART1Buffer


u8 recvFindAndFilter(char* target, char* begin, char* end, char* data, uint32_t timeout);
	/** 
	 * Verify ESP8266 whether live or not. 
	 *
	 * Actually, this method will send command "AT" to ESP8266 and waiting for "OK". 
	 * 
	 * @retval 1 - alive.
	 * @retval 0 - dead.
	 */
	u8 kick(void);
	/**
	 * Restart ESP8266 by "AT+RST". 
	 *
	 * This method will take 3 seconds or more. 
	 *
	 * @retval true - success.
	 * @retval false - failure.
	 */
	u8 restart(void);
	/**
	 * Get the version of AT Command Set.  
	 * 
	 * @return the char* of version.  
	 */
	u8 getVersion(char*);
	/**
	 * Start function of deep sleep.  
	 * 
	 *  @param time - the sleep time. 
	 *  @retval true - success.
	 *  @retval false - failure.
	 *  @note the feature requires hardware support. 
	 */
	u8 deepSleep(uint32_t time);
	 /**
	 * Switch the echo function.    
	 * 
	 *  @param mode - 1 start echo -0 stop echo
	 *  @retval true - success. 
	 *  @retval false - failure. 
	 *  
	 */
	u8 setEcho(uint8_t mode);
	/**
		*  Restore factory.   
		*  @retval true - success.  
		*  @retval false - failure.  
		*  @note  The operation can lead to restart the machine.  
		*/
	u8 restore(void);
	/**
	 * Set up a serial port configuration.  
	 *
	 * @param pattern -1 send "AT+UART=", -2 send "AT+UART_CUR=", -3 send "AT+UART_DEF=". 
	 * @param baudrate - the uart baudrate. 
	 * @retval true - success. 
	 * @retval false - failure. 
	 * @note  Only allows baud rate design, for the other parameters:databits- 8,stopbits -1,parity -0,flow control -0 . 
	 */
	u8 setUart(uint32_t baudrate,uint8_t pattern);
    /**
     * Set operation mode to station. 
     *
     * @param   pattern1 -1, send "AT+CWMODE_DEF?",-2,send "AT+CWMODE_CUR?",-3,send "AT+CWMODE?". 
     * @param   pattern2 -1, send "AT+CWMODE_DEF=",-2,send "AT+CWMODE_CUR=",-3,send "AT+CWMODE=". 
     * @retval true - success.
     * @retval false - failure.
     * 
     */
    u8 setOprToStation(uint8_t pattern1,uint8_t pattern2);
    
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
    u8 setOprToSoftAP(uint8_t pattern1,uint8_t pattern2);
   
    /**
     * Set operation mode to station + softap.  
     * @param   pattern1 -1, send "AT+CWMODE_DEF?",-2,send  "AT+CWMODE_CUR?",-3,send "AT+CWMODE?". 
     * @param   pattern2 -1, send "AT+CWMODE_DEF=",-2,send "AT+CWMODE_CUR=",-3,send "AT+CWMODE=". 
     * 
     * @retval true - success. 
     * @retval false - failure. 
     */
    u8 setOprToStationSoftAP(uint8_t pattern1,uint8_t pattern2);
    
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
    u8 joinAP(char* ssid, char* pwd,uint8_t pattern);
    
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
    u8 setSoftAPParam(char* ssid, char* pwd, uint8_t chl , uint8_t ecn,uint8_t pattern);
    
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
     u8 setDHCP(uint8_t mode, uint8_t en, uint8_t pattern);
     
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
     u8 setStationMac(char* mac,uint8_t pattern);
     
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
    u8 enableMUX(void);
    
    /**
     * Disable IP MUX(single connection mode). 
     *
     * In single connection mode, only one TCP or UDP communication can be builded. 
     * 
     * @retval true - success.
     * @retval false - failure.
     */
    u8 disableMUX(void);
    
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
    u8 setTCPServerTimeout(uint32_t timeout);
    
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
    u8 startTCPServer(uint32_t port);

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
    uint32_t recvMultipleModeComingmux_id(uint8_t *coming_mux_id, uint8_t *buffer, uint32_t buffer_size, uint32_t timeout);
		
		
#endif    /* #ifndef _ESP8266_H */
