#include "calculate.h"
#include "math.h"
#include "sys.h"

//计算方均根
//@param sample[]  数组（向量）
//@param len  数组长度
double calculate_rms(double sample[], u16 len, double offset)
{
	double temp = 0;
	u16 i = 0;
	if(len == 0)
	{
		return 0;
	}
	for(i=0;i<len;i++)
	{
		temp += (sample[i]-offset) * (sample[i]-offset);
	}
	temp = temp / len;
	temp = sqrt(temp);
	return temp;
}

//计算点积（数量积）
//@param a[]  数组（向量）
//@param b[]  数组（向量）
//@param len  数组长度
double calculate_dot_product(double a[], double offsetA, double b[], double offsetB, u16 len)
{
	double temp = 0;
	u16 i = 0;
	if(len == 0)
	{
		return 0;
	}
	for(i=0;i<len;i++)
	{
		temp += (a[i]-offsetA) * (b[i]-offsetB);
	}
	return temp;
}

//计算电压有效值
//@param v[]  数组（向量）
//@param len  数组长度
double rms_voltage(double v[], u16 len ,double offset)
{
	return calculate_rms(v, len, offset);
}

//计算电压有效值
//@param i[]  数组（向量）
//@param len  数组长度
double rms_current(double i[], u16 len ,double offset)
{
	return calculate_rms(i, len, offset);
}

//计算点积（数量积）
//@param cur[]  数组（向量）（电流）
//@param vol[]  数组（向量）（电压）//位置不能反过来
//@param len  数组长度
double active_power(double cur[], double offsetI, double vol[], double offsetV, u16 len)
{
	return calculate_dot_product(cur, offsetI, vol, offsetV, len) / len;
}

//功率因数
double power_factor(double act, double app)
{
	double temp;
	temp=act/app;
	if(temp<-1)
	{
		temp=-1;
	}else if(temp>1)
	{
		temp=1;
	}
	return temp;
}

//计算周期
//@param sample 采样数组
//@param len    数组长度
//@param offset 偏移量
//@paran sample_fre  采样频率
double frequency(double sample[], u16 len, double offset, u32 sample_fre)
{
	u16 i=0;
	u16 start=0;
	u16 end=0;
	u8 N=0;//求取平均
	u32 temp=0;
	double fre=0;
	//不能小于64个采样点
	if(len<64)
	{
		return 0;
	}
	for(i=0;i<len-1;i++)
	{
		if(sample[i]>=offset)
		{
			if(sample[i+1]<offset)
			{
				start=end;
				end=i;
				if(start==0||(end-start)<64||(end-start)>300)
				{
					continue;
				}else
				{
					temp+=end-start;
					N++;
				}
			}
		}
	}
	fre=(sample_fre*1.0)/((temp*1.0)/N);
	return fre;
}

//将修正系数转变成3或4个字节保存
void Float2ByteArray(float x , u8* byte, u8 len)
{
	u16 temp=x;
	if(len==3)
	{
	  byte[0]=temp%0X100;
	  temp=(x-temp+0.000001)*1000;
	  byte[1]=temp/0X100;
	  byte[2]=temp%0X100;
	}
	else if(len==4)
	{
		byte[0]=temp/0X100;
	  byte[1]=temp%0X100;
	  temp=(x-temp+0.000001)*1000;
	  byte[2]=temp/0X100;
	  byte[3]=temp%0X100;
	}
}

//4字节数组转float
float ByteArray2Float(u8 byte[], u8 len)
{
	if(len==4)
	  return byte[0]*0X100+byte[1]+((byte[2]*0X100+byte[3])*10+1)/10000.0;
	if(len==3)
		return byte[0]+((byte[1]*0X100+byte[2])*10+1)/10000.0;
	return 1;
}

