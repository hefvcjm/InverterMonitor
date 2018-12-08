#include "user_interface.h"
#include "sys.h"
#include "delay.h"
#include "adc.h"
#include "led.h"
#include "lcd12864.h"
#include "calculate.h"
#include "fft.h"
#include "rtc.h"
#include "ds18b20.h"
#include "eeprom.h"
#include "pgcp.h"
#include "timer.h"
#include "math.h"
#include "dsp.h"

#define CAL_FFT_N  512  //计算用的FFT点数

u8 display_flag=0;//显示标志，在此标志为1时候，不允许改变相关状态
u8 first_time_show=0;//

//自定义符号，标记功率模块模式
u8 power_mode_flag[]={
  0X00,0X78,0X00,0X24,0X00,0X24,0X00,0X38,0X00,0X20,0X00,0X20,0X00,0X23,0X00,0X73,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00
};

u8 inverter_data_page=0;//保存逆变器模式下数据显示的页码（多页显示）
u8 inverter_setting_select=0;//保存逆变器模式下当前设置选项
u8 inverter_on_off_select=1;//保存选择启动或停止
u8 inverter_mode_select=0;//保存逆变器模式下模式选择选项
u8 inverter_deadzone_select=0;//选择死区时间
u8 inverter_invalid_pluse_select=0;//选择无效脉冲隔离
u8 inverter_state_select=ITEM_POWER_ON;//选择工作模式，不可操作，只能查看
u8 inverter_fault_state_select=0;//选择故障状态，不可操作，只能查看
u8 inverter_time_select=0;//选择设置时间位置
u8 inverter_ratio_page=0;//修正系数当前页数

u8 inverter_time_mode=INVERTER_TIME_SELECT_BIT;//当前设置时间为选位操作
u8 is_time=0;//是否设置时间标记
u8 is_time_finish=1;//是否完成时间设置,0,未完成;1,完成

u8 inverter_ratio_mode=INVERTER_RATIO_SELECT_BIT;//当前为位选
u8 ratio_which_bit=0;//当前为哪一位，共8位，前4位电压，后4位电流，分别1位整数，3位小数
u8 is_ratio_finish=1;//是否完成系数的设置操作

u8 led_state=LED_NORMAL;//用于标示led状态，以便控制其闪烁等状态

u8 power_data_page=0;//保存功率模块模式下数据显示的页码（多页显示）
u8 power_setting_select=0;//保存功率模块模式下当前设置选项
u8 power_mode_select=0;//保存功率模块模式下模式选择选项
u8 power_state_select=0;//选择工作模式，不可操作，只能查看
u8 power_time_select=0;//选择设置时间位置
u8 power_ratio_page=0;//修正系数当前页数

u8 power_time_mode=POWER_TIME_SELECT_BIT;//当前设置时间为选位操作
u8 power_ratio_mode=POWER_RATIO_SELECT_BIT;//当前为位选

u8 should_clr=1;//进入某个界面是否要清屏，也就是是不是第一次进入该显示界面，0，不用清屏；1，需要清屏
u8 refresh_inverse=0;//记录是否该刷新反白显示，反之闪烁
u16 refresh_prd=0;//计数周期值

u8 ok=OK_UP;//确认键未按下
u8 cancel=CANCEL_UP;//取消键未按下
u8 warn_select=0;//警告确认，0，否；1，是

u8 inverter_setting_page=0;//逆变器模式下设置页面，0或1,2
u8 inverter_state_select_page=0;//工作状态显示页面，0或1
u8 power_setting_page=0;//功率模块模式下设置页面，0或1

//缓存时间信息
u16 year=2017;
u8 mon=7;
u8 day=1;
u8 hour=14;
u8 min=30;
u8 sec=0;

float ratio_vol_temp=1;//缓存电压修正系数
float ratio_cur_temp=1;//缓存电流修正系数

u8 display=INVERTER_DATA_INTERFACE;//保存显示界面标识
u8 mode=INVERTER_MODE;//逆变器模式
u8 setting=INVERTER_SETTING_MODE;//逆变器模式下设置
//u8 inverter_on_off=ITEM_ON;//逆变器启动与停止

//各个通道修正系数
float vol_dc_ratio=1;
float cur_dc_ratio=1;
float vol_dc1_ratio=1;
float cur_dc1_ratio=1;
float vol_a_ratio=1;
float cur_a_ratio=1;
float vol_b_ratio=1;
float cur_b_ratio=1;
float vol_c_ratio=1;
float cur_c_ratio=1;

//定义变量求和，以便求取平均值
double vol_dc_sum=0;
double cur_dc_sum=0;
double vol_dc1_sum=0;
double cur_dc1_sum=0;
double vol_a_sum=0;
double cur_a_sum=0;
double act_pow_a_sum=0;
double vol_b_sum=0;
double cur_b_sum=0;
double act_pow_b_sum=0;
double vol_c_sum=0;
double cur_c_sum=0;
double act_pow_c_sum=0;

double fre_sum=0;

//定义各个变量
double vol_dc=0;
double cur_dc=0;
double pow_dc=0;
double vol_dc1=0;
double cur_dc1=0;
double pow_dc1=0;
double vol_a=0;
double cur_a=0;
double act_pow_a=0;
double app_pow_a=0;
double pow_factor_a=1;
double vol_b=0;
double cur_b=0;
double act_pow_b=0;
double app_pow_b=0;
double pow_factor_b=1;
double vol_c=0;
double cur_c=0;
double act_pow_c=0;
double app_pow_c=0;
double pow_factor_c=1;

float fre=50.00;//频率
u16 fs=10000;//采样频率
//fft方式求出的幅值和频率
double fft_amp=0;
double fft_harm=0;//谐波
double fft_fre=0;
double thd=0;//谐波失真

double fft_amp_sum=0;
double fft_harm_sum=0;
double fft_fre_sum=0;

double production=34230000.4334;//发电量,最大值为4294967296
double time=0;

u8 average=0;

//功率模块标识符，初始化，自定义图形
void InitPowerFlag(void)
{
//	u8 i=0;
//	Write_COM(0X40);
//	is_address_plus=0;
//	for(i=0;i<16;i++)
//	{
//		Write_DAT(power_mode_flag[i*2]);
//		Write_DAT(power_mode_flag[i*2+1]);
//	}
//	is_address_plus=1;
}

//上电回复存储的数据
void Recover_Ratio(void)
{
	u8 byte[3]={1,0,0};
	u8 i=0;
	float ratio[10]={1,1,1,1,1,1,1,1,1,1};
	//是否为首次初始化，是的话保存为默认值，否则加载已保存的值
	if(AT24CXX_ReadOneByte(0)!=EEPROM_INIT_FLAG)
	{
		AT24CXX_WriteOneByte(0,EEPROM_INIT_FLAG);
		for(i=0;i<20;i++)
		{
			 AT24CXX_Write(0+i*3+1,byte,3);
		}
		AT24CXX_WriteOneByte(100,inverter_deadzone_select);//死区时间
		AT24CXX_WriteOneByte(101,inverter_invalid_pluse_select);//脉冲隔离
	}else{
		if(mode==INVERTER_MODE)
		{
			for(i=0;i<10;i++)
		  {
			  AT24CXX_Read(0+i*3+1,byte,3);
		    ratio[i]=ByteArray2Float(byte,3);
		    if(ratio[i]>RATIO_UP_RANGE||ratio[i]<=RATIO_DOWN_RANGE)
	  	  {
			    Float2ByteArray(ratio[i],byte,3);
			    AT24CXX_Write(0+i*3+1,byte,3);
				  ratio[i]=1;
		    }
	    }
			if(AT24CXX_ReadOneByte(100)<INVERTER_DEADZONE_CHOICE)
		  {
		  	inverter_deadzone_select=AT24CXX_ReadOneByte(100);
		  }
		  if(AT24CXX_ReadOneByte(101)<INVERTER_INVALID_PLUSE_CHOICE)
		  {
			  inverter_invalid_pluse_select=AT24CXX_ReadOneByte(101);
		  }
		}else if(mode==POWER_MODE)
		{
			for(i=0;i<10;i++)
		  {
			  AT24CXX_Read(50+i*3+1,byte,3);
		    ratio[i]=ByteArray2Float(byte,3);
		    if(ratio[i]>RATIO_UP_RANGE||ratio[i]<=RATIO_DOWN_RANGE)
	  	  {
			    Float2ByteArray(ratio[i],byte,3);
			    AT24CXX_Write(50+i*3+1,byte,3);
				  ratio[i]=1;
		    }
	    }
		}
	}
	vol_dc_ratio=ratio[0];
	cur_dc_ratio=ratio[1];
	vol_dc1_ratio=ratio[2];
	cur_dc1_ratio=ratio[3];
	vol_a_ratio=ratio[4];
	cur_a_ratio=ratio[5];
	vol_b_ratio=ratio[6];
	cur_b_ratio=ratio[7];
	vol_c_ratio=ratio[8];
	cur_c_ratio=ratio[9];
	ratio_vol_temp=vol_dc_ratio;
	ratio_cur_temp=cur_dc_ratio;
}

