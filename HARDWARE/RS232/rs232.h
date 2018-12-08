#ifndef _RS232_H
#define _RS232_H
#include "sys.h"	 								  

	  		  	
extern u8 RS232_RX_BUF[264]; 		//���ջ���,���264���ֽ�
extern u16 RS232_RX_CNT;   			//���յ������ݳ���

//����봮���жϽ��գ��벻Ҫע�����º궨��
#define EN_USART2_RX 	1			//0,������;1,����.

void RS232_Init(u32 bound);
void RS232_Send_Data(u8 *buf,u8 len);
u8 RS232_Receive_Data(u8 *buf,u16 *len);


#endif

