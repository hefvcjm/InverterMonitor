 #include "adc.h"
 #include "delay.h" 
 #include "user_interface.h"
 #include "calculate.h"
 #include "fft.h"
#include "math.h"
//ADC

//#define ADC1_DR_Address    ((uint32_t)0x4001244C)

u16 Adc_ResultBuff[ADC_CH_NUM*SAMPLE_COUNT_CH];
u32 Adc_Result[ADC_CH_NUM];
double sample_dc_vol[COUNT_SAMPLE];
double sample_dc_cur[COUNT_SAMPLE];
double sample_dc1_vol[COUNT_SAMPLE];
double sample_dc1_cur[COUNT_SAMPLE];
double sample_a_vol[COUNT_SAMPLE];
double sample_a_cur[COUNT_SAMPLE];
double sample_b_vol[COUNT_SAMPLE];
double sample_b_cur[COUNT_SAMPLE];
double sample_c_vol[COUNT_SAMPLE];
double sample_c_cur[COUNT_SAMPLE];
complex fft_complex[COUNT_SAMPLE];
//u8 flag_dma=0;
//u8 flag_adc=0;
//u16 count_adc_it=0;
u16 count=0;
u8 completed=0;

void Average_Ch(void);

ADC_InitTypeDef ADC_InitStructure;
DMA_InitTypeDef DMA_InitStructure;
TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
TIM_OCInitTypeDef         TIM_OCInitStructure;
/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
  /* ADCCLK = PCLK2/2 */
  RCC_ADCCLKConfig(RCC_PCLK2_Div2); 
#else
  /* ADCCLK = PCLK2/4 */
  RCC_ADCCLKConfig(RCC_PCLK2_Div4); 
#endif
  /* Enable peripheral clocks ------------------------------------------------*/
  /* Enable DMA1 clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

  /* Enable ADC1 and GPIOC clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_TIM1, ENABLE);
}

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure PA.0,1,4,5,6,7(ADC Channel 0,1,4,5,6,7) as analog input ----6---------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* Configure PC.10,11,12,13(ADC Channel 10,11,12,13) as analog input ----4---------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;			//使能中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;	//抢占优先级2， 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;					//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能中断通道
	NVIC_Init(&NVIC_InitStructure); 
	
	NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure); 
}

//fs采样频率
void TIM_Configuration(u16 fs)
{
	/* TIM1 configuration ------------------------------------------------------*/ 
  /* Time Base configuration */
	u16 period=12000000/fs-1;
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
  TIM_TimeBaseStructure.TIM_Period = period;         //100us 
  TIM_TimeBaseStructure.TIM_Prescaler = 5;       
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
  /* TIM1 channel1 configuration in PWM mode */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                
  TIM_OCInitStructure.TIM_Pulse = (period+1)/2; 
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;         
  TIM_OC1Init(TIM1, &TIM_OCInitStructure);
}

		   
//初始化ADC
//这里我们仅以规则通道为例
//我们默认将开启通道0~3																	   
void Adc_Init(void)
{
	
	/* System clocks configuration ---------------------------------------------*/
  RCC_Configuration();

  /* GPIO configuration ------------------------------------------------------*/
  GPIO_Configuration();
	
	NVIC_Configuration();
	
	TIM_Configuration(10000);//默认10K采样率

  /* DMA1 channel1 configuration ----------------------------------------------*/
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr =(u32)&ADC1->DR;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Adc_ResultBuff;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = ADC_CH_NUM*SAMPLE_COUNT_CH;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  
  /* Enable DMA1 channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);
	
	//传输完成触发中断
	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);
  
  /* ADC1 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = ADC_CH_NUM;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel0,1,4,5,6,7,10,11,12,13 configuration */ 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 3, ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 4, ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 5, ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 6, ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 7, ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 8, ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 9, ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 10, ADC_SampleTime_71Cycles5);

  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);
  
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);
	
	/* Enable ADC1 external trigger */ 
  ADC_ExternalTrigConvCmd(ADC1, ENABLE);

  /* Enable ADC1 reset calibration register */   
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));
     
	//转换完成触发中断
	//ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
	
  /* Start ADC1 Software Conversion */ 
  //ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	
	/* TIM1 counter enable */
  TIM_Cmd(TIM1, ENABLE);
  /* TIM1 main Output Enable */
  TIM_CtrlPWMOutputs(TIM1, ENABLE);  
	
}		

