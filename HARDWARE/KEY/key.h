#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h" 	 

//#define KEY0 PBin(3)   	//PB3
//#define KEY1 PBin(4)	//PB4 
//#define KEY2 PBin(5)	//PB5
//#define KEY3 PBin(6)	//PB6

#define KEY0  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0)//读取按键0
#define KEY1  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)//读取按键1
#define KEY2  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)//读取按键2 
#define KEY3  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)//读取按键3

 

#define KEY0_PRES 	1	//KEY0按下
#define KEY1_PRES	2	//KEY1按下
#define KEY2_PRES	3	//KEY2按下
#define KEY3_PRES   4	//KEY2按下


void KEY_Init(void);//IO初始化
//u8 KEY_Scan(u8);  	//按键扫描函数		
void Button_Up(void);
void Button_Down(void);
void Button_Cancel(void);
void Button_Ok(void);
#endif
