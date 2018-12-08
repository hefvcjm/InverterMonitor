#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "adc.h"
#include "lcd12864.h"
#include "calculate.h"
#include "exti.h"
#include "user_interface.h"
#include "bkp.h"
#include "rtc.h"
#include "ds18b20.h"
#include "eeprom.h"
#include "rs232.h"
#include "dsp.h"
#include "pgcp.h"
#include "communication.h"
#include "timer.h"
#include "wdg.h"

#define WAIT   100  //�ϵ硰��ӭ������ȴ�����ʱ��
#define TEMP_OVER   30.0  //���������¶Ƚ���
#define TEMP_BELOW  29.0  //�رշ����¶Ƚ���

u8 buff[255];//��PC��ͨ�Ž��ջ���
u16 len_buff;//��PC��ͨ�Ž����ֽڳ���

int main(void)
{
//	u8 byte[4];
	u16 wait=0;
	u16 count=0;
//	u16 temp=0;
	delay_init();	    	 //��ʱ������ʼ��	  
	Lcd12864_Init();//��ʼ��LCD12864ģ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	USART1_Init(9600);//��ʼ������1����DSPͨ�Ŵ���
	RS232_Init(9600);//��ʼ������2��RS232����PC��ͨ��
 	LED_Init();			     //LED�˿ڳ�ʼ��		 	
 	Adc_Init();		  		//ADC��ʼ��
	KEY_Init(); //��ʼ���������ƺ���
	EXTIX_Init();		 	//�ⲿ�жϳ�ʼ�������ڰ���
	BKP_Init();//�󱸼Ĵ�����ʼ��������ϵͳʱ�Ӻͷ�������
	RTC_Init();//ʵʱʱ��
	DS18B20_Init();//�¶ȴ�����
	AT24CXX_Init();//EEPROM
	Init_Display();//��ʾ���ֳ�ʼ��
	TIM2_Int_Init(4999,719);//��ʼ����ʱ��2��100KHz����Ƶ�ʣ���ʱ50ms������LED��˸
	TIM3_Int_Init(4999,71);//��ʼ����ʱ��3��1MHz����Ƶ�ʣ���ʱ5ms�������������õĶ�ʱ
	TIM4_Int_Init(4999,719);//��ʼ����ʱ��3��100KHz����Ƶ�ʣ���ʱ50ms�����ƴ���1ͨ�ų�ʱ
	IWDG_Init(4,6250);//��ʼ�����Ź���10s
	POWER=1;//��ԴLED
	WARN=0;//����LED
	ERROR=0;//����LED
	while(1)
	{
		IWDG_Feed();//ι��
		if(wait==0)
		{
			wait=1;
			DispStr(1,3,"��");
			DispStr(1,4,"ӭ");
			DispStr(2,2,"WELCOME!");
			SetPosition(3,0);
		}
		if(wait<WAIT)//�ȴ�1���ȶ�
		{
			wait++;
			delay_ms(10);
			continue;
		}
		if(mode==INVERTER_MODE)
		{
			TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx	
			USART_Cmd(USART1, ENABLE);
			if(inverter_state_select!=state[1])
			{
				inverter_state_select=state[1];
				if(display==INVERTER_SETTING_STATE)
				{
					refresh_prd=REFRESH_PRD_SLOW;
				  should_clr=1;
				  refresh_inverse=1;
				}
				if(display==INVERTER_FAULT_STATE)
				{
					if(inverter_state_select!=ITEM_FAULT)
					{
						refresh_prd=REFRESH_PRD_SLOW;
				    should_clr=1;
				    refresh_inverse=1;
						display=INVERTER_SETTING_STATE;
					}
				}
			}
			if(inverter_fault_state_select!=state[0])
			{
				if(display==INVERTER_FAULT_STATE)
				{
					refresh_prd=REFRESH_PRD_SLOW;
				  //should_clr=1;
				  refresh_inverse=1;
				}
				inverter_fault_state_select=state[0];
			}
		}else{
			TIM_Cmd(TIM4, DISABLE); 
			TIM_Cmd(TIM2, DISABLE);  
			USART_Cmd(USART1, DISABLE);
			POWER=1;
	    WARN=0;
	    ERROR=0;
		}
		Analysis();//������PC��ͨ��������ݣ���ִ�ж�Ӧ����
		if(count>=100 && completed)//100��ѭ������ADC��ɱ�־��λ���ȡ�¶�����
		{
			count=0;
			temperature=DS18B20_Get_Temp()/10.0;
		  if(temperature>TEMP_OVER){
				//�¶ȸ�������ִ�в���
			//  LED2=1;
		  }else if(temperature<TEMP_BELOW){
				//�¶ȵ����½�ִ�в���
			//  LED2=0;
		  }
		}
	  Display();//���沼��
		delay_ms(1);
		count++;
	}
}

