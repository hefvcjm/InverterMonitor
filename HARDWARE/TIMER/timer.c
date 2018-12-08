#include "timer.h"
#include "user_interface.h"
#include "led.h"
#include "dsp.h"
//��ʱ����������LED��˸�����߼�������ʵ��
//ͬʱ��DSPͨѶ��ԼҲ�ڸó���ʵ��

u16 timer_count=0;//�жϴ��������ڼ��㹦�ʶ�ʱ

//����LED��˸�õ��Ķ�ʱ��
//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��2!
void TIM2_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM2�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx					 
}

u8 time_led=0;//���Ʋ�ͬLED��˸ʱ��
//��ʱ��3�жϷ������
void TIM2_IRQHandler(void)   //TIM3�ж�
{
	time_led++;
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
		{
			TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
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


//���㹦���õ��Ķ�ʱ��
//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx					 
}
//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
		{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
		timer_count++;
		}
}


//����DSPͨѶ��ʱ
//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��4!
void TIM4_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM4�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx					 
}

#define TIME_OUT    200  //���峬ʱʱ��
u8 step=1;//ͨ�Ź�Լ����
u16 time_out=0;//��ʱ������
u8 len;//���յ������ݳ���
u8 data[6];//������յ�������
u8 ii=0;//����forѭ���еı���
u8 state[2]={0,ITEM_POWER_ON};//��ʼ��״̬�����յ���״̬�͹�������
u8 bit=0;//

//��ʱ��3�жϷ������
void TIM4_IRQHandler(void)   //TIM3�ж�
{
	time_out++;
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //���TIM4�����жϷ������
		{
			TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
			switch(dsp_com)
			{
				case SET_WORK:
					switch(step)//4��ͨ�Ų���
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
					switch(step)//4��ͨ�Ų���
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
									bit=len-1;//�����յ���λ��
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