void Init_Display(void)
{
	Recover_Ratio();
	InitPowerFlag();
}

void DispPowerFlag(void)
{
//	SetPosition(0,7);
//	Write_DAT(0X00);
//	Write_DAT(0X00);
	DispStr(0,7," P");
}

//故障或者警告标示
void DispFaultFlag(void)
{
	u8 temp=inverter_fault_state_select;
	if(inverter_state_select==ITEM_NORMAL_WORK)
	{
		POWER=1;
		TIM_Cmd(TIM2, DISABLE); 
	}else{
		TIM_Cmd(TIM2, ENABLE);  //使能TIMx	
	}
	if(inverter_state_select==ITEM_FAULT)
	{
		POWER=1;
		if(temp<ITEM_OVER_CUR)//0x10
		{
			//警告
			DispStr(0,7," w");
			led_state=LED_WARN;
		}else{
			//故障
			DispStr(0,7," e");
			led_state=LED_ERROR;
		}
	}else{			
		DispStr(0,7,"  ");
		led_state=LED_NORMAL;
		WARN=0;
		ERROR=0;
	}
}

void calculate(void)
{
	u16 n=0;
	u16 k=0;
	u16 m=0;
	double temp=0;
	if(completed)//各路ADC均完成转换，待读结果
	{
		//DC
		cur_dc_sum+=rms_current(sample_dc_cur,COUNT_SAMPLE,CUR_DC_OFFSET);
		vol_dc_sum+=rms_voltage(sample_dc_vol,COUNT_SAMPLE,VOL_DC_OFFSET);
		//DC1
		cur_dc1_sum+=rms_current(sample_dc1_cur,COUNT_SAMPLE,CUR_DC1_OFFSET);
		vol_dc1_sum+=rms_voltage(sample_dc1_vol,COUNT_SAMPLE,VOL_DC1_OFFSET);
		//A
		cur_a_sum+=rms_current(sample_a_cur,COUNT_SAMPLE,CUR_A_OFFSET);
		vol_a_sum+=rms_voltage(sample_a_vol,COUNT_SAMPLE,VOL_A_OFFSET);
		act_pow_a_sum+=active_power(sample_a_cur,CUR_A_OFFSET,sample_a_vol,VOL_A_OFFSET,COUNT_SAMPLE);
		//B
		cur_b_sum+=rms_current(sample_b_cur,COUNT_SAMPLE,CUR_B_OFFSET);
		vol_b_sum+=rms_voltage(sample_b_vol,COUNT_SAMPLE,VOL_B_OFFSET);
		act_pow_b_sum+=active_power(sample_b_cur,CUR_B_OFFSET,sample_b_vol,VOL_B_OFFSET,COUNT_SAMPLE);
		//C
		cur_c_sum+=rms_current(sample_c_cur,COUNT_SAMPLE,CUR_C_OFFSET);
		vol_c_sum+=rms_voltage(sample_c_vol,COUNT_SAMPLE,VOL_C_OFFSET);
		act_pow_c_sum+=active_power(sample_c_cur,CUR_C_OFFSET,sample_c_vol,VOL_C_OFFSET,COUNT_SAMPLE);
		//频率
		fre_sum+=frequency(sample_a_vol,COUNT_SAMPLE,VOL_A_OFFSET,fs);
		
		fft(CAL_FFT_N,fft_complex);
		//找出最大幅值点，以便求取幅值和频率
		for (n = 0; n<CAL_FFT_N/2; n++)
		{
			if (temp < sqrt(fft_complex[n].real*fft_complex[n].real + fft_complex[n].imag*fft_complex[n].imag))
			{
				temp = sqrt(fft_complex[n].real*fft_complex[n].real + fft_complex[n].imag*fft_complex[n].imag);
				k = n;
			}
		}
		fft_amp_sum+=temp*2/CAL_FFT_N;
		m=CAL_FFT_N/(2*k);
		for(n=2; n<m; n++)
		{
			fft_harm_sum+=(fft_complex[k*n].real*fft_complex[k*n].real + fft_complex[k*n].imag*fft_complex[k*n].imag)*4/(CAL_FFT_N*CAL_FFT_N);
		}
		//fft_harm_sum=fft_harm_sum*2/CAL_FFT_N;
		fft_fre_sum+=fs*k*1.0/(CAL_FFT_N*1.0);
		
		average++;
		if(average>=AVERAGE)//求取平均值
		{
			//计算发电量
			time=(TIM_GetCounter(TIM3)+(u32)(5000*timer_count))*1.0/1000000.0;
			production+=(act_pow_a+act_pow_b+act_pow_c)*time/3600.0;
			timer_count=0;
			//DC
		  cur_dc=K_DC_CUR*(cur_dc_sum/average);
		  vol_dc=K_DC_VOL*(vol_dc_sum/average);
		  pow_dc=(((u32)(cur_dc*10000))*1.0/10000.0)*(((u32)(vol_dc*10000))*1.0/10000.0);
			pow_dc=pow_dc/1000.0;
			//DC1
		  cur_dc1=K_DC1_CUR*(cur_dc1_sum/average);
		  vol_dc1=K_DC1_VOL*(vol_dc1_sum/average);
		  pow_dc1=(((u32)(cur_dc1*10000))*1.0/10000.0)*(((u32)(vol_dc1*10000))*1.0/10000.0);
			pow_dc1=pow_dc1/1000.0;
		  //A
		  cur_a=K_A_CUR*(cur_a_sum/average);
		  vol_a=K_A_VOL*(vol_a_sum/average);
		  act_pow_a=K_A_ACT_POW*(act_pow_a_sum/average);
		  app_pow_a=cur_a*vol_a;
		  pow_factor_a=power_factor(act_pow_a,app_pow_a);
			act_pow_a=act_pow_a/1000.0;
		  //B
		  cur_b=K_B_CUR*(cur_b_sum/average);
		  vol_b=K_B_VOL*(vol_b_sum/average);
		  act_pow_b=K_B_ACT_POW*(act_pow_b_sum/average);
		  app_pow_b=cur_b*vol_b;
		  pow_factor_b=power_factor(act_pow_b,app_pow_b);
			act_pow_b=act_pow_b/1000.0;
		  //C
		  cur_c=K_C_CUR*(cur_c_sum/average);
		  vol_c=K_C_VOL*(vol_c_sum/average);
		  act_pow_c=K_C_ACT_POW*(act_pow_c_sum/average);
		  app_pow_c=cur_c*vol_c;
		  pow_factor_c=power_factor(act_pow_c,app_pow_c);
			act_pow_c=act_pow_c/1000.0;
			//频率
			fre=fre_sum/average;
			//自同步采样率
			if(fre>40 && fre<60)
			{
				if(fs!=(u16)(fre*FFT_N))
				{
					fs=(u16)(fre*FFT_N);
					if(fs>=4000 && fs<=10000)//最大采样率10000，最小采样率4000
					{
						TIM_Configuration(fs);//重新设置采样率
					}
			  }
			}
			fft_amp=fft_amp_sum/average;
			fft_harm=fft_harm_sum/average;
			fft_fre=fft_fre_sum/average;
			thd=fft_harm/fft_amp*fft_amp;
//			thd=(vol_a*vol_a-fft_amp*fft_amp/2)/(fft_amp*fft_amp/2);
//			if(thd<0)
//			{
//				thd=0;
//			}
//			if(thd>1)
//			{
//				thd=1;
//			}
			//清零
			average=0;
			vol_dc_sum=0;
      cur_dc_sum=0;
			vol_dc1_sum=0;
      cur_dc1_sum=0;
      vol_a_sum=0;
      cur_a_sum=0;
      act_pow_a_sum=0;
      vol_b_sum=0;
      cur_b_sum=0;
      act_pow_b_sum=0;
      vol_c_sum=0;
      cur_c_sum=0;
      act_pow_c_sum=0;
			fre_sum=0;
			fft_amp_sum=0;
			fft_harm_sum=0;
			fft_fre_sum=0;
			//格式化数据，用于通信
			//A相电压
			info_data[0]=(u16)(vol_a*100)%0X100;
			info_data[1]=(u16)(vol_a*100)/0X100;
			//B相电压
			info_data[2]=(u16)(vol_b*100)%0X100;
			info_data[3]=(u16)(vol_b*100)/0X100;
			//C相电压
			info_data[4]=(u16)(vol_c*100)%0X100;
			info_data[5]=(u16)(vol_c*100)/0X100;
			//电流
			//A相电流
			info_data[6]=(u16)(cur_a*100)%0X100;
			info_data[7]=(u16)(cur_a*100)/0X100;
			//B相电流
			info_data[8]=(u16)(cur_b*100)%0X100;
			info_data[9]=(u16)(cur_b*100)/0X100;
			//C相电流
			info_data[10]=(u16)(cur_c*100)%0X100;
			info_data[11]=(u16)(cur_c*100)/0X100;
			//功率
			//A相有功功率
			info_data[12]=(u16)(act_pow_a*1000)%0X100;
			info_data[13]=(u16)(act_pow_a*1000)/0X100;
			//B相有功功率
			info_data[14]=(u16)(act_pow_b*1000)%0X100;
			info_data[15]=(u16)(act_pow_b*1000)/0X100;
			//C相有功功率
			info_data[16]=(u16)(act_pow_c*1000)%0X100;
			info_data[17]=(u16)(act_pow_c*1000)/0X100;
			//功率因数
			//A相功率因数
			info_data[18]=(u16)(pow_factor_a*1000)%0X100;
			info_data[19]=(u16)(pow_factor_a*1000)/0X100;
			//B相功率因数
			info_data[20]=(u16)(pow_factor_b*1000)%0X100;
			info_data[21]=(u16)(pow_factor_b*1000)/0X100;
			//C相功率因数
			info_data[22]=(u16)(pow_factor_c*1000)%0X100;
			info_data[23]=(u16)(pow_factor_c*1000)/0X100;
			//频率
			info_data[24]=(u16)(fre*100)%0X100;
			info_data[25]=(u16)(fre*100)/0X100;
			//累计发电量
			info_data[26]=(u16)((production-(u32)production)*1000)%0X100;
			info_data[27]=(u16)((production-(u32)production)*1000)/0X100;
			info_data[28]=(u32)(production)%0X100;
			info_data[29]=(u32)(production)%0X10000/0X100;
			info_data[30]=(u32)(production)%0X1000000/0X10000;
			info_data[31]=(u32)(production)/0X1000000;
			//光伏
			//光伏1电压
			info_data[32]=(u16)(vol_dc*100)%0X100;
			info_data[33]=(u16)(vol_dc*100)/0X100;
			//光伏2电压
			info_data[34]=(u16)(vol_dc1*100)%0X100;
			info_data[35]=(u16)(vol_dc1*100)/0X100;
			//电流
			//光伏1电流
			info_data[36]=(u16)(cur_dc*100)%0X100;
			info_data[37]=(u16)(cur_dc*100)/0X100;
			//光伏2电流
			info_data[38]=(u16)(cur_dc1*100)%0X100;
			info_data[39]=(u16)(cur_dc1*100)/0X100;
			//功率
			//光伏1功率
			info_data[40]=(u16)(pow_dc*1000)%0X100;
			info_data[41]=(u16)(pow_dc*1000)/0X100;
			//光伏2功率
			info_data[42]=(u16)(pow_dc1*100)%0X100;
			info_data[43]=(u16)(pow_dc1*100)/0X100;
		}
	}
}

