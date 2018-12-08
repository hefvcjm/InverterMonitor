#ifndef _FFT_H
#define _FFT_H
#include "sys.h"

typedef struct complex //复数
{
	float real;       //实部 
	float imag;       //虚部
}complex;

#define PI 3.1415926

//快速傅里叶变换
void fft(int N, complex *f);

#endif
