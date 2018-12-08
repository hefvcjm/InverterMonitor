#ifndef _RS232_H
#define _RS232_H
#include "sys.h"	 								  

	  		  	
extern u8 RS232_RX_BUF[264]; 		//接收缓冲,最大264个字节
extern u16 RS232_RX_CNT;   			//接收到的数据长度

//如果想串口中断接收，请不要注释以下宏定义
#define EN_USART2_RX 	1			//0,不接收;1,接收.

void RS232_Init(u32 bound);
void RS232_Send_Data(u8 *buf,u8 len);
u8 RS232_Receive_Data(u8 *buf,u16 *len);


#endif