void show(void)
{
	display_flag=1;//设置标志
	if(should_clr)
	{
		Lcd_Clr();//清屏
		delay_ms(10);
		should_clr=0;//清空标记
	}
	if(mode==INVERTER_MODE)
	{
		switch(display)
		{
			case INVERTER_DATA_INTERFACE:
				switch(inverter_data_page)
	      {
		      case DATA_DC_DISPLAY:
						if(should_clr)
	          {
		          Lcd_Clr();//清屏
		          delay_ms(10);
							Lcd_Clr();//清屏
		          delay_ms(10);
		          should_clr=0;//清空标记
	          }
						DispFaultFlag();
				    DispStr(0,2,"光伏通道1");
			      DispStr(1,0,"U:");
		        DispDecimalNext(vol_dc,2);
		        DispCharNext('V');
		        DispStrSingleLineNext("        ");
		        DispStr(2,0,"I:");
            DispDecimalNext(cur_dc,3);
		        DispCharNext('A');
		        DispStrSingleLineNext("        ");
				    DispStr(3,0,"P:");
            DispDecimalNext(pow_dc,3);
				    DispStrNext("kW");
				    DispStrSingleLineNext("        ");
			      break;
					case DATA_DC1_DISPLAY:
						if(should_clr)
	          {
		          Lcd_Clr();//清屏
		          delay_ms(10);
							Lcd_Clr();//清屏
		          delay_ms(10);
		          should_clr=0;//清空标记
	          }
						DispFaultFlag();
				    DispStr(0,2,"光伏通道2");
			      DispStr(1,0,"U:");
		        DispDecimalNext(vol_dc1,2);
		        DispCharNext('V');
		        DispStrSingleLineNext("        ");
		        DispStr(2,0,"I:");
            DispDecimalNext(cur_dc1,3);
		        DispCharNext('A');
		        DispStrSingleLineNext("        ");
				    DispStr(3,0,"P:");
            DispDecimalNext(pow_dc1,3);
				    DispStrNext("kW");
				    DispStrSingleLineNext("        ");
						break;
		      case DATA_A_DISPLAY:
						if(should_clr)
	          {
		          Lcd_Clr();//清屏
		          delay_ms(10);
							Lcd_Clr();//清屏
		          delay_ms(10);
		          should_clr=0;//清空标记
	          }
						DispFaultFlag();
			      DispStr(0,0,"U(A):");
		        DispDecimalNext(vol_a,2);
		        DispCharNext('V');
		        DispBlankRemainLast();
		        DispStr(1,0,"I(A):");
            DispDecimalNext(cur_a,3);
		        DispCharNext('A');
		        DispStrSingleLineNext("        ");
				    DispStr(2,0,"P(A):");
            DispDecimalNext(act_pow_a,3);
				    DispStrNext("kW");
				    DispStrSingleLineNext("        ");
						SetPosition(3,0);
						Write_DAT(0XA6);
						Write_DAT(0XD7);
						DispStrNext("(A):");
            DispDecimalNext(pow_factor_a,4);
				    DispStrSingleLineNext("        ");
			      break;
		      case DATA_B_DISPLAY:
						if(should_clr)
	          {
		          Lcd_Clr();//清屏
		          delay_ms(10);
							Lcd_Clr();//清屏
		          delay_ms(10);
		          should_clr=0;//清空标记
	          }
						DispFaultFlag();
			      DispStr(0,0,"U(B):");
		        DispDecimalNext(vol_b,2);
		        DispCharNext('V');
		        DispBlankRemainLast();
		        DispStr(1,0,"I(B):");
            DispDecimalNext(cur_b,3);
		        DispCharNext('A');
		        DispStrSingleLineNext("        ");
				    DispStr(2,0,"P(B):");
            DispDecimalNext(act_pow_b,3);
				    DispStrNext("kW");
				    DispStrSingleLineNext("        ");
						SetPosition(3,0);
						Write_DAT(0XA6);
						Write_DAT(0XD7);
						DispStrNext("(B):");
            DispDecimalNext(pow_factor_b,4);
				    DispStrSingleLineNext("        ");
		      	break;
		      case DATA_C_DISPLAY:
						if(should_clr)
	          {
		          Lcd_Clr();//清屏
		          delay_ms(10);
							Lcd_Clr();//清屏
		          delay_ms(10);
		          should_clr=0;//清空标记
	          }
						DispFaultFlag();
		     	  DispStr(0,0,"U(C):");
		        DispDecimalNext(vol_c,2);
		        DispCharNext('V');
		        DispBlankRemainLast();
		        DispStr(1,0,"I(C):");
            DispDecimalNext(cur_c,3);
		        DispCharNext('A');
		        DispStrSingleLineNext("        ");
				    DispStr(2,0,"P(C):");
            DispDecimalNext(act_pow_c,3);
				    DispStrNext("kW");
				    DispStrSingleLineNext("        ");
						SetPosition(3,0);
						Write_DAT(0XA6);
						Write_DAT(0XD7);
						DispStrNext("(C):");
            DispDecimalNext(pow_factor_c,4);
				    DispStrSingleLineNext("        ");
		      	break;
					case DATA_FRE_DISPLAY:
						if(should_clr)
	          {
		          Lcd_Clr();//清屏
		          delay_ms(10);
							Lcd_Clr();//清屏
		          delay_ms(10);
		          should_clr=0;//清空标记
	          }
						DispFaultFlag();
						DispStr(0,0,"电网频率:");
		        DispBlankRemainLast();
						DispDecimal(1,0,fre,2);
						DispStrNext("Hz");
						DispStrSingleLineNext("        ");
						DispStr(2,0,"总谐波畸变THD:");
//						DispDecimal(2,0,vol_a,2);
//						DispStrNext("  ");
//						DispDecimalNext(fft_amp/sqrt(2.0),2);
//		        DispStrSingleLineNext("        ");
						DispDecimal(3,0,thd*100,2);
						DispStrNext("%");
						DispStrSingleLineNext("        ");
						break;
	      	default:
		      	break;
	      }
				break;
			case INVERTER_SETTING_INTERFACE:
				if(should_clr)
	      {
		      Lcd_Clr();//清屏
		      delay_ms(10);
					Lcd_Clr();//清屏
		      delay_ms(10);
		      should_clr=0;//清空标记
					refresh_inverse=1;
	      }
				DispFaultFlag();
				if(inverter_setting_select==ITEM_ON_OFF)
				{
					inverter_setting_page=0;
				}else if(inverter_setting_select==ITEM_RATIO)
				{
					inverter_setting_page=3;
				}else if(inverter_setting_select<inverter_setting_page)
				{
					inverter_setting_page--;
				}else if(inverter_setting_select>inverter_setting_page+3){
					inverter_setting_page++;
				}
//				if(inverter_setting_select==ITEM_ON_OFF)
//				{
//					inverter_setting_page=0;
//				}else if(inverter_setting_select==ITEM_RATIO)
//				{
//					inverter_setting_page=3;
//				}else if(inverter_setting_select==ITEM_DEADZONE){
//					if(inverter_setting_page==2)
//					{
//						inverter_setting_page=1;
//					}
//				}else if(inverter_setting_select==ITEM_TIME)
//				{
//					if(inverter_setting_page==0)
//					{
//						inverter_setting_page=1;
//					}
//				}
				switch(inverter_setting_page)
				{
					case 0:
					  DispStr(0,0,"启动与停止");
						DispBlankRemainLast();
						DispStr(1,0,"工作模式");
						DispStrSingleLineNext("          ");
						DispStr(2,0,"脉冲死区时间");
						DispStrSingleLineNext("          ");
						DispStr(3,0,"无效脉冲隔离");
						DispStrSingleLineNext("          ");
						break;
					case 1:
						DispStr(0,0,"工作模式");
						DispBlankRemainLast();
						DispStr(1,0,"脉冲死区时间");
						DispStrSingleLineNext("          ");
						DispStr(2,0,"无效脉冲隔离");
						DispStrSingleLineNext("          ");
						DispStr(3,0,"工作状态");
						DispStrSingleLineNext("          ");
						break;
					case 2:
						DispStr(0,0,"脉冲死区时间");
						DispBlankRemainLast();
						DispStr(1,0,"无效脉冲隔离");
						DispStrSingleLineNext("          ");
						DispStr(2,0,"工作状态");
						DispStrSingleLineNext("          ");
						DispStr(3,0,"系统时钟");
						DispStrSingleLineNext("          ");
						break;
					case 3:
						DispStr(0,0,"无效脉冲隔离");
						DispBlankRemainLast();
						DispStr(1,0,"工作状态");
						DispStrSingleLineNext("          ");
						DispStr(2,0,"系统时钟");
						DispStrSingleLineNext("          ");
						DispStr(3,0,"修正系数");
						DispStrSingleLineNext("          ");
						break;
					default:
						break;
				}
			  if(refresh_inverse)
				{
					refresh_inverse=0;
					switch(inverter_setting_page)
					{
						case 0:
							switch(inverter_setting_select)
							{
								case ITEM_ON_OFF:
									Inverse(0,0,10);
									break;
								case ITEM_MODE:
									Inverse(1,0,8);
									break;
								case ITEM_DEADZONE:
									Inverse(2,0,12);
									break;
								case ITEN_INVALID_PLUSE:
									Inverse(3,0,12);
									break;
								case ITEM_STATE:
									//Inverse(3,0,12);
									break;
								case ITEM_TIME:
									//Inverse(3,0,12);
									break;
								case ITEM_RATIO:
									//Inverse(3,0,12);
									break;
								default:
									break;
							}
							break;
						case 1:
							switch(inverter_setting_select)
							{
								case ITEM_ON_OFF:
									//Inverse(0,0,10);
									break;
								case ITEM_MODE:
									Inverse(0,0,8);
									break;
								case ITEM_DEADZONE:
									Inverse(1,0,12);
									break;
								case ITEN_INVALID_PLUSE:
									Inverse(2,0,12);
									break;
								case ITEM_STATE:
									Inverse(3,0,8);
									break;
								case ITEM_TIME:
									//Inverse(3,0,12);
									break;
								case ITEM_RATIO:
									//Inverse(3,0,12);
									break;
								default:
									break;
							}
							break;
						case 2:
							switch(inverter_setting_select)
							{
								case ITEM_ON_OFF:
									//Inverse(0,0,10);
									break;
								case ITEM_MODE:
									//Inverse(0,0,8);
									break;
								case ITEM_DEADZONE:
									Inverse(0,0,12);
									break;
								case ITEN_INVALID_PLUSE:
									Inverse(1,0,12);
									break;
								case ITEM_STATE:
									Inverse(2,0,8);
									break;
								case ITEM_TIME:
									Inverse(3,0,8);
									break;
								case ITEM_RATIO:
									//Inverse(3,0,12);
									break;
								default:
									break;
							}
							break;
						case 3:
							switch(inverter_setting_select)
							{
								case ITEM_ON_OFF:
									//Inverse(0,0,10);
									break;
								case ITEM_MODE:
									//Inverse(0,0,8);
									break;
								case ITEM_DEADZONE:
									//Inverse(1,0,12);
									break;
								case ITEN_INVALID_PLUSE:
									Inverse(0,0,12);
									break;
								case ITEM_STATE:
									Inverse(1,0,8);
									break;
								case ITEM_TIME:
									Inverse(2,0,8);
									break;
								case ITEM_RATIO:
									Inverse(3,0,8);
									break;
								default:
									break;
							}
							break;
						default:
							break;
					}
				}
				break;
			case INVERTER_SETTING_ON_OFF:
				if(should_clr)
	      {
		      Lcd_Clr();//清屏
		      delay_ms(10);
					Lcd_Clr();//清屏
		      delay_ms(10);
		      should_clr=0;//清空标记
					refresh_inverse=1;
	      }
				DispFaultFlag();
				DispStr(0,0,"停止");
			  DispStr(1,0,"启动");
			  if(refresh_inverse)
				{
					refresh_inverse=0;
					switch(inverter_on_off_select)
				  {
					  case ITEM_INVERTER_ON:
						  Inverse(1,0,4);
						  break;
					  case ITEM_INVERTER_OFF:
						  Inverse(0,0,4);
					  	break;
				  	default:
					  	break;
				  }
				}
				break;
			case INVERTER_SETTING_MODE:
				if(should_clr)
	      {
		      Lcd_Clr();//清屏
		      delay_ms(10);
					Lcd_Clr();//清屏
		      delay_ms(10);
		      should_clr=0;//清空标记
					refresh_inverse=1;
	      }
				DispFaultFlag();
				DispStr(0,0,"逆变器模式");
			  DispStr(1,0,"功率模块模式");
			  if(refresh_inverse)
				{
					refresh_inverse=0;
					switch(inverter_mode_select)
				  {
					  case ITEM_INVERTER:
						  Inverse(0,0,10);
						  break;
					  case ITEM_POWER:
						  Inverse(1,0,12);
					  	break;
				  	default:
					  	break;
				  }
				}
				break;
			case INVERTER_SETTING_DEADZONE:
				if(should_clr)
	      {
		      Lcd_Clr();//清屏
		      delay_ms(10);
					Lcd_Clr();//清屏
		      delay_ms(10);
		      should_clr=0;//清空标记
					refresh_inverse=1;
	      }
				DispFaultFlag();
				DispStr(0,0,"1us");
			  DispStr(1,0,"1.5us");
			  DispStr(2,0,"2us");
			  DispStr(3,0,"2.5us");
			  if(refresh_inverse)
				{
					refresh_inverse=0;
					switch(inverter_deadzone_select)
				  {
					  case ITEM_0:
						  Inverse(0,0,3);
					  	break;
					  case ITEM_1:
					  	Inverse(1,0,5);
					  	break;
					  case ITEM_2:
					  	Inverse(2,0,3);
					  	break;
					  case ITEM_3:
					  	Inverse(3,0,5);
					  	break;
					  default:
					  	break;
			  	}
				}
				break;
			case INVERTER_SETTING_INVALID_PLUSE:
				if(should_clr)
	      {
		      Lcd_Clr();//清屏
		      delay_ms(10);
					Lcd_Clr();//清屏
		      delay_ms(10);
		      should_clr=0;//清空标记
					refresh_inverse=1;
	      }
				DispFaultFlag();
				DispStr(0,0,"隔离");
			  DispStr(1,0,"不隔离");
			  if(refresh_inverse)
				{
					refresh_inverse=0;
					switch(inverter_invalid_pluse_select)
				  {
					  case ITEM_ON:
					 	  Inverse(0,0,4);
						  break;
					  case ITEM_OFF:
					  	Inverse(1,0,6);
					  	break;
					  default:
					  	break;
		  		}
				}
				break;
			case INVERTER_SETTING_STATE:
				if(should_clr)
	      {
		      Lcd_Clr();//清屏
		      delay_ms(10);
					Lcd_Clr();//清屏
		      delay_ms(10);
		      should_clr=0;//清空标记
					refresh_inverse=1;
	      }
				DispFaultFlag();
				if(inverter_state_select==ITEM_POWER_ON)
				{
					inverter_state_select_page=0;
				}else if(inverter_state_select==ITEM_FAULT)
				{
					inverter_state_select_page=1;
				}
				switch(inverter_state_select_page)
				{
					case 0:
						DispStr(0,0,"上电模式");
					  DispBlankRemainLast();
			      DispStr(1,0,"待机模式");
					  DispStrSingleLineNext("    ");
			      DispStr(2,0,"软启动模式");
					  DispStrSingleLineNext("    ");
				    DispStr(3,0,"正常工作模式");
					  DispStrSingleLineNext("    ");
						break;
					case 1:
			      DispStr(0,0,"待机模式");
					  DispBlankRemainLast();
			      DispStr(1,0,"软启动模式");
					  DispStrSingleLineNext("    ");
				    DispStr(2,0,"正常工作模式");
					  DispStrSingleLineNext("    ");
					  DispStr(3,0,"故障");
					  DispStrSingleLineNext("    ");
						break;
					default:
						break;
				}
			  if(refresh_inverse)
				{
					refresh_inverse=0;
					if(inverter_state_select_page==0)
					{
						switch(inverter_state_select)
				    {
					    case ITEM_POWER_ON:
						    Inverse(0,0,8);
						    break;
					    case ITEM_STAND_BY:
						    Inverse(1,0,8);
						    break;
					    case ITEN_SOFT_START:
						    Inverse(2,0,10);
						    break;
					  	case ITEM_NORMAL_WORK:
							  Inverse(3,0,12);
						    break;
							case ITEM_FAULT:
								break;
					    default:
						    break;
				    }
					}else if(inverter_state_select_page==1)
					{
						switch(inverter_state_select)
				    {
					    case ITEM_POWER_ON:
						    //Inverse(0,0,8);
						    break;
					    case ITEM_STAND_BY:
						    Inverse(0,0,8);
						    break;
					    case ITEN_SOFT_START:
						    Inverse(1,0,10);
						    break;
					  	case ITEM_NORMAL_WORK:
							  Inverse(2,0,12);
						    break;
							case ITEM_FAULT:
								Inverse(3,0,4);
								break;
					    default:
						    break;
				    }
					}
				}
				break;
			case INVERTER_FAULT_STATE:
				if(should_clr)
	      {
		      Lcd_Clr();//清屏
		      delay_ms(10);
					Lcd_Clr();//清屏
		      delay_ms(10);
		      should_clr=0;//清空标记
					refresh_inverse=1;
	      }
				DispFaultFlag();
				DispStr(0,0,"过压");
				DispStr(0,4,"欠压");
				DispStr(1,0,"过频");
				DispStr(1,4,"欠频");
				DispStr(2,0,"过流");
				DispStr(2,4,"过载");
				DispStr(3,0,"短路");
				DispStr(3,4,"其他");
				if(refresh_inverse)
				{
					refresh_inverse=0;
					Init_Inverse();
					if((inverter_fault_state_select&ITEM_OVER_VOL)==ITEM_OVER_VOL)
					{
						InverseContinue(0,0,4);
					}else if((inverter_fault_state_select&ITEN_UNDER_VOL)==ITEN_UNDER_VOL)
					{
						InverseContinue(0,8,4);
					}
					if((inverter_fault_state_select&ITEM_OVER_FRE)==ITEM_OVER_FRE)
					{
						InverseContinue(1,0,4);
					}else if((inverter_fault_state_select&ITEN_UNDER_FRE)==ITEN_UNDER_FRE)
					{
						InverseContinue(1,8,4);
					}
					if((inverter_fault_state_select&ITEM_OVER_CUR)==ITEM_OVER_CUR)
					{
						InverseContinue(2,0,4);
					}else if((inverter_fault_state_select&ITEM_OVER_LOAD)==ITEM_OVER_LOAD)
					{
						InverseContinue(2,8,4);
					}
					if((inverter_fault_state_select&ITEM_SHORT_CIRCUIT)==ITEM_SHORT_CIRCUIT)
					{
						InverseContinue(3,0,4);
					}
					if((inverter_fault_state_select%0x80)==0)
					{
						InverseContinue(3,8,4);
					}
					FinishInverseContinue();
				}
				break;
			case INVERTER_SETTING_TIME:
				if(should_clr)
	      {
		      Lcd_Clr();//清屏
		      delay_ms(10);
					Lcd_Clr();//清屏
		      delay_ms(10);
		      should_clr=0;//清空标记
					refresh_inverse=1;
	      }
				DispFaultFlag();
				if(!is_time_finish)
				{
					DispFigure(0,0,year);
		      DispCharNext('/');
		      if(mon<10)
		      {
			      DispFigureNext(0);
		      }
		      DispFigureNext(mon);
		      DispCharNext('/');
		      if(day<10)
		      {
			      DispFigureNext(0);
		      }
		      DispFigureNext(day);
		      DispBlankRemainLast();
					SetPosition(1,0);
		      if(hour<10)
		      {
			      DispFigureNext(0);
		      }
		      DispFigureNext(hour);
		      DispCharNext(':');
		      if(min<10)
		      {
			      DispFigureNext(0);
		      }
		      DispFigureNext(min);
		      DispCharNext(':');
		      if(sec<10)
		      {
			      DispFigureNext(0);
		      }
		      DispFigureNext(sec);
		      DispStrSingleLineNext("         ");
		      switch(RTC_Get_Week(year,mon,day))
		      {
			      case 0:
				      DispStr(2,0,"星期日");
				      break;
			      case 1:
				      DispStr(2,0,"星期一");
				      break;
			      case 2:
				      DispStr(2,0,"星期二");
				      break;
			      case 3:
				      DispStr(2,0,"星期三");
				      break;
			      case 4:
				      DispStr(2,0,"星期四");
			       	break;
			      case 5:
				      DispStr(2,0,"星期五");
			      	break;
			      case 6:
			      	DispStr(2,0,"星期六");
				      break;
			      default:
				      break;
		      }
					DispStr(3,6,"保存");
					if(refresh_inverse)
					{
						if(inverter_time_mode==INVERTER_TIME_SELECT_BIT)
						{
							refresh_inverse=0;
						}
						switch(inverter_time_select)
						{
							case 0:
								Inverse(0,0,4);
								break;
							case 1:
								Inverse(0,5,2);
								break;
							case 2:
								Inverse(0,8,2);
								break;
							case 3:
								Inverse(1,0,2);
								break;
							case 4:
								Inverse(1,3,2);
								break;
							case 5:
								Inverse(1,6,2);
								break;
							case 6:
								Inverse(3,12,4);
							  refresh_inverse=0;
								break;
							default:
								break;
						}
					}
				}else if(first_time_show){
					Time_Show();
					first_time_show=0;
				}
				break;
			case INVERTER_SETTING_RATIO:
				if(should_clr)
	      {
		      Lcd_Clr();//清屏
		      delay_ms(10);
					Lcd_Clr();//清屏
		      delay_ms(10);
		      should_clr=0;//清空标记
					refresh_inverse=1;
	      }
				DispFaultFlag();
				switch(inverter_ratio_page)
				{
					case RATIO_DC:
						DispStr(0,0,"光伏修正系数1:");
					  //DispBlankRemainLast();
					  DispStr(1,0,"RU:");
					  DispDecimalNext(ratio_vol_temp,3);
					  DispStrSingleLineNext("            ");
					  DispStr(2,0,"RI:");
					  DispDecimalNext(ratio_cur_temp,3);
					  DispStrSingleLineNext("            ");
						break;
					case RATIO_DC1:
						DispStr(0,0,"光伏修正系数2:");
					  //DispBlankRemainLast();
					  DispStr(1,0,"RU:");
					  DispDecimalNext(ratio_vol_temp,3);
					  DispStrSingleLineNext("            ");
					  DispStr(2,0,"RI:");
					  DispDecimalNext(ratio_cur_temp,3);
					  DispStrSingleLineNext("            ");
						break;
					case RATIO_A:
						DispStr(0,0,"逆变输出(A):");
					  //DispBlankRemainLast();
					  DispStr(1,0,"RU:");
					  DispDecimalNext(ratio_vol_temp,3);
					  DispStrSingleLineNext("            ");
					  DispStr(2,0,"RI:");
					  DispDecimalNext(ratio_cur_temp,3);
					  DispStrSingleLineNext("            ");
						break;
					case RATIO_B:
						DispStr(0,0,"逆变输出(B):");
					  //DispBlankRemainLast();
					  DispStr(1,0,"RU:");
					  DispDecimalNext(ratio_vol_temp,3);
					  DispStrSingleLineNext("            ");
					  DispStr(2,0,"RI:");
					  DispDecimalNext(ratio_cur_temp,3);
					  DispStrSingleLineNext("            ");
						break;
					case RATIO_C:
						DispStr(0,0,"逆变输出(C):");
					  //DispBlankRemainLast();
					  DispStr(1,0,"RU:");
					  DispDecimalNext(ratio_vol_temp,3);
					  DispStrSingleLineNext("            ");
					  DispStr(2,0,"RI:");
					  DispDecimalNext(ratio_cur_temp,3);
					  DispStrSingleLineNext("            ");
						break;
					default:
						break;
				}
				if(inverter_ratio_mode!=INVERTER_RATIO_VIEW)
				{
					DispStr(3,6,"保存");
					if(refresh_inverse)
					{
						if(inverter_ratio_mode==INVERTER_RATIO_SELECT_BIT)
					  {
						  refresh_inverse=0;
					  }
						switch(ratio_which_bit)
					  {
						  case 0:
							  Inverse(1,3,1);
							  break;
						  case 1:
						  	Inverse(1,5,1);
							  break;
						  case 2:
							  Inverse(1,6,1);
							  break;
						  case 3:
							  Inverse(1,7,1);
						  	break;
					  	case 4:
						    Inverse(2,3,1);
						  	break;
					  	case 5:
						  	Inverse(2,5,1);
						  	break;
						  case 6:
						  	Inverse(2,6,1);
							  break;
						  case 7:
							  Inverse(2,7,1);
						  	break;
						  case 8:
							  Inverse(3,12,4);
						    refresh_inverse=0;
						  	break;
					  	default:
						  	break;
				  	}
					}
				}
				break;
			case INVERTER_WARN:
				if(should_clr)
	      {
		      Lcd_Clr();//清屏
		      delay_ms(10);
					Lcd_Clr();//清屏
		      delay_ms(10);
		      should_clr=0;//清空标记
					refresh_inverse=1;
	      }
				DispFaultFlag();
				if(inverter_setting_select==ITEM_ON_OFF)
				{
					if(inverter_on_off_select==ITEM_INVERTER_ON)
					{
						DispStr(0,3,"警告");
						DispStr(1,0,"是否启动逆变器？");
						DispStr(2,2,"是");
						DispStr(2,5,"否");
					}else{
						DispStr(0,3,"警告");
						DispStr(1,0,"是否停止逆变器？");
						DispStr(2,2,"是");
						DispStr(2,5,"否");
					}
				}
				if(inverter_setting_select==ITEM_MODE)
				{
					DispStr(0,3,"警告");
					DispStr(1,0,"是否要切换模式？");
					DispStr(2,2,"是");
					DispStr(2,5,"否");
				}
			  if(refresh_inverse)
				{
					refresh_inverse=0;
					switch(warn_select)
					{
						case WARN_YES:
							Inverse(2,3,4);
							break;
						case WARN_NO:
							Inverse(2,9,4);
							break;
						default:
							break;
					}
				}
				break;
			default:
				break;
		}
	}else if(mode==POWER_MODE)
	{
		DispPowerFlag();
		switch(display)
		{
			case POWER_DATA_INTERFACE:
				switch(power_data_page)
	      {
		      case DATA_DC_DISPLAY:
						if(should_clr)
	          {
		          Lcd_Clr();//清屏
		          delay_ms(10);
							Lcd_Clr();//清屏
		          delay_ms(10);
		          should_clr=0;//清空标记
	          }
						DispPowerFlag();
				    DispStr(0,2,"光伏通道1");
			      DispStr(1,0,"U:");
		        DispDecimalNext(vol_dc,2);
		        DispCharNext('V');
		        DispStrSingleLineNext("        ");
		        DispStr(2,0,"I:");
            DispDecimalNext(cur_dc,3);
		        DispCharNext('A');
		        DispStrSingleLineNext("        ");
				    DispStr(3,0,"P:");
            DispDecimalNext(pow_dc,3);
				    DispStrNext("kW");
				    DispStrSingleLineNext("        ");
			      break;
					case DATA_DC1_DISPLAY:
						if(should_clr)
	          {
		          Lcd_Clr();//清屏
		          delay_ms(10);
							Lcd_Clr();//清屏
		          delay_ms(10);
		          should_clr=0;//清空标记
	          }
						DispPowerFlag();
				    DispStr(0,2,"光伏通道2");
			      DispStr(1,0,"U:");
		        DispDecimalNext(vol_dc1,2);
		        DispCharNext('V');
		        DispStrSingleLineNext("        ");
		        DispStr(2,0,"I:");
            DispDecimalNext(cur_dc1,3);
		        DispCharNext('A');
		        DispStrSingleLineNext("        ");
				    DispStr(3,0,"P:");
            DispDecimalNext(pow_dc1,3);
				    DispStrNext("kW");
				    DispStrSingleLineNext("        ");
			      break;
		      case DATA_A_DISPLAY:
						if(should_clr)
	          {
		          Lcd_Clr();//清屏
		          delay_ms(10);
							Lcd_Clr();//清屏
		          delay_ms(10);
		          should_clr=0;//清空标记
	          }
			      DispStr(0,0,"U(A):");
		        DispDecimalNext(vol_a,2);
		        DispCharNext('V');
		        DispBlankRemainLast();
		        DispStr(1,0,"I(A):");
            DispDecimalNext(cur_a,3);
		        DispCharNext('A');
		        DispStrSingleLineNext("        ");
				    DispStr(2,0,"P(A):");
            DispDecimalNext(act_pow_a,3);
				    DispStrNext("kW");
				    DispStrSingleLineNext("        ");
						SetPosition(3,0);
						Write_DAT(0XA6);
						Write_DAT(0XD7);
						DispStrNext("(A):");
            DispDecimalNext(pow_factor_a,4);
				    DispStrSingleLineNext("        ");
						DispPowerFlag();
			      break;
		      case DATA_B_DISPLAY:
						if(should_clr)
	          {
		          Lcd_Clr();//清屏
		          delay_ms(10);
							Lcd_Clr();//清屏
		          delay_ms(10);
		          should_clr=0;//清空标记
	          }
						DispStr(0,0,"U(B):");
		        DispDecimalNext(vol_b,2);
		        DispCharNext('V');
		        DispBlankRemainLast();
		        DispStr(1,0,"I(B):");
            DispDecimalNext(cur_b,3);
		        DispCharNext('A');
		        DispStrSingleLineNext("        ");
				    DispStr(2,0,"P(B):");
            DispDecimalNext(act_pow_b,3);
				    DispStrNext("kW");
				    DispStrSingleLineNext("        ");
						SetPosition(3,0);
						Write_DAT(0XA6);
						Write_DAT(0XD7);
						DispStrNext("(B):");
            DispDecimalNext(pow_factor_b,4);
				    DispStrSingleLineNext("        ");
						DispPowerFlag();
		      	break;
		      case DATA_C_DISPLAY:
						if(should_clr)
	          {
		          Lcd_Clr();//清屏
		          delay_ms(10);
							Lcd_Clr();//清屏
		          delay_ms(10);
		          should_clr=0;//清空标记
	          }
		     	  DispStr(0,0,"U(C):");
		        DispDecimalNext(vol_c,2);
		        DispCharNext('V');
		        DispBlankRemainLast();
		        DispStr(1,0,"I(C):");
            DispDecimalNext(cur_c,3);
		        DispCharNext('A');
		        DispStrSingleLineNext("        ");
				    DispStr(2,0,"P(C):");
            DispDecimalNext(act_pow_c,3);
				    DispStrNext("kW");
				    DispStrSingleLineNext("        ");
						SetPosition(3,0);
						Write_DAT(0XA6);
						Write_DAT(0XD7);
						DispStrNext("(C):");
            DispDecimalNext(pow_factor_c,4);
				    DispStrSingleLineNext("        ");
						DispPowerFlag();
		      	break;
					case DATA_FRE_DISPLAY:
						if(should_clr)
	          {
		          Lcd_Clr();//清屏
		          delay_ms(10);
							Lcd_Clr();//清屏
		          delay_ms(10);
		          should_clr=0;//清空标记
	          }
						DispStr(0,0,"电网频率:");
		        DispBlankRemainLast();
						DispDecimal(1,0,fre,2);
						DispStrNext("Hz");
						DispStrSingleLineNext("        ");
						DispStr(2,0,"总谐波畸变THD:");
						DispDecimal(3,0,thd*100,2);
						DispStrNext("%");
						DispStrSingleLineNext("        ");
						DispPowerFlag();
						break;
	      	default:
		      	break;
	      }
				break;
			case POWER_SETTING_INTERFACE:
				if(should_clr)
	      {
		      Lcd_Clr();//清屏
		      delay_ms(10);
					Lcd_Clr();//清屏
		      delay_ms(10);
		      should_clr=0;//清空标记
					refresh_inverse=1;
	      }
				DispPowerFlag();
				if(power_setting_select==OPTION_MODE)
				{
					power_setting_page=0;
				}else if(power_setting_select==OPTION_RATIO)
				{
					power_setting_page=1;
				}
				switch(power_setting_page)
				{
					case 0:
						DispStr(0,0,"工作模式");
					  DispBlankRemainLast();
			      DispStr(1,0,"发电量");
					  DispStrSingleLineNext("    ");
			      DispStr(2,0,"工作状态");
					  DispStrSingleLineNext("    ");
				    DispStr(3,0,"系统时钟");
					  DispStrSingleLineNext("    ");
						break;
					case 1:
			      DispStr(0,0,"发电量");
					  DispBlankRemainLast();
			      DispStr(1,0,"工作状态");
					  DispStrSingleLineNext("    ");
				    DispStr(2,0,"系统时钟");
					  DispStrSingleLineNext("    ");
					  DispStr(3,0,"修正系数");
					  DispStrSingleLineNext("    ");
						break;
					default:
						break;
				}
			  if(refresh_inverse)
				{
					refresh_inverse=0;
					if(power_setting_page==0)
					{
						switch(power_setting_select)
				    {
					    case OPTION_MODE:
						    Inverse(0,0,8);
						    break;
					    case OPTION_PRODUCTION:
						    Inverse(1,0,6);
						    break;
					    case OPTION_STATE:
						    Inverse(2,0,8);
						    break;
					  	case OPTION_TIME:
							  Inverse(3,0,8);
						    break;
							case OPTION_RATIO:
								break;
					    default:
						    break;
				    }
					}else if(power_setting_page==1)
					{
						switch(power_setting_select)
				    {
					    case OPTION_MODE:
						    //Inverse(0,0,8);
						    break;
					    case OPTION_PRODUCTION:
						    Inverse(0,0,6);
						    break;
					    case OPTION_STATE:
						    Inverse(1,0,8);
						    break;
					  	case OPTION_TIME:
							  Inverse(2,0,8);
						    break;
							case OPTION_RATIO:
								Inverse(3,0,8);
								break;
					    default:
						    break;
				    }
					}
				}
				break;
			case POWER_SETTING_MODE:
				if(should_clr)
	      {
		      Lcd_Clr();//清屏
		      delay_ms(10);
					Lcd_Clr();//清屏
		      delay_ms(10);
		      should_clr=0;//清空标记
					refresh_inverse=1;
	      }
				DispPowerFlag();
				DispStr(0,0,"逆变器模式");
			  DispStr(1,0,"功率模块模式");
			  if(refresh_inverse)
				{
					refresh_inverse=0;
					switch(power_mode_select)
				  {
					  case OPTION_INVERTER:
						  Inverse(0,0,10);
						  break;
					  case OPTION_POWER:
						  Inverse(1,0,12);
					  	break;
				  	default:
					  	break;
				  }
				}
				break;
			case POWER_SETTING_PRODUCTION:
				if(should_clr)
	      {
		      Lcd_Clr();//清屏
		      delay_ms(10);
					Lcd_Clr();//清屏
		      delay_ms(10);
		      should_clr=0;//清空标记
	      }
				DispPowerFlag();
				DispStr(0,0,"发电量:(kW");
				DispCharNext(0X07);//点
				DispStrNext("h)");
				DispDecimal(1,0,production,3);
				DispStrSingleLineNext("                ");
				break;
			case POWER_SETTING_STATE:
				if(should_clr)
	      {
		      Lcd_Clr();//清屏
		      delay_ms(10);
					Lcd_Clr();//清屏
		      delay_ms(10);
		      should_clr=0;//清空标记
					refresh_inverse=1;
	      }
				DispPowerFlag();
				DispStr(0,0,"正常");
			  DispStr(1,0,"过压");
			  DispStr(2,0,"欠压");
			  DispStr(3,0,"过流");
			  if(refresh_inverse)
				{
					refresh_inverse=0;
					switch(power_state_select)
				  {
					  case OPTION_NORMAL:
						  Inverse(0,0,4);
						  break;
					  case OPTION_OVER_VOL:
						  Inverse(1,0,4);
						  break;
					  case OPTION_UNDER_VOL:
						  Inverse(2,0,4);
						  break;
					  case OPTION_OVER_CUR:
						  Inverse(3,0,4);
						  break;
					  default:
						  break;
				  }
				}
				break;
			case POWER_SETTING_TIME:
				if(should_clr)
	      {
		      Lcd_Clr();//清屏
		      delay_ms(10);
					Lcd_Clr();//清屏
		      delay_ms(10);
		      should_clr=0;//清空标记
					refresh_inverse=1;
	      }
				DispPowerFlag();
				if(!is_time_finish)
				{
					DispFigure(0,0,year);
		      DispCharNext('/');
		      if(mon<10)
		      {
			      DispFigureNext(0);
		      }
		      DispFigureNext(mon);
		      DispCharNext('/');
		      if(day<10)
		      {
			      DispFigureNext(0);
		      }
		      DispFigureNext(day);
		      DispBlankRemainLast();
					SetPosition(1,0);
		      if(hour<10)
		      {
			      DispFigureNext(0);
		      }
		      DispFigureNext(hour);
		      DispCharNext(':');
		      if(min<10)
		      {
			      DispFigureNext(0);
		      }
		      DispFigureNext(min);
		      DispCharNext(':');
		      if(sec<10)
		      {
			      DispFigureNext(0);
		      }
		      DispFigureNext(sec);
		      DispStrSingleLineNext("         ");
		      switch(RTC_Get_Week(year,mon,day))
		      {
			      case 0:
				      DispStr(2,0,"星期日");
				      break;
			      case 1:
				      DispStr(2,0,"星期一");
				      break;
			      case 2:
				      DispStr(2,0,"星期二");
				      break;
			      case 3:
				      DispStr(2,0,"星期三");
				      break;
			      case 4:
				      DispStr(2,0,"星期四");
			       	break;
			      case 5:
				      DispStr(2,0,"星期五");
			      	break;
			      case 6:
			      	DispStr(2,0,"星期六");
				      break;
			      default:
				      break;
		      }
					DispStr(3,6,"保存");
					if(refresh_inverse)
					{
						if(power_time_mode==POWER_TIME_SELECT_BIT)
						{
							refresh_inverse=0;
						}
						switch(power_time_select)
						{
							case 0:
								Inverse(0,0,4);
								break;
							case 1:
								Inverse(0,5,2);
								break;
							case 2:
								Inverse(0,8,2);
								break;
							case 3:
								Inverse(1,0,2);
								break;
							case 4:
								Inverse(1,3,2);
								break;
							case 5:
								Inverse(1,6,2);
								break;
							case 6:
								Inverse(3,12,4);
							  refresh_inverse=0;
								break;
							default:
								break;
						}
					}
				}else if(first_time_show){
					Time_Show();
					first_time_show=0;
				}
				break;
			case POWER_SETTING_RATIO:
				
				if(should_clr)
	      {
		      Lcd_Clr();//清屏
		      delay_ms(10);
					Lcd_Clr();//清屏
		      delay_ms(10);
		      should_clr=0;//清空标记
					refresh_inverse=1;
	      }
				//DispPowerFlag();
				switch(power_ratio_page)
				{
					case RATIO_DC:
						DispStr(0,0,"光伏修正系数1:");
					  //DispBlankRemainLast();
					  DispStr(1,0,"RU:");
					  DispDecimalNext(ratio_vol_temp,3);
					  DispStrSingleLineNext("            ");
					  DispStr(2,0,"RI:");
					  DispDecimalNext(ratio_cur_temp,3);
					  DispStrSingleLineNext("            ");
						break;
					case RATIO_DC1:
						DispStr(0,0,"光伏修正系数2:");
					  //DispBlankRemainLast();
					  DispStr(1,0,"RU:");
					  DispDecimalNext(ratio_vol_temp,3);
					  DispStrSingleLineNext("            ");
					  DispStr(2,0,"RI:");
					  DispDecimalNext(ratio_cur_temp,3);
					  DispStrSingleLineNext("            ");
						break;
					case RATIO_A:
						DispStr(0,0,"逆变输出(A):");
					  //DispBlankRemainLast();
					  DispStr(1,0,"RU:");
					  DispDecimalNext(ratio_vol_temp,3);
					  DispStrSingleLineNext("            ");
					  DispStr(2,0,"RI:");
					  DispDecimalNext(ratio_cur_temp,3);
					  DispStrSingleLineNext("            ");
						break;
					case RATIO_B:
						DispStr(0,0,"逆变输出(B):");
					  //DispBlankRemainLast();
					  DispStr(1,0,"RU:");
					  DispDecimalNext(ratio_vol_temp,3);
					  DispStrSingleLineNext("            ");
					  DispStr(2,0,"RI:");
					  DispDecimalNext(ratio_cur_temp,3);
					  DispStrSingleLineNext("            ");
						break;
					case RATIO_C:
						DispStr(0,0,"逆变输出(C):");
					  //DispBlankRemainLast();
					  DispStr(1,0,"RU:");
					  DispDecimalNext(ratio_vol_temp,3);
					  DispStrSingleLineNext("            ");
					  DispStr(2,0,"RI:");
					  DispDecimalNext(ratio_cur_temp,3);
					  DispStrSingleLineNext("            ");
						break;
					default:
						break;
				}
				if(power_ratio_mode!=POWER_RATIO_VIEW)
				{
					DispStr(3,6,"保存");
					if(refresh_inverse)
					{
						if(power_ratio_mode==POWER_RATIO_SELECT_BIT)
					  {
						  refresh_inverse=0;
					  }
						switch(ratio_which_bit)
					  {
						  case 0:
							  Inverse(1,3,1);
							  break;
						  case 1:
						  	Inverse(1,5,1);
							  break;
						  case 2:
							  Inverse(1,6,1);
							  break;
						  case 3:
							  Inverse(1,7,1);
						  	break;
					  	case 4:
						    Inverse(2,3,1);
						  	break;
					  	case 5:
						  	Inverse(2,5,1);
						  	break;
						  case 6:
						  	Inverse(2,6,1);
							  break;
						  case 7:
							  Inverse(2,7,1);
						  	break;
						  case 8:
							  Inverse(3,12,4);
						    refresh_inverse=0;
						  	break;
					  	default:
						  	break;
				  	}
					}
				}
				DispPowerFlag();
				break;
			case POWER_WARN:
				if(should_clr)
	      {
		      Lcd_Clr();//清屏
		      delay_ms(10);
					Lcd_Clr();//清屏
		      delay_ms(10);
		      should_clr=0;//清空标记
					refresh_inverse=1;
	      }
				DispPowerFlag();
				DispStr(0,3,"警告");
				DispStr(1,0,"是否要切换模式？");
			  DispStr(2,2,"是");
			  DispStr(2,5,"否");
			  if(refresh_inverse)
				{
					refresh_inverse=0;
					switch(warn_select)
					{
						case WARN_YES:
							Inverse(2,3,4);
							break;
						case WARN_NO:
							Inverse(2,9,4);
							break;
						default:
							break;
					}
				}
				break;
			default:
				break;
		}
	}
	display_flag=0;//清空标志
//	if(completed)
//	{
//		completed=0;//清零标记
//		TIM_Cmd(TIM1, ENABLE);//重新使能定时器，用于触发ADC
//	}
}

