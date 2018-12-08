#include "sys.h"		    
#include "dsp.h"	 
#include "delay.h"
#include "user_interface.h"
#include "timer.h"
#include "led.h"
//串口1 

#ifdef EN_USART1_RX   	//如果使能了接收

//接收缓存区 	
u8 USART1_RX_BUF[6];  	//接收缓冲,最大6个字节.
//接收到的数据长度
u8 USART1_RX_CNT=0; 		  
  
void USART1_IRQHandler(void)
{
	u8 res;	    
 	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //接收到数据
	{	 
		res =USART_ReceiveData(USART1); 	//读取接收到的数据
		if(USART1_RX_CNT<6)
		{
			USART1_RX_BUF[USART1_RX_CNT]=res;		//记录接收到的值
			USART1_RX_CNT++;						//接收数据增加1 
		} 
	}	
} 
#endif										 
//初始化IO 串口2
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率	  
void USART1_Init(u32 bound)
{  
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置
  #ifdef EN_USART1_RX		  	//如果使能了接收
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART1, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART1, ENABLE);                    //使能串口1 
	#endif
 
}

//USART1发送len个字节.
void USART1_Send_Data(u8 data)
{
		   
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	  
	USART_SendData(USART1,data);
 
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		
	USART1_RX_CNT=0;	  
}
//USART1查询接收到的数据
//buf:接收缓存首地址
//len:读到的数据长度
u8 USART1_Receive_Data(u8 *buf,u8 *len)
{
	u8 rxlen=USART1_RX_CNT;
	u8 i=0;
	*len=0;				//默认为0
	delay_ms(10);		//等待10ms,连续超过10ms没有接收到一个数据,则认为接收结束
	if(rxlen==USART1_RX_CNT&&rxlen)//接收到了数据,且接收完成了
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=USART1_RX_BUF[i];	
		}		
		*len=USART1_RX_CNT;	//记录本次数据长度
		USART1_RX_CNT=0;		//清零
		return 1;//完成接收
	}
	return 0;//未接收到数据或者未完成接收数据
}

u8 dsp_com=GET_STATE;
//操作
//com 0或1,
//    SET_WORK   0   //设置工作方式，启动或停止
//    GET_STATE  1   //获取工作状态
void dspOperation(u8 com)
{
	dsp_com=com;
}

u8 on_off;

//启动或停止
//START_UP   0xA5   //启动
//SHUT_DOWN  0xAA   //停止
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

//获取工作状态，设置到全局变量，以便控制显示和LED
void ExeState(void)
{
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
}



