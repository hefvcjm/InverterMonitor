#ifndef _DSP_H
#define _DSP_H
#include "sys.h"

#define SET_WORK   0   //���ù�����ʽ��������ֹͣ
#define GET_STATE  1   //��ȡ����״̬

#define START_UP   0xA5   //����
#define SHUT_DOWN  0xAA   //ֹͣ


//����봮���жϽ��գ��벻Ҫע�����º궨��
#define EN_USART1_RX 	1			//0,������;1,����.

extern u8 dsp_com;
extern u8 on_off;

void USART1_Init(u32 bound);
void USART1_Send_Data(u8 data);
u8 USART1_Receive_Data(u8 *buf,u8 *len);
void dspOperation(u8 com);
void dspOnOff(u8 com);
void ExeState(void);



#endif