void Display(void)
{
	//用于调整显示屏刷新频率
	refresh_prd++;
	if(display==INVERTER_DATA_INTERFACE || display==POWER_DATA_INTERFACE || display==POWER_SETTING_PRODUCTION)
	{
		if(refresh_prd<REFRESH_PRD_FAST)
		{
			return;
		}
		if(completed)
		{
			calculate();
			refresh_prd=0;//清空显示屏刷新周期计数器
		  show();
			completed=0;//清零标记
			TIM_Cmd(TIM1, ENABLE);//重新使能定时器，用于触发ADC
		}
	}else{
		if(refresh_prd<REFRESH_PRD_SLOW)
		{
			if((refresh_prd%REFRESH_PRD_FAST)==0)
			{
				if(completed)
				{
					calculate();
					completed=0;//清零标记
					TIM_Cmd(TIM1, ENABLE);//重新使能定时器，用于触发ADC
				}
			}
			return;
		}
		if(completed)
		{
			calculate();
			refresh_prd=0;//清空显示屏刷新周期计数器
		  show();
			completed=0;//清零标记
			TIM_Cmd(TIM1, ENABLE);//重新使能定时器，用于触发ADC
		}
	}
	if(refresh_prd>=PRD_OVERFLOW)
	{
		refresh_prd=0;
	}
}


