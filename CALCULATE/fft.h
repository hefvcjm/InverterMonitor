#ifndef _FFT_H
#define _FFT_H
#include "sys.h"

typedef struct complex //����
{
	float real;       //ʵ�� 
	float imag;       //�鲿
}complex;

#define PI 3.1415926

//���ٸ���Ҷ�任
void fft(int N, complex *f);

#endif
