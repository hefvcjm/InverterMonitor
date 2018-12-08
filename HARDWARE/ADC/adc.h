#ifndef _ADC_H
#define _ADC_H
#include "sys.h"
#include "fft.h"
#include "user_interface.h"

//使用ADC1的9个通道，其中定义连接方式如下：
//          PA:0,1,4,5,6,7              PC:10,11,12
//ADC1_Channel:0,1,4,5,6,7    ADC1_Channel:10,11,12
//对应为：
// PA0:A相电压
// PA1:A相电流
// PA4:B相电压
// PA5:B相电流
// PA6:C相电压
// PA7:C相电流
//PC0:光伏电压
//PC1:光伏电流
//PC2:光伏电压1
//PC3:光伏电流1
//下面定义数组下标
#define ADC_A_VOL    0   //A相电压
#define ADC_B_VOL    1   //A相电流
#define ADC_C_VOL    2   //B相电压
#define ADC_A_CUR    3   //B相电流
#define ADC_B_CUR    4   //C相电压
#define ADC_C_CUR    5   //C相电流
#define ADC_PV_VOL   6   //光伏电压
#define ADC_PV_CUR   7   //光伏电流
#define ADC_PV_VOL1  8   //光伏电压1
#define ADC_PV_CUR1  9   //光伏电流1

#define ADC_CH_NUM  10   //使用的ADC通道数目
#define SAMPLE_COUNT_CH  1   //每个通道采样次数，用于求平均
#define COUNT_SAMPLE   600     //一周期采样次数

extern u16 Adc_ResultBuff[ADC_CH_NUM*SAMPLE_COUNT_CH];
extern u32 Adc_Result[ADC_CH_NUM];
extern double sample_dc_vol[COUNT_SAMPLE];
extern double sample_dc_cur[COUNT_SAMPLE];
extern double sample_dc1_vol[COUNT_SAMPLE];
extern double sample_dc1_cur[COUNT_SAMPLE];
extern double sample_a_vol[COUNT_SAMPLE];
extern double sample_a_cur[COUNT_SAMPLE];
extern double sample_b_vol[COUNT_SAMPLE];
extern double sample_b_cur[COUNT_SAMPLE];
extern double sample_c_vol[COUNT_SAMPLE];
extern double sample_c_cur[COUNT_SAMPLE];
extern complex fft_complex[COUNT_SAMPLE];
extern u8 flag_dma;
extern u8 flag_adc;
extern u16 count_adc_it;
extern u8 completed;

void TIM_Configuration(u16 fs);
void Adc_Init(void);
//void DMA_Start(void);
//u16 Get_Adc_Digital(u8 ch);
//double Get_Adc_Analog(u8 ch);


#endif



