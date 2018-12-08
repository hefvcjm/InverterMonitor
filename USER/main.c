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

#define WAIT   100  //上电“欢迎”界面等待控制时间
#define TEMP_OVER   30.0  //开启风扇温度界限
#define TEMP_BELOW  29.0  //关闭风扇温度界限

u8 buff[255];//与PC机通信接收缓存
u16 len_buff;//与PC机通信接收字节长度

int main(void)
{
//	u8 byte[4];
	u16 wait=0;
	u16 count=0;
//	u16 temp=0;
	delay_init();	    	 //延时函数初始化	  
	Lcd12864_Init();//初始化LCD12864模块
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	USART1_Init(9600);//初始化串口1，与DSP通信串口
	RS232_Init(9600);//初始化串口2，RS232，与PC机通信
 	LED_Init();			     //LED端口初始化		 	
 	Adc_Init();		  		//ADC初始化
	KEY_Init(); //初始化按键控制函数
	EXTIX_Init();		 	//外部中断初始化，用于按键
	BKP_Init();//后备寄存器初始化，保存系统时钟和发电总量
	RTC_Init();//实时时钟
	DS18B20_Init();//温度传感器
	AT24CXX_Init();//EEPROM
	Init_Display();//显示布局初始化
	TIM2_Int_Init(4999,719);//初始化定时器2，100KHz计数频率，定时50ms，控制LED闪烁
	TIM3_Int_Init(4999,71);//初始化定时器3，1MHz计数频率，定时5ms，发电量计算用的定时
	TIM4_Int_Init(4999,719);//初始化定时器3，100KHz计数频率，定时50ms，控制串口1通信超时
	IWDG_Init(4,6250);//初始化看门狗，10s
	POWER=1;//电源LED
	WARN=0;//报警LED
	ERROR=0;//故障LED
	while(1)
	{
		IWDG_Feed();//喂狗
		if(wait==0)
		{
			wait=1;
			DispStr(1,3,"欢");
			DispStr(1,4,"迎");
			DispStr(2,2,"WELCOME!");
			SetPosition(3,0);
		}
		if(wait<WAIT)//等待1秒稳定
		{
			wait++;
			delay_ms(10);
			continue;
		}
		if(mode==INVERTER_MODE)
		{
			TIM_Cmd(TIM4, ENABLE);  //使能TIMx	
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
		Analysis();//解析与PC机通信相关数据，并执行对应操作
		if(count>=100 && completed)//100次循环且在ADC完成标志置位后读取温度数据
		{
			count=0;
			temperature=DS18B20_Get_Temp()/10.0;
		  if(temperature>TEMP_OVER){
				//温度高于上限执行操作
			//  LED2=1;
		  }else if(temperature<TEMP_BELOW){
				//温度低于下界执行操作
			//  LED2=0;
		  }
		}
	  Display();//界面布局
		delay_ms(1);
		count++;
	}
}

