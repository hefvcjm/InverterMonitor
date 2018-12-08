#include "sys.h"		    
#include "dsp.h"	 
#include "delay.h"
#include "user_interface.h"
#include "timer.h"
#include "led.h"
//����1 

#ifdef EN_USART1_RX   	//���ʹ���˽���

//���ջ����� 	
u8 USART1_RX_BUF[6];  	//���ջ���,���6���ֽ�.
//���յ������ݳ���
u8 USART1_RX_CNT=0; 		  
  
void USART1_IRQHandler(void)
{
	u8 res;	    
 	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //���յ�����
	{	 
		res =USART_ReceiveData(USART1); 	//��ȡ���յ�������
		if(USART1_RX_CNT<6)
		{
			USART1_RX_BUF[USART1_RX_CNT]=res;		//��¼���յ���ֵ
			USART1_RX_CNT++;						//������������1 
		} 
	}	
} 
#endif										 
//��ʼ��IO ����2
//pclk1:PCLK1ʱ��Ƶ��(Mhz)
//bound:������	  
void USART1_Init(u32 bound)
{  
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������
  #ifdef EN_USART1_RX		  	//���ʹ���˽���
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 
	#endif
 
}

//USART1����len���ֽ�.
void USART1_Send_Data(u8 data)
{
		   
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	  
	USART_SendData(USART1,data);
 
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		
	USART1_RX_CNT=0;	  
}
//USART1��ѯ���յ�������
//buf:���ջ����׵�ַ
//len:���������ݳ���
u8 USART1_Receive_Data(u8 *buf,u8 *len)
{
	u8 rxlen=USART1_RX_CNT;
	u8 i=0;
	*len=0;				//Ĭ��Ϊ0
	delay_ms(10);		//�ȴ�10ms,��������10msû�н��յ�һ������,����Ϊ���ս���
	if(rxlen==USART1_RX_CNT&&rxlen)//���յ�������,�ҽ��������
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=USART1_RX_BUF[i];	
		}		
		*len=USART1_RX_CNT;	//��¼�������ݳ���
		USART1_RX_CNT=0;		//����
		return 1;//��ɽ���
	}
	return 0;//δ���յ����ݻ���δ��ɽ�������
}

u8 dsp_com=GET_STATE;
//����
//com 0��1,
//    SET_WORK   0   //���ù�����ʽ��������ֹͣ
//    GET_STATE  1   //��ȡ����״̬
void dspOperation(u8 com)
{
	dsp_com=com;
}

u8 on_off;

//������ֹͣ
//START_UP   0xA5   //����
//SHUT_DOWN  0xAA   //ֹͣ
void dspOnOff(u8 com)
{
	if(com!=START_UP && com!=SHUT_DOWN)
	{
		return;
	}
	time_out=0;
	dspOperation(SET_WORK);
	on_off=com;
}

//��ȡ����״̬�����õ�ȫ�ֱ������Ա������ʾ��LED
void ExeState(void)
{
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
}



