#include "timer.h"
#include "user_interface.h"
#include "led.h"
#include "dsp.h"
//定时器程序，其中LED闪烁控制逻辑在这里实现
//同时与DSP通讯规约也在该程序实现

u16 timer_count=0;//中断次数，用于计算功率定时

//控制LED闪烁用到的定时器
//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器2!
void TIM2_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //使能指定的TIM2中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

	TIM_Cmd(TIM2, ENABLE);  //使能TIMx					 
}

u8 time_led=0;//控制不同LED闪烁时长
//定时器3中断服务程序
void TIM2_IRQHandler(void)   //TIM3中断
{
	time_led++;
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
		{
			TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx更新中断标志 
			switch(led_state)
			{
				case LED_NORMAL:
					if(inverter_state_select==ITEM_POWER_ON)
					{
						if(time_led>=2)
						{
							POWER=!POWER;
							time_led=0;
						}
					}
					if(inverter_state_select==ITEM_STAND_BY)
					{
						if(time_led>=4)
						{
							POWER=!POWER;
							time_led=0;
						}
					}
					if(inverter_state_select==ITEN_SOFT_START)
					{
						if(time_led>=6)
						{
							POWER=!POWER;
							time_led=0;
						}
					}
					WARN=0;
				  ERROR=0;
					break;
				case LED_WARN:
					if(time_led>=4)
					{
						WARN=!WARN;
						time_led=0;
					}
				  ERROR=0;
					break;
				case LED_ERROR:
					if(time_led>=4)
					{
						ERROR=!ERROR;
						time_led=0;
					}
				  WARN=0;
				  break;
				default:
					break;
			}
		}
		if(time_led>=100)
		{
			time_led=0;
		}
}


//计算功率用到的定时器
//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIMx					 
}
//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
		{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx更新中断标志 
		timer_count++;
		}
}


//用于DSP通讯定时
//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器4!
void TIM4_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM4中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级1级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

	TIM_Cmd(TIM4, ENABLE);  //使能TIMx					 
}

#define TIME_OUT    200  //定义超时时长
u8 step=1;//通信规约步骤
u16 time_out=0;//超时计数器
u8 len;//接收到的数据长度
u8 data[6];//缓存接收到的数据
u8 ii=0;//用于for循环中的变量
u8 state[2]={0,ITEM_POWER_ON};//初始化状态，接收到的状态和故障类型
u8 bit=0;//

//定时器3中断服务程序
void TIM4_IRQHandler(void)   //TIM3中断
{
	time_out++;
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //检查TIM4更新中断发生与否
		{
			TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx更新中断标志 
			switch(dsp_com)
			{
				case SET_WORK:
					switch(step)//4步通信步骤
					{
						case 1:
							if(time_out==1)
							{
								USART1_Send_Data(0x03);
							}
						  USART1_Receive_Data(data,&len);
						  if(time_out<=TIME_OUT)
							{
								if(len!=0 && data[0]==0x03)
								{
									time_out=0;
									step++;
								}
							}else{
								time_out=0;
								step=1;
								dsp_com=GET_STATE;
							}
							break;
						case 2:
							if(time_out==1)
							{
								USART1_Send_Data(0x00);
							}
						  USART1_Receive_Data(data,&len);
						  if(time_out<=TIME_OUT)
							{
								if(len!=0 && data[0]==0x00)
								{
									time_out=0;
									step++;
								}
							}else{
								time_out=0;
								step=1;
								dsp_com=GET_STATE;
							}
							break;
						case 3:
							if(time_out==1)
							{
								USART1_Send_Data(0x00);
							}
						  USART1_Receive_Data(data,&len);
						  if(time_out<=TIME_OUT)
							{
								if(len!=0 && data[0]==0x00)
								{
									time_out=0;
									step++;
								}
							}else{
								time_out=0;
								step=1;
								dsp_com=GET_STATE;
							}
							break;
						case 4:
							if(time_out==1)
							{
								USART1_Send_Data(on_off);
							}
						  USART1_Receive_Data(data,&len);
						  if(time_out<=TIME_OUT)
							{
								if(len!=0 && data[0]==on_off)
								{
									time_out=0;
								  step=1;
								  dsp_com=GET_STATE;
									if(on_off==START_UP)
									{
										inverter_on_off_select=ITEM_INVERTER_OFF;
									}
									if(on_off==SHUT_DOWN)
									{
										inverter_on_off_select=ITEM_INVERTER_ON;
									}
									refresh_prd=REFRESH_PRD_SLOW;
									should_clr=1;
									refresh_inverse=1;
								}
							}else{
								time_out=0;
								step=1;
								dsp_com=GET_STATE;
							}
							break;
						default:
//							time_out=0;
//							step=1;
//							dsp_com=GET_STATE;
							break;
					}
					break;
				case GET_STATE:
					switch(step)//4步通信步骤
					{
						case 1:
							if(time_out==1)
							{
								USART1_Send_Data(0x04);
							}
						  USART1_Receive_Data(data,&len);
						  if(time_out<=TIME_OUT)
							{
								if(len!=0 && data[0]==0x04)
								{
									time_out=0;
									step++;
								}
							}else{
								time_out=0;
								step=1;
							}
							break;
						case 2:
							if(time_out==1)
							{
								USART1_Send_Data(0x0a);
							}
						  USART1_Receive_Data(data,&len);
						  if(time_out<=TIME_OUT)
							{
								if(len!=0 && data[0]==0x0a)
								{
									time_out=0;
									step++;
								}
							}else{
								time_out=0;
								step=1;
							}
							break;
						case 3:
							if(time_out==1)
							{
								USART1_Send_Data(0x00);
							}
						  USART1_Receive_Data(data,&len);
						  if(time_out<=TIME_OUT)
							{
								if(len!=0 && data[0]==0x00)
								{
									time_out=0;
									step++;
								}
							}else{
								time_out=0;
								step=1;
							}
							break;
						case 4:
							if(time_out==1)
							{
								USART1_Send_Data(0x01);
							}
						  USART1_Receive_Data(data,&len);
						  if(time_out<=TIME_OUT)
							{
								if(len!=0 && data[0]==0x01)
								{
									time_out=0;
									step++;
									bit=len-1;//后面收到的位数
									if(bit>2)
									{
										bit=2;
									}
									switch(bit)
									{
										case 0:
											break;
										case 1:
											state[0]=data[1];
											bit=1;
											break;
										case 2:
											for(ii=0;ii<2;ii++)
											{
												state[ii]=data[ii+1];
											}
											time_out=0;
								      step=1;
									    bit=0;
											break;
									}
								}
							}else{
								time_out=0;
								step=1;
							}
							break;
						case 5:
							if(time_out<=TIME_OUT)
							{
								switch(bit)
								{
									case 0:
										USART1_Receive_Data(data,&len);
										if(len!=0)
										{
											if(len>2)
											{
												len=2;
											}
											for(ii=0;ii<len;ii++)
											{
												state[ii]=data[ii];
												bit++;
											}
										}
										break;
									case 1:
										USART1_Receive_Data(data,&len);
										if(len!=0)
										{
											state[1]=data[0];
											time_out=0;
								      step=1;
									    bit=0;
										}
										break;
									case 2:
										time_out=0;
								    step=1;
									  bit=0;
										break;
									default:
										break;
								}
								time_out=0;
							}else{
								time_out=0;
								step=1;
							}
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}
		}
}












