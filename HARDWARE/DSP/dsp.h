#ifndef _DSP_H
#define _DSP_H
#include "sys.h"

#define SET_WORK   0   //设置工作方式，启动或停止
#define GET_STATE  1   //获取工作状态

#define START_UP   0xA5   //启动
#define SHUT_DOWN  0xAA   //停止


//如果想串口中断接收，请不要注释以下宏定义
#define EN_USART1_RX 	1			//0,不接收;1,接收.

extern u8 dsp_com;
extern u8 on_off;

void USART1_Init(u32 bound);
void USART1_Send_Data(u8 data);
u8 USART1_Receive_Data(u8 *buf,u8 *len);
void dspOperation(u8 com);
void dspOnOff(u8 com);
void ExeState(void);



#endif


