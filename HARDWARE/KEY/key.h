#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h" 	 

//#define KEY0 PBin(3)   	//PB3
//#define KEY1 PBin(4)	//PB4 
//#define KEY2 PBin(5)	//PB5
//#define KEY3 PBin(6)	//PB6

#define KEY0  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0)//��ȡ����0
#define KEY1  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)//��ȡ����1
#define KEY2  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)//��ȡ����2 
#define KEY3  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)//��ȡ����3

 

#define KEY0_PRES 	1	//KEY0����
#define KEY1_PRES	2	//KEY1����
#define KEY2_PRES	3	//KEY2����
#define KEY3_PRES   4	//KEY2����


void KEY_Init(void);//IO��ʼ��
//u8 KEY_Scan(u8);  	//����ɨ�躯��		
void Button_Up(void);
void Button_Down(void);
void Button_Cancel(void);
void Button_Ok(void);
#endif