//DMA中断服务函数 
void DMA1_Channel1_IRQHandler(void)
{
	
	if(DMA_GetFlagStatus(DMA1_FLAG_TC1)==SET)
	{
		//flag_dma=1;
		sample_a_vol[count]=(double)Adc_ResultBuff[0]*(3.3/4096)*vol_a_ratio;
		sample_a_cur[count]=(double)Adc_ResultBuff[1]*(3.3/4096)*cur_a_ratio;
		sample_b_vol[count]=(double)Adc_ResultBuff[2]*(3.3/4096)*vol_b_ratio;
		sample_b_cur[count]=(double)Adc_ResultBuff[3]*(3.3/4096)*cur_b_ratio;
		sample_c_vol[count]=(double)Adc_ResultBuff[4]*(3.3/4096)*vol_c_ratio;
		sample_c_cur[count]=(double)Adc_ResultBuff[5]*(3.3/4096)*cur_c_ratio;
		sample_dc_vol[count]=(double)Adc_ResultBuff[6]*(3.3/4096)*vol_dc_ratio;
		sample_dc_cur[count]=(double)Adc_ResultBuff[7]*(3.3/4096)*cur_dc_ratio;
		sample_dc1_vol[count]=(double)Adc_ResultBuff[8]*(3.3/4096)*vol_dc1_ratio;
		sample_dc1_cur[count]=(double)Adc_ResultBuff[9]*(3.3/4096)*cur_dc1_ratio;
		//取A相电压信号最为fft分析信号
		fft_complex[count].real=sample_a_vol[count]-VOL_A_OFFSET;
		fft_complex[count].imag=0;
		count++;
		if(count==COUNT_SAMPLE)
		{
			count=0;
			completed=1;
			/* TIM1 counter disable */
      TIM_Cmd(TIM1, DISABLE);
		}
	  //Average_Ch();
	  DMA_ClearITPendingBit(DMA1_FLAG_GL1);//清除通道1传输完成标志
	  DMA_ClearFlag(DMA1_FLAG_TC1);
	}
}

//ADC中断服务函数
//void ADC1_2_IRQHandler(void)
//{
//	ADC_ClearITPendingBit(ADC1,ADC_IT_EOC);
//	count_adc_it++;
//	flag_adc=1;
//	if(count_adc_it<SAMPLE_COUNT_CH)
//	{
	//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
//	}
//	if(count_adc_it>=SAMPLE_COUNT_CH)
//	{
//		count_adc_it=0;
//	}
//}

//求平均值
//void Average_Ch(void)
//{
//	int i=0;
//	int n=ADC_CH_NUM*SAMPLE_COUNT_CH;
//	for(i=0;i<ADC_CH_NUM;i++)
//	{
//		Adc_Result[i]=0;
//	}
//	for(i=0;i<n;i++)
//	{
//		Adc_Result[i%ADC_CH_NUM]+=Adc_ResultBuff[i];
//	}
//	for(i=0;i<ADC_CH_NUM;i++)
//	{
//		Adc_Result[i]/=SAMPLE_COUNT_CH;
//	}
//}

//启动DMA
//void DMA_Start(void)
//{
//	DMA_Cmd(DMA1_Channel1, DISABLE);  //关闭DMA
//	DMA_SetCurrDataCounter(DMA1_Channel1,ADC_CH_NUM*SAMPLE_COUNT_CH);//DMA通道的DMA缓存的大小
//  DMA_Cmd(DMA1_Channel1, ENABLE);  //开启DMA
//}

//获取对应通道的数字量数据
//u16 Get_Adc_Digital(u8 ch)
//{
//	if(ch>ADC_CH_NUM)
//	{
//		return 0;
//	}
//	return Adc_Result[ch];
//}

//获取对应通道的模拟量数据
//double Get_Adc_Analog(u8 ch)
//{
//	if(ch>ADC_CH_NUM)
//	{
//		return 0;
//	}
//	return (double)Adc_Result[ch]*(3.3/4096);
//}







