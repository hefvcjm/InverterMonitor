#ifndef _CALCULATE_H
#define _CALCULATE_H
#include "sys.h"

//#define PI   3.1415926   //π

//电压电流偏移量
//DC
#define VOL_DC_OFFSET   0
#define CUR_DC_OFFSET   0
//DC1
#define VOL_DC1_OFFSET   0
#define CUR_DC1_OFFSET   0
//A
#define VOL_A_OFFSET    1.6
#define CUR_A_OFFSET    1.6
//B
#define VOL_B_OFFSET    1.6
#define CUR_B_OFFSET    1.6
//C
#define VOL_C_OFFSET    1.6
#define CUR_C_OFFSET    1.6

//比例系数(电流电压降低倍数)
//DC
#define K_DC_VOL     207.418
#define K_DC_CUR     18.856
//DC1
#define K_DC1_VOL    207.418
#define K_DC1_CUR    18.856
//A
#define K_A_VOL      207.418
#define K_A_CUR      18.856
#define K_A_ACT_POW  (K_A_VOL*K_A_CUR)
//B
#define K_B_VOL      207.418
#define K_B_CUR      18.856
#define K_B_ACT_POW  (K_B_VOL*K_B_CUR)
//C
#define K_C_VOL      207.418
#define K_C_CUR      18.856
#define K_C_ACT_POW  (K_C_VOL*K_C_CUR)

//修正系数
extern float ratio;

double rms_voltage(double v[], u16 len ,double offset);
double rms_current(double i[], u16 len ,double offset);
double active_power(double cur[], double offsetI, double vol[], double offsetV, u16 len);
double power_factor(double act, double app);
double frequency(double sample[], u16 len, double offset, u32 sample_fre);
void Float2ByteArray(float x , u8* byte, u8 len);
float ByteArray2Float(u8 byte[], u8 len);

#endif
