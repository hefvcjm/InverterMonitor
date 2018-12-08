#ifndef _ADC_H
#define _ADC_H
#include "sys.h"
#include "fft.h"
#include "user_interface.h"

//ʹ��ADC1��9��ͨ�������ж������ӷ�ʽ���£�
//          PA:0,1,4,5,6,7              PC:10,11,12
//ADC1_Channel:0,1,4,5,6,7    ADC1_Channel:10,11,12
//��ӦΪ��
// PA0:A���ѹ
// PA1:A�����
// PA4:B���ѹ
// PA5:B�����
// PA6:C���ѹ
// PA7:C�����
//PC0:�����ѹ
//PC1:�������
//PC2:�����ѹ1
//PC3:�������1
//���涨�������±�
#define ADC_A_VOL    0   //A���ѹ
#define ADC_B_VOL    1   //A�����
#define ADC_C_VOL    2   //B���ѹ
#define ADC_A_CUR    3   //B�����
#define ADC_B_CUR    4   //C���ѹ
#define ADC_C_CUR    5   //C�����
#define ADC_PV_VOL   6   //�����ѹ
#define ADC_PV_CUR   7   //�������
#define ADC_PV_VOL1  8   //�����ѹ1
#define ADC_PV_CUR1  9   //�������1

#define ADC_CH_NUM  10   //ʹ�õ�ADCͨ����Ŀ
#define SAMPLE_COUNT_CH  1   //ÿ��ͨ������������������ƽ��
#define COUNT_SAMPLE   600     //һ���ڲ�������

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



