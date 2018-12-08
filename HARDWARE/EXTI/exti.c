#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
#include "lcd12864.h"
#include "user_interface.h"
//�ⲿ�жϣ����ڰ���

//�ⲿ�ж�0�������
void EXTIX_Init(void)
{
 
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

	KEY_Init();	 //	�����˿ڳ�ʼ��

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��

	//GPIOE.0 �ж����Լ��жϳ�ʼ������   �½��ش���
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource0);

	EXTI_InitStructure.EXTI_Line=EXTI_Line0;	//KEY0
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

 //GPIOE.1	  �ж����Լ��жϳ�ʼ������ �½��ش��� //KEY1
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource1);
	EXTI_InitStructure.EXTI_Line=EXTI_Line1;
	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

 //GPIOE.2	  �ж����Լ��жϳ�ʼ������  �½��ش���	//KEY2
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource2);
	EXTI_InitStructure.EXTI_Line=EXTI_Line2;
	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���


 //GPIOE.3	  �ж����Լ��жϳ�ʼ������  �½��ش���	//KEY3
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);
	EXTI_InitStructure.EXTI_Line=EXTI_Line3;
	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���


	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//ʹ�ܰ���KEY0���ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); 

	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			//ʹ�ܰ���KEY1���ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);


	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;			//ʹ�ܰ���KEY2���ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//�����ȼ�1 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			//ʹ�ܰ���KEY3���ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//�����ȼ�0 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
 
}

//�ⲿ�ж�0������� 
void EXTI0_IRQHandler(void)//��
{
	delay_ms(10);//����
	if(KEY0==1)	 	 //WK_UP����
	{ 
    if(!display_flag)
		{
			Button_Up();
		}			
	}
	EXTI_ClearITPendingBit(EXTI_Line0); //���LINE0�ϵ��жϱ�־λ  
}
 
//�ⲿ�ж�1�������
void EXTI1_IRQHandler(void)//��
{
	delay_ms(10);//����
	if(KEY1==0)	  //����KEY2
	{
		if(!display_flag)
		{
			Button_Down();
		}
	}		 
	EXTI_ClearITPendingBit(EXTI_Line1);  //���LINE2�ϵ��жϱ�־λ  
}
//�ⲿ�ж�2�������
void EXTI2_IRQHandler(void)//ȡ��
{
	delay_ms(10);//����
	if(KEY2==0)	 //����KEY1
	{	
    if(!display_flag)
		{
			Button_Cancel();
		}		
	}		 
	EXTI_ClearITPendingBit(EXTI_Line2);  //���LINE3�ϵ��жϱ�־λ  
}

void EXTI3_IRQHandler(void)//ȷ��
{
	delay_ms(10);//����
	if(KEY3==0)	 //����KEY0
	{
		if(!display_flag)
		{
			Button_Ok();
		}	
	}		 
	EXTI_ClearITPendingBit(EXTI_Line3);  //���LINE4�ϵ��жϱ�־λ  
}
 
