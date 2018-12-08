#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h" 
#include "user_interface.h"
#include "rtc.h"
#include "eeprom.h"
#include "calculate.h"
#include "rs232.h"
#include "timer.h"
#include "dsp.h"
//该程序用于定义按键的相关操作，包括四个按键按下后处理的相关逻辑


u8 inverter_deadzone_temp;//缓存选择死区时间，用于取消选择
u8 inverter_invalid_pluse_temp;//缓存选择无效脉冲隔离，用于取消选择
u8 inverter_on_off_temp;//缓存设置前的启动与停止选择，用于取消选择时
u8 ratio_bit[8];//保存修正系数的各个位置数
u8 byte_vol[3];//将浮点数转换为3字节数据，第一个元素为整数部分，后两字节为小数部分，电压数据
u8 byte_cur[3];//将浮点数转换为3字节数据，第一个元素为整数部分，后两字节为小数部分，电流数据
float ratio_vol=1;//保存设置前的值
float ratio_cur=1;//保存设置前的值
u8 i=0;

//u8 rs232_send_buf[3];

//按键初始化函数
void KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);//使能PORTA,PORTE时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;//KEY0-KEY3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE0,1,2,3

}
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY0按下     1
//2，KEY1按下     3
//3，KEY2按下     5
//4，KEY3按下     2
//注意此函数有响应优先级,KEY0>KEY1>KEY2>KEY3!!
//u8 KEY_Scan(u8 mode)
//{	 
//	static u8 key_up=1;//按键按松开标志
//	if(mode)key_up=1;  //支持连按		  
//	if(key_up&&(KEY0==0||KEY1==0||KEY2==0||KEY3==0))
//	{
//		delay_ms(10);//去抖动 
//		key_up=0;
//		if(KEY0==0)return KEY0_PRES;
//		else if(KEY1==0)return KEY1_PRES;
//		else if(KEY2==0)return KEY2_PRES;
//		else if(KEY3==0)return KEY3_PRES;
//	}else if(KEY0==1&&KEY1==1&&KEY2==1&&KEY3==1)key_up=1; 	    
// 	return 0;// 无按键按下
//}

void Button_Up(void)
{
	if(mode==INVERTER_MODE)//逆变器模式
	{
		switch(display)
		{
			case INVERTER_DATA_INTERFACE:
				refresh_prd=REFRESH_PRD_SLOW;
		    should_clr=1;
			  if(inverter_data_page==0)
				{
					inverter_data_page=INVERTER_DATA_PAGE;
				}
				inverter_data_page--;
				break;
			case INVERTER_SETTING_INTERFACE:
				refresh_prd=REFRESH_PRD_SLOW;
			  refresh_inverse=1;
			  if(inverter_setting_select==0)
				{
					inverter_setting_select=INVERTER_SETTING_CHOICE;
					should_clr=1;
				}
				inverter_setting_select--;
				break;
			case INVERTER_SETTING_ON_OFF:
				refresh_prd=REFRESH_PRD_SLOW;
			  refresh_inverse=1;
			  if(inverter_on_off_select==0)
				{
					inverter_on_off_select=INVERTER_ON_OFF_CHOICE;
				}
				inverter_on_off_select--;
				break;
			case INVERTER_SETTING_MODE:
				refresh_prd=REFRESH_PRD_SLOW;
			  refresh_inverse=1;
			  if(inverter_mode_select==0)
				{
					inverter_mode_select=INVERTER_MODE_CHOICE;
				}
				inverter_mode_select--;
				break;
			case INVERTER_SETTING_DEADZONE:
				refresh_prd=REFRESH_PRD_SLOW;
			  refresh_inverse=1;
			  if(inverter_deadzone_select==0)
				{
					inverter_deadzone_select=INVERTER_DEADZONE_CHOICE;
				}
				inverter_deadzone_select--;
				break;
			case INVERTER_SETTING_INVALID_PLUSE:
				refresh_prd=REFRESH_PRD_SLOW;
			  refresh_inverse=1;
			  if(inverter_invalid_pluse_select==0)
				{
					inverter_invalid_pluse_select=INVERTER_INVALID_PLUSE_CHOICE;
				}
				inverter_invalid_pluse_select--;
				break;
			case INVERTER_SETTING_STATE:
				break;
			case INVERTER_FAULT_STATE:
				break;
			case INVERTER_SETTING_TIME:
				refresh_prd=REFRESH_PRD_SLOW;
				if(is_time_finish)
				{
					//refresh_inverse=1;
					//is_time=!is_time;
				}else{
					switch(inverter_time_mode)
					{
						case INVERTER_TIME_SELECT_BIT:
							refresh_inverse=1;
							if(inverter_time_select==0)
				      {
					      inverter_time_select=INVERTER_TIME_CHOICE;
				      }
				      inverter_time_select--;
							break;
						case INVERTER_TIME_SET_BIT:
							switch(inverter_time_select)
							{
								case 0:
									year++;
								  if(year>=2100)
									{
										year=2017;
									}
									break;
								case 1:
									mon++;
								  if(mon>=13)
									{
										mon=1;
									}
									break;
								case 2:
									day++;
								  if(Is_Leap_Year(year)&&mon==2)
									{
										if(day>=29)
										{
											day=1;
										}
										break;
									}
									if(day>=mon_table[mon-1])
									{
										day=1;
									}
									break;
								case 3:
									hour++;
									if(hour>=24)
									{
										hour=0;
									}
									break;
								case 4:
									min++;
									if(min>=60)
									{
										min=0;
									}
									break;
								case 5:
									sec++;
								  if(sec>=60)
									{
										sec=0;
									}
									break;
								case 6:
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
			case INVERTER_SETTING_RATIO:
				refresh_prd=REFRESH_PRD_SLOW;
			  switch(inverter_ratio_mode)
				{
					case INVERTER_RATIO_VIEW:
						should_clr=1;
			      if(inverter_ratio_page==0)
				    {
					    inverter_ratio_page=INVERTER_RATIO_PAGE;
				    }
				    inverter_ratio_page--;
						switch(inverter_ratio_page)
						{
							case RATIO_DC:
								ratio_vol_temp=vol_dc_ratio;
							  ratio_cur_temp=cur_dc_ratio;
								break;
							case RATIO_DC1:
								ratio_vol_temp=vol_dc1_ratio;
							  ratio_cur_temp=cur_dc1_ratio;
								break;
						 case RATIO_A:
								ratio_vol_temp=vol_a_ratio;
						    ratio_cur_temp=cur_a_ratio;
						    break;
					   case RATIO_B:
							 	ratio_vol_temp=vol_b_ratio;
							  ratio_cur_temp=cur_b_ratio;
						  	break;
					   case RATIO_C:
							  ratio_vol_temp=vol_c_ratio;
							  ratio_cur_temp=cur_c_ratio;
							  break;
						  default:
						  	break;
						}
						ratio_vol=ratio_vol_temp;
						ratio_cur=ratio_cur_temp;
						break;
					case INVERTER_RATIO_SELECT_BIT:
						refresh_inverse=1;
						if(ratio_which_bit==0)
						{
							ratio_which_bit=9;
						}
						ratio_which_bit--;
						break;
					case INVERTER_RATIO_SET_BIT:
						if(ratio_which_bit!=8)
						{
							ratio_bit[ratio_which_bit]++;
							if(ratio_bit[ratio_which_bit]>=10)
							{
								ratio_bit[ratio_which_bit]=0;
							}
//							ratio_vol_temp=ratio_bit[0]+(ratio_bit[1]*10000+ratio_bit[2]*1000+ratio_bit[3]*100+1+0.000001)/100000.0;
//							ratio_cur_temp=ratio_bit[4]+(ratio_bit[5]*10000+ratio_bit[6]*1000+ratio_bit[7]*100+1+0.000001)/100000.0;
							ratio_vol_temp=ratio_bit[0]+(ratio_bit[1]*100+ratio_bit[2]*10+ratio_bit[3])*1.0/1000.0;
							ratio_cur_temp=ratio_bit[4]+(ratio_bit[5]*100+ratio_bit[6]*10+ratio_bit[7])*1.0/1000.0;
						}
						break;
					default:
						break;
				}
        break;				
			case INVERTER_WARN:
				refresh_prd=REFRESH_PRD_SLOW;
				warn_select=!warn_select;
			  refresh_inverse=1;
				break;
			default:
				break;
		}
	}else if(mode==POWER_MODE)//功率模块模式
	{
		switch(display)
		{
			case POWER_DATA_INTERFACE:
				refresh_prd=REFRESH_PRD_SLOW;
		    should_clr=1;
			  if(power_data_page==0)
				{
					power_data_page=POWER_DATA_PAGE;
				}
				power_data_page--;
				break;
			case POWER_SETTING_INTERFACE:
				refresh_prd=REFRESH_PRD_SLOW;
			  refresh_inverse=1;
			  if(power_setting_select==0)
				{
					power_setting_select=POWER_SETTING_CHOICE;
				}
				power_setting_select--;
				break;
			case POWER_SETTING_MODE:
				refresh_prd=REFRESH_PRD_SLOW;
			  refresh_inverse=1;
			  if(power_mode_select==0)
				{
					power_mode_select=POWER_MODE_CHOICE;
				}
				power_mode_select--;
				break;
			case POWER_SETTING_PRODUCTION:
				break;
			case POWER_SETTING_STATE:
				break;
			case POWER_SETTING_TIME:
				refresh_prd=REFRESH_PRD_SLOW;
				if(is_time_finish)
				{
					//refresh_inverse=1;
					//is_time=!is_time;
				}else{
					switch(power_time_mode)
					{
						case POWER_TIME_SELECT_BIT:
							refresh_inverse=1;
							if(power_time_select==0)
				      {
					      power_time_select=POWER_TIME_CHOICE;
				      }
				      power_time_select--;
							break;
						case POWER_TIME_SET_BIT:
							switch(power_time_select)
							{
								case 0:
									year++;
								  if(year>=2100)
									{
										year=2017;
									}
									break;
								case 1:
									mon++;
								  if(mon>=13)
									{
										mon=1;
									}
									break;
								case 2:
									day++;
								  if(Is_Leap_Year(year)&&mon==2)
									{
										if(day>=29)
										{
											day=1;
										}
										break;
									}
									if(day>=mon_table[mon-1])
									{
										day=1;
									}
									break;
								case 3:
									hour++;
									if(hour>=24)
									{
										hour=0;
									}
									break;
								case 4:
									min++;
									if(min>=60)
									{
										min=0;
									}
									break;
								case 5:
									sec++;
								  if(sec>=60)
									{
										sec=0;
									}
									break;
								case 6:
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
			case POWER_SETTING_RATIO:
				refresh_prd=REFRESH_PRD_SLOW;
			  switch(power_ratio_mode)
				{
					case POWER_RATIO_VIEW:
						should_clr=1;
			      if(power_ratio_page==0)
				    {
					    power_ratio_page=POWER_RATIO_PAGE;
				    }
				    power_ratio_page--;
						switch(power_ratio_page)
						{
							case RATIO_DC:
								ratio_vol_temp=vol_dc_ratio;
							  ratio_cur_temp=cur_dc_ratio;
								break;
							case RATIO_DC1:
								ratio_vol_temp=vol_dc1_ratio;
							  ratio_cur_temp=cur_dc1_ratio;
								break;
						 case RATIO_A:
								ratio_vol_temp=vol_a_ratio;
						    ratio_cur_temp=cur_a_ratio;
						    break;
					   case RATIO_B:
							 	ratio_vol_temp=vol_b_ratio;
							  ratio_cur_temp=cur_b_ratio;
						  	break;
					   case RATIO_C:
							  ratio_vol_temp=vol_c_ratio;
							  ratio_cur_temp=cur_c_ratio;
							  break;
						  default:
						  	break;
						}
						ratio_vol=ratio_vol_temp;
						ratio_cur=ratio_cur_temp;
						break;
					case POWER_RATIO_SELECT_BIT:
						refresh_inverse=1;
						if(ratio_which_bit==0)
						{
							ratio_which_bit=9;
						}
						ratio_which_bit--;
						break;
					case POWER_RATIO_SET_BIT:
						if(ratio_which_bit!=8)
						{
							ratio_bit[ratio_which_bit]++;
							if(ratio_bit[ratio_which_bit]>=10)
							{
								ratio_bit[ratio_which_bit]=0;
							}
							ratio_vol_temp=ratio_bit[0]+(ratio_bit[1]*100+ratio_bit[2]*10+ratio_bit[3])*1.0/1000.0;
							ratio_cur_temp=ratio_bit[4]+(ratio_bit[5]*100+ratio_bit[6]*10+ratio_bit[7])*1.0/1000.0;
						}
						break;
					default:
						break;
				}
				break;
			case POWER_WARN:
				refresh_prd=REFRESH_PRD_SLOW;
				warn_select=!warn_select;
			  refresh_inverse=1;
				break;
			default:
				break;
		}
	}
}

void Button_Down(void)
{
	if(mode==INVERTER_MODE)//逆变器模式
	{
		switch(display)
		{
			case INVERTER_DATA_INTERFACE:
				refresh_prd=REFRESH_PRD_SLOW;
		    should_clr=1;
			  inverter_data_page++;
			  if(inverter_data_page==INVERTER_DATA_PAGE)
				{
					inverter_data_page=0;
				}
				break;
			case INVERTER_SETTING_INTERFACE:
				refresh_prd=REFRESH_PRD_SLOW;
			  refresh_inverse=1;
			  inverter_setting_select++;
			  if(inverter_setting_select==INVERTER_SETTING_CHOICE)
				{
					inverter_setting_select=0;
					should_clr=1;
				}
				break;
			case INVERTER_SETTING_ON_OFF:
				refresh_prd=REFRESH_PRD_SLOW;
			  refresh_inverse=1;
			  inverter_on_off_select++;
			  if(inverter_on_off_select==INVERTER_ON_OFF_CHOICE)
				{
					inverter_on_off_select=0;
				}
			  break;
			case INVERTER_SETTING_MODE:
				refresh_prd=REFRESH_PRD_SLOW;
			  refresh_inverse=1;
			  inverter_mode_select++;
			  if(inverter_mode_select==INVERTER_MODE_CHOICE)
				{
					inverter_mode_select=0;
				}
				break;
			case INVERTER_SETTING_DEADZONE:
				refresh_prd=REFRESH_PRD_SLOW;
			  refresh_inverse=1;
			  inverter_deadzone_select++;
			  if(inverter_deadzone_select==INVERTER_DEADZONE_CHOICE)
				{
					inverter_deadzone_select=0;
				}
				break;
			case INVERTER_SETTING_INVALID_PLUSE:
				refresh_prd=REFRESH_PRD_SLOW;
			  refresh_inverse=1;
			  inverter_invalid_pluse_select++;
			  if(inverter_invalid_pluse_select==INVERTER_INVALID_PLUSE_CHOICE)
				{
					inverter_invalid_pluse_select=0;
				}
				break;
			case INVERTER_SETTING_STATE:
				break;
			case INVERTER_FAULT_STATE:
				break;
			case INVERTER_SETTING_TIME:
				refresh_prd=REFRESH_PRD_SLOW;
				if(is_time_finish)
				{
					//refresh_inverse=1;
					//is_time=!is_time;
				}else{
					switch(inverter_time_mode)
					{
						case INVERTER_TIME_SELECT_BIT:
							refresh_inverse=1;
							inverter_time_select++;
			        if(inverter_time_select==INVERTER_TIME_CHOICE)
				      {
					      inverter_time_select=0;
				      }
							break;
						case INVERTER_TIME_SET_BIT:
							switch(inverter_time_select)
							{
								case 0:
								  if(year<=2017)
									{
										year=2100;
									}
									year--;
									break;
								case 1:
									if(mon<=1)
									{
										mon=13;
									}
									mon--;
									break;
								case 2:
								  if(Is_Leap_Year(year)&&mon==2)
									{
										if(day<=1)
										{
											day=30;
										}
										day--;
										break;
									}
									if(day<=1)
									{
										day=mon_table[mon-1]+1;
									}
									day--;
									break;
								case 3:
									if(hour<=0)
									{
										hour=24;
									}
									hour--;
									break;
								case 4:
									if(min<=0)
									{
										min=60;
									}
									min--;
									break;
								case 5:
									if(sec<=0)
									{
										sec=60;
									}
									sec--;
									break;
								case 6:
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
			case INVERTER_SETTING_RATIO:
				refresh_prd=REFRESH_PRD_SLOW;
			  switch(inverter_ratio_mode)
				{
					case INVERTER_RATIO_VIEW:
						should_clr=1;
					  inverter_ratio_page++;
			      if(inverter_ratio_page==INVERTER_RATIO_PAGE)
				    {
					    inverter_ratio_page=0;
				    }
						switch(inverter_ratio_page)
						{
							case RATIO_DC:
								ratio_vol_temp=vol_dc_ratio;
							  ratio_cur_temp=cur_dc_ratio;
								break;
							case RATIO_DC1:
								ratio_vol_temp=vol_dc1_ratio;
							  ratio_cur_temp=cur_dc1_ratio;
								break;
						 case RATIO_A:
								ratio_vol_temp=vol_a_ratio;
						    ratio_cur_temp=cur_a_ratio;
						    break;
					   case RATIO_B:
							 	ratio_vol_temp=vol_b_ratio;
							  ratio_cur_temp=cur_b_ratio;
						  	break;
					   case RATIO_C:
							  ratio_vol_temp=vol_c_ratio;
							  ratio_cur_temp=cur_c_ratio;
							  break;
						  default:
						  	break;
						}
						ratio_vol=ratio_vol_temp;
						ratio_cur=ratio_cur_temp;
						break;
					case INVERTER_RATIO_SELECT_BIT:
						refresh_inverse=1;
						ratio_which_bit++;
						if(ratio_which_bit==9)
						{
							ratio_which_bit=0;
						}
						break;
					case INVERTER_RATIO_SET_BIT:
						if(ratio_which_bit!=8)
						{
							if(ratio_bit[ratio_which_bit]<=0)
							{
								ratio_bit[ratio_which_bit]=10;
							}
							ratio_bit[ratio_which_bit]--;
							ratio_vol_temp=ratio_bit[0]+(ratio_bit[1]*100+ratio_bit[2]*10+ratio_bit[3])*1.0/1000.0;
							ratio_cur_temp=ratio_bit[4]+(ratio_bit[5]*100+ratio_bit[6]*10+ratio_bit[7])*1.0/1000.0;
						}
						break;
					default:
						break;
					}
        break;
			case INVERTER_WARN:
				refresh_prd=REFRESH_PRD_SLOW;
				warn_select=!warn_select;
			  refresh_inverse=1;
				break;
			default:
				break;
		}
	}else if(mode==POWER_MODE)//功率模块模式
	{
		switch(display)
		{
			case POWER_DATA_INTERFACE:
				refresh_prd=REFRESH_PRD_SLOW;
		    should_clr=1;
			  power_data_page++;
			  if(power_data_page==POWER_DATA_PAGE)
				{
					power_data_page=0;
				}
				break;
			case POWER_SETTING_INTERFACE:
				refresh_prd=REFRESH_PRD_SLOW;
			  refresh_inverse=1;
			  power_setting_select++;
			  if(power_setting_select==POWER_SETTING_CHOICE)
				{
					power_setting_select=0;
				}
				break;
			case POWER_SETTING_MODE:
				refresh_prd=REFRESH_PRD_SLOW;
			  refresh_inverse=1;
			  power_mode_select++;
			  if(power_mode_select==POWER_MODE_CHOICE)
				{
					power_mode_select=0;
				}
				break;
			case POWER_SETTING_PRODUCTION:
				break;
			case POWER_SETTING_STATE:
				break;
			case POWER_SETTING_TIME:
				refresh_prd=REFRESH_PRD_SLOW;
				if(is_time_finish)
				{
					//refresh_inverse=1;
					//is_time=!is_time;
				}else{
					switch(power_time_mode)
					{
						case POWER_TIME_SELECT_BIT:
							refresh_inverse=1;
							power_time_select++;
			        if(power_time_select==POWER_TIME_CHOICE)
				      {
					      power_time_select=0;
				      }
							break;
						case POWER_TIME_SET_BIT:
							switch(power_time_select)
							{
								case 0:
								  if(year<=2017)
									{
										year=2100;
									}
									year--;
									break;
								case 1:
									if(mon<=1)
									{
										mon=13;
									}
									mon--;
									break;
								case 2:
								  if(Is_Leap_Year(year)&&mon==2)
									{
										if(day<=1)
										{
											day=30;
										}
										day--;
										break;
									}
									if(day<=1)
									{
										day=mon_table[mon-1]+1;
									}
									day--;
									break;
								case 3:
									if(hour<=0)
									{
										hour=24;
									}
									hour--;
									break;
								case 4:
									if(min<=0)
									{
										min=60;
									}
									min--;
									break;
								case 5:
									if(sec<=0)
									{
										sec=60;
									}
									sec--;
									break;
								case 6:
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
			case POWER_SETTING_RATIO:
				refresh_prd=REFRESH_PRD_SLOW;
			  switch(power_ratio_mode)
				{
					case POWER_RATIO_VIEW:
						should_clr=1;
					  power_ratio_page++;
			      if(power_ratio_page==POWER_RATIO_PAGE)
				    {
					    power_ratio_page=0;
				    }
						switch(power_ratio_page)
						{
							case RATIO_DC:
								ratio_vol_temp=vol_dc_ratio;
							  ratio_cur_temp=cur_dc_ratio;
								break;
							case RATIO_DC1:
								ratio_vol_temp=vol_dc1_ratio;
							  ratio_cur_temp=cur_dc1_ratio;
								break;
						  case RATIO_A:
								ratio_vol_temp=vol_a_ratio;
						    ratio_cur_temp=cur_a_ratio;
						    break;
					    case RATIO_B:
							 	ratio_vol_temp=vol_b_ratio;
							  ratio_cur_temp=cur_b_ratio;
						  	break;
					    case RATIO_C:
							  ratio_vol_temp=vol_c_ratio;
							  ratio_cur_temp=cur_c_ratio;
							  break;
						  default:
						  	break;
						}
						ratio_vol=ratio_vol_temp;
						ratio_cur=ratio_cur_temp;
						break;
					case POWER_RATIO_SELECT_BIT:
						refresh_inverse=1;
						ratio_which_bit++;
						if(ratio_which_bit==9)
						{
							ratio_which_bit=0;
						}
						break;
					case POWER_RATIO_SET_BIT:
						if(ratio_which_bit!=8)
						{
							if(ratio_bit[ratio_which_bit]<=0)
							{
								ratio_bit[ratio_which_bit]=10;
							}
							ratio_bit[ratio_which_bit]--;
							ratio_vol_temp=ratio_bit[0]+(ratio_bit[1]*100+ratio_bit[2]*10+ratio_bit[3])*1.0/1000.0;
							ratio_cur_temp=ratio_bit[4]+(ratio_bit[5]*100+ratio_bit[6]*10+ratio_bit[7])*1.0/1000.0;
						}
						break;
					default:
						break;
					}
				break;
			case POWER_WARN:
				refresh_prd=REFRESH_PRD_SLOW;
				warn_select=!warn_select;
			  refresh_inverse=1;
				break;
			default:
				break;
		}
	}
}

void Button_Cancel(void)
{
	if(mode==INVERTER_MODE)//逆变器模式
	{
		switch(display)
		{
			case INVERTER_DATA_INTERFACE:
				break;
			case INVERTER_SETTING_INTERFACE:
				refresh_prd=REFRESH_PRD_SLOW;
		    should_clr=1;
				display=INVERTER_DATA_INTERFACE;
			  inverter_data_page=0;
				break;
			case INVERTER_SETTING_ON_OFF:
				refresh_prd=REFRESH_PRD_SLOW;
		    should_clr=1;
			  refresh_inverse=1;
			  inverter_on_off_select=inverter_on_off_temp;
				display=INVERTER_SETTING_INTERFACE;
				break;
			case INVERTER_SETTING_MODE:
				refresh_prd=REFRESH_PRD_SLOW;
		    should_clr=1;
			  refresh_inverse=1;
				display=INVERTER_SETTING_INTERFACE;
				break;
			case INVERTER_SETTING_DEADZONE:
				refresh_prd=REFRESH_PRD_SLOW;
		    should_clr=1;
			  refresh_inverse=1;
			  inverter_deadzone_select=inverter_deadzone_temp;
				display=INVERTER_SETTING_INTERFACE;
				break;
			case INVERTER_SETTING_INVALID_PLUSE:
				refresh_prd=REFRESH_PRD_SLOW;
		    should_clr=1;
			  refresh_inverse=1;
			  inverter_invalid_pluse_select=inverter_invalid_pluse_temp;
				display=INVERTER_SETTING_INTERFACE;
				break;
			case INVERTER_SETTING_STATE:
				refresh_prd=REFRESH_PRD_SLOW;
		    should_clr=1;
			  refresh_inverse=1;
				display=INVERTER_SETTING_INTERFACE;
				break;
			case INVERTER_FAULT_STATE:
				refresh_prd=REFRESH_PRD_SLOW;
		    should_clr=1;
			  refresh_inverse=1;
				display=INVERTER_SETTING_STATE;
				break;
			case INVERTER_SETTING_TIME:
				refresh_prd=REFRESH_PRD_SLOW;
		    should_clr=1;
			  refresh_inverse=1;
			  if(is_time_finish)
				{
					//is_time=0;
					display=INVERTER_SETTING_INTERFACE;
				}else{
					//is_time=0;
					is_time_finish=1;
					inverter_time_select=0;
					inverter_time_mode=POWER_TIME_SELECT_BIT;
					first_time_show=1;
				}
				break;
			case INVERTER_SETTING_RATIO:
				switch(inverter_ratio_mode)
				{
					case INVERTER_RATIO_VIEW:
						refresh_prd=REFRESH_PRD_SLOW;
		        should_clr=1;
			      refresh_inverse=1;
					  ratio_which_bit=0;
				    display=INVERTER_SETTING_INTERFACE;
						break;
					case INVERTER_RATIO_SELECT_BIT:
					case INVERTER_RATIO_SET_BIT:
						refresh_prd=REFRESH_PRD_SLOW;
		        should_clr=1;
			      //refresh_inverse=1;
					  ratio_which_bit=0;
				    inverter_ratio_mode=INVERTER_RATIO_VIEW;
					  ratio_vol_temp=ratio_vol;
						ratio_cur_temp=ratio_cur;
						break;
					default:
						break;
					}
        break;
			case INVERTER_WARN:
				refresh_prd=REFRESH_PRD_SLOW;
				if(inverter_setting_select==ITEM_ON_OFF)
				{
					warn_select=0;
					should_clr=1;
					refresh_inverse=1;
					display=INVERTER_SETTING_ON_OFF;
				}
				if(inverter_setting_select==ITEM_MODE)
				{
					warn_select=0;
					should_clr=1;
					refresh_inverse=1;
					display=INVERTER_SETTING_MODE;
				}
				break;
			default:
				break;
		}
	}else if(mode==POWER_MODE)//功率模块模式
	{
		switch(display)
		{
			case POWER_DATA_INTERFACE:
				break;
			case POWER_SETTING_INTERFACE:
				refresh_prd=REFRESH_PRD_SLOW;
		    should_clr=1;
				display=POWER_DATA_INTERFACE;
			  power_data_page=0;
				break;
			case POWER_SETTING_MODE:
				refresh_prd=REFRESH_PRD_SLOW;
		    should_clr=1;
			  refresh_inverse=1;
				display=POWER_SETTING_INTERFACE;
				break;
			case POWER_SETTING_PRODUCTION:
				refresh_prd=REFRESH_PRD_SLOW;
		    should_clr=1;
			  refresh_inverse=1;
				display=POWER_SETTING_INTERFACE;
				break;
			case POWER_SETTING_STATE:
				refresh_prd=REFRESH_PRD_SLOW;
		    should_clr=1;
			  refresh_inverse=1;
				display=POWER_SETTING_INTERFACE;
				break;
			case POWER_SETTING_TIME:
				refresh_prd=REFRESH_PRD_SLOW;
		    should_clr=1;
			  refresh_inverse=1;
			  if(is_time_finish)
				{
					//is_time=0;
					display=POWER_SETTING_INTERFACE;
				}else{
					//is_time=0;
					is_time_finish=1;
					power_time_select=0;
					power_time_mode=POWER_TIME_SELECT_BIT;
					first_time_show=1;
				}
				break;
			case POWER_SETTING_RATIO:
				switch(power_ratio_mode)
				{
					case POWER_RATIO_VIEW:
						refresh_prd=REFRESH_PRD_SLOW;
		        should_clr=1;
			      refresh_inverse=1;
					  ratio_which_bit=0;
				    display=POWER_SETTING_INTERFACE;
						break;
					case POWER_RATIO_SELECT_BIT:
					case POWER_RATIO_SET_BIT:
						refresh_prd=REFRESH_PRD_SLOW;
		        should_clr=1;
			      //refresh_inverse=1;
					  ratio_which_bit=0;
				    power_ratio_mode=POWER_RATIO_VIEW;
					  ratio_vol_temp=ratio_vol;
						ratio_cur_temp=ratio_cur;
						break;
					default:
						break;
					}
				break;
			case POWER_WARN:
				refresh_prd=REFRESH_PRD_SLOW;
				warn_select=0;
			  should_clr=1;
			  refresh_inverse=1;
			  display=POWER_SETTING_MODE;
				break;
			default:
				break;
		}
	}
}

void Button_Ok(void)
{
	if(mode==INVERTER_MODE)//逆变器模式
	{
		switch(display)
		{
			case INVERTER_DATA_INTERFACE:
				refresh_prd=REFRESH_PRD_SLOW;
		    should_clr=1;
			  refresh_inverse=1;
			  inverter_setting_select=0;
				display=INVERTER_SETTING_INTERFACE;
				break;
			case INVERTER_SETTING_INTERFACE:
				refresh_prd=REFRESH_PRD_SLOW;
		    should_clr=1;
			  refresh_inverse=1;
				switch(inverter_setting_select)
				{
					case ITEM_ON_OFF:
						display=INVERTER_SETTING_ON_OFF;
					  inverter_on_off_temp=inverter_on_off_select;
						break;
					case ITEM_MODE:
						display=INVERTER_SETTING_MODE;
					  inverter_mode_select=0;
						break;
					case ITEM_DEADZONE:
						inverter_deadzone_temp=inverter_deadzone_select;
						display=INVERTER_SETTING_DEADZONE;
						break;
					case ITEN_INVALID_PLUSE:
						inverter_invalid_pluse_temp=inverter_invalid_pluse_select;
						display=INVERTER_SETTING_INVALID_PLUSE;
						break;
					case ITEM_STATE:
						display=INVERTER_SETTING_STATE;
						break;
					case ITEM_TIME:
						display=INVERTER_SETTING_TIME;
					  first_time_show=1;
						break;
					case ITEM_RATIO:
						display=INVERTER_SETTING_RATIO;
					  inverter_ratio_mode=INVERTER_RATIO_VIEW;
					  inverter_ratio_page=0;
					  ratio_vol_temp=vol_dc_ratio;
						ratio_cur_temp=cur_dc_ratio;
					  ratio_vol=ratio_vol_temp;
						ratio_cur=ratio_cur_temp;
						break;
					default:
						break;
				}
				break;
			case INVERTER_SETTING_ON_OFF:
				refresh_prd=REFRESH_PRD_SLOW;
				should_clr=1;
				refresh_inverse=1;
				warn_select=0;
				display=INVERTER_WARN;
				break;
			case INVERTER_SETTING_MODE:
			  if(inverter_mode_select)
				{
					refresh_prd=REFRESH_PRD_SLOW;
		      should_clr=1;
			    refresh_inverse=1;
					warn_select=0;
				  display=INVERTER_WARN;
				}
				break;
			case INVERTER_SETTING_DEADZONE:
				refresh_prd=REFRESH_PRD_SLOW;
		    should_clr=1;
			  refresh_inverse=1;
				display=INVERTER_SETTING_INTERFACE;
			  if(AT24CXX_ReadOneByte(100)!=inverter_deadzone_select)
				{
					AT24CXX_WriteOneByte(100,inverter_deadzone_select);
				}
				//rs232_send_buf[0]='S';
				//rs232_send_buf[1]='a'+inverter_deadzone_select;
				//rs232_send_buf[2]='E';
				//RS232_Send_Data(rs232_send_buf,3);
				break;
			case INVERTER_SETTING_INVALID_PLUSE:
				refresh_prd=REFRESH_PRD_SLOW;
		    should_clr=1;
			  refresh_inverse=1;
				display=INVERTER_SETTING_INTERFACE;
			  if(AT24CXX_ReadOneByte(101)!=inverter_invalid_pluse_select)
				{
					AT24CXX_WriteOneByte(101,inverter_invalid_pluse_select);
				}
				break;
			case INVERTER_SETTING_STATE:
				refresh_prd=REFRESH_PRD_SLOW;
		    should_clr=1;
			  refresh_inverse=1;
			  if(inverter_state_select==ITEM_FAULT)
				{
					display=INVERTER_FAULT_STATE;
				}else{
					display=INVERTER_SETTING_INTERFACE;
				}
				break;
			case INVERTER_FAULT_STATE:
				refresh_prd=REFRESH_PRD_SLOW;
		    should_clr=1;
			  refresh_inverse=1;
				display=INVERTER_SETTING_STATE;
				break;
			case INVERTER_SETTING_TIME:
				refresh_prd=REFRESH_PRD_SLOW;
			  if(is_time_finish)
				{
					should_clr=1;
					refresh_inverse=1;
					is_time_finish=0;
					year=calendar.w_year;
					mon=calendar.w_month;
					day=calendar.w_date;
					hour=calendar.hour;
					min=calendar.min;
					sec=calendar.sec;
					refresh_inverse=1;
				}else{
					refresh_inverse=1;
					if(inverter_time_select==INVERTER_TIME_CHOICE-1)
				  {
					  inverter_time_select=0;
					  RTC_Set(year,mon,day,hour,min,sec);
					  is_time_finish=1;
				  	inverter_time_mode=INVERTER_TIME_SELECT_BIT;
				  	//refresh_inverse=1;
						should_clr=1;
						first_time_show=1;
						break;
				  }
					switch(inverter_time_mode)
					{
						case INVERTER_TIME_SELECT_BIT:
							inverter_time_mode=INVERTER_TIME_SET_BIT;
							break;
						case INVERTER_TIME_SET_BIT:
							inverter_time_mode=INVERTER_TIME_SELECT_BIT;
							break;
						default:
							break;
					}
				}
				break;
			case INVERTER_SETTING_RATIO:
				refresh_prd=REFRESH_PRD_SLOW;
				switch(inverter_ratio_mode)
				{
					case INVERTER_RATIO_VIEW:
		        should_clr=1;
			      refresh_inverse=1;
					  ratio_which_bit=0;
				    inverter_ratio_mode=INVERTER_RATIO_SELECT_BIT;
						//vol
						ratio_bit[0]=(u16)(ratio_vol_temp*1000)/1000;
						ratio_bit[1]=(u16)(ratio_vol_temp*1000)%1000/100;
						ratio_bit[2]=(u16)(ratio_vol_temp*1000)%100/10;
						ratio_bit[3]=(u16)(ratio_vol_temp*1000)%10;
						//cur
						ratio_bit[4]=(u16)(ratio_cur_temp*1000)/1000;
						ratio_bit[5]=(u16)(ratio_cur_temp*1000)%1000/100;
						ratio_bit[6]=(u16)(ratio_cur_temp*1000)%100/10;
						ratio_bit[7]=(u16)(ratio_cur_temp*1000)%10;
						break;
					case INVERTER_RATIO_SELECT_BIT:
						refresh_prd=REFRESH_PRD_SLOW;
		        //should_clr=1;
					  refresh_inverse=1;
					  if(ratio_which_bit>=8)
						{
							should_clr=1;
							ratio_which_bit=0;
							inverter_ratio_mode=INVERTER_RATIO_VIEW;
							ratio_vol_temp=ratio_bit[0]+(ratio_bit[1]*100+ratio_bit[2]*10+ratio_bit[3])*1.0/1000.0;
							ratio_cur_temp=ratio_bit[4]+(ratio_bit[5]*100+ratio_bit[6]*10+ratio_bit[7])*1.0/1000.0;
							if(ratio_vol_temp>RATIO_UP_RANGE){
								ratio_vol_temp=RATIO_UP_RANGE;
							}
							if(ratio_vol_temp<=RATIO_DOWN_RANGE){
								ratio_vol_temp=RATIO_DOWN_RANGE;
							}
							if(ratio_cur_temp>RATIO_UP_RANGE){
								ratio_cur_temp=RATIO_UP_RANGE;
							}
							if(ratio_cur_temp<=RATIO_DOWN_RANGE){
								ratio_cur_temp=RATIO_DOWN_RANGE;
							}
						  switch(inverter_ratio_page)
						  {
							  case RATIO_DC:
								  vol_dc_ratio=ratio_vol_temp;
							    cur_dc_ratio=ratio_cur_temp;
								  break;
								case RATIO_DC1:
								  vol_dc1_ratio=ratio_vol_temp;
							    cur_dc1_ratio=ratio_cur_temp;
								  break;
						  	case RATIO_A:
							  	vol_a_ratio=ratio_vol_temp;
							    cur_a_ratio=ratio_cur_temp;
								  break;
							  case RATIO_B:
							  	vol_b_ratio=ratio_vol_temp;
							    cur_b_ratio=ratio_cur_temp;
							  	break;
							  case RATIO_C:
							  	vol_c_ratio=ratio_vol_temp;
							    cur_c_ratio=ratio_cur_temp;
							  	break;
						  	default:
						  		break;
						  }
							AT24CXX_Read(inverter_ratio_page*2*3+1,byte_vol,3);
							AT24CXX_Read(inverter_ratio_page*2*3+4,byte_cur,3);
							if(ByteArray2Float(byte_vol,3)!=ratio_vol_temp)
				      {
								Float2ByteArray(ratio_vol_temp,byte_vol,3);
					      AT24CXX_Write(inverter_ratio_page*2*3+1,byte_vol,3);
			      	}
							if(ByteArray2Float(byte_cur,3)!=ratio_cur_temp)
				      {
								Float2ByteArray(ratio_cur_temp,byte_cur,3);
					      AT24CXX_Write(inverter_ratio_page*2*3+4,byte_cur,3);
			      	}
						}else{
							//refresh_inverse=1;
							inverter_ratio_mode=INVERTER_RATIO_SET_BIT;
						}
						break;
					case INVERTER_RATIO_SET_BIT:
						//refresh_prd=REFRESH_PRD_SLOW;
		        //should_clr=1;
			      refresh_inverse=1;
					  //ratio_which_bit=0;
				    inverter_ratio_mode=INVERTER_RATIO_SELECT_BIT;
						break;
					default:
						break;
					}
        break;
			case INVERTER_WARN:
				refresh_prd=REFRESH_PRD_SLOW;
			  if(inverter_setting_select==ITEM_ON_OFF)
				{
					if(warn_select)
					{
						warn_select=0;
						should_clr=1;
						refresh_inverse=1;
						//inverter_on_off_select=!inverter_on_off_select;
						display=INVERTER_SETTING_ON_OFF;
						if(inverter_on_off_select==ITEM_INVERTER_OFF)
						{
							dspOnOff(SHUT_DOWN);
						}
						if(inverter_on_off_select==ITEM_INVERTER_ON)
						{
							dspOnOff(START_UP);
						}
					}else{
						should_clr=1;
						refresh_inverse=1;
						display=INVERTER_SETTING_ON_OFF;
					}
				}
				if(inverter_setting_select==ITEM_MODE)
				{
					if(warn_select)
					{
						warn_select=0;
						should_clr=1;
						mode=POWER_MODE;
						display=POWER_DATA_INTERFACE;
						power_data_page=0;
						Recover_Ratio();
					}else{
						should_clr=1;
						refresh_inverse=1;
						display=INVERTER_SETTING_MODE;
					}
				}
				break;
			default:
				break;
		}
	}else if(mode==POWER_MODE)//功率模块模式
	{
		switch(display)
		{
			case POWER_DATA_INTERFACE:
				refresh_prd=REFRESH_PRD_SLOW;
		    should_clr=1;
			  refresh_inverse=1;
			  power_setting_select=0;
				display=POWER_SETTING_INTERFACE;
				break;
			case POWER_SETTING_INTERFACE:
				refresh_prd=REFRESH_PRD_SLOW;
		    should_clr=1;
			  refresh_inverse=1;
				switch(power_setting_select)
				{
					case OPTION_MODE:
						display=POWER_SETTING_MODE;
					  power_mode_select=1;
						break;
					case OPTION_PRODUCTION:
						display=POWER_SETTING_PRODUCTION;
						break;
					case OPTION_STATE:
						display=POWER_SETTING_STATE;
					  break;
					case OPTION_TIME:
						display=POWER_SETTING_TIME;
					  first_time_show=1;
						break;
					case OPTION_RATIO:
						display=POWER_SETTING_RATIO;
					  power_ratio_mode=POWER_RATIO_VIEW;
					  power_ratio_page=0;
					  ratio_vol_temp=vol_dc_ratio;
						ratio_cur_temp=cur_dc_ratio;
					  ratio_vol=ratio_vol_temp;
						ratio_cur=ratio_cur_temp;
						break;
					default:
						break;
				}
				break;
			case POWER_SETTING_MODE:
				if(!power_mode_select)
				{
					refresh_prd=REFRESH_PRD_SLOW;
		      should_clr=1;
			    refresh_inverse=1;
					warn_select=0;
				  display=POWER_WARN;
				}
				break;
			case POWER_SETTING_PRODUCTION:
				break;
			case POWER_SETTING_STATE:
				break;
			case POWER_SETTING_TIME:
				refresh_prd=REFRESH_PRD_SLOW;
			  if(is_time_finish)
				{
					should_clr=1;
					refresh_inverse=1;
					is_time_finish=0;
					year=calendar.w_year;
					mon=calendar.w_month;
					day=calendar.w_date;
					hour=calendar.hour;
					min=calendar.min;
					sec=calendar.sec;
					refresh_inverse=1;
				}else{
					refresh_inverse=1;
					if(power_time_select==POWER_TIME_CHOICE-1)
				  {
					  power_time_select=0;
					  RTC_Set(year,mon,day,hour,min,sec);
					  is_time_finish=1;
				  	power_time_mode=POWER_TIME_SELECT_BIT;
				  	//refresh_inverse=1;
						should_clr=1;
						first_time_show=1;
						break;
				  }
					switch(power_time_mode)
					{
						case POWER_TIME_SELECT_BIT:
							power_time_mode=POWER_TIME_SET_BIT;
							break;
						case POWER_TIME_SET_BIT:
							power_time_mode=POWER_TIME_SELECT_BIT;
							break;
						default:
							break;
					}
				}
				break;
			case POWER_SETTING_RATIO:
				refresh_prd=REFRESH_PRD_SLOW;
				switch(power_ratio_mode)
				{
					case POWER_RATIO_VIEW:
		        should_clr=1;
			      refresh_inverse=1;
					  ratio_which_bit=0;
				    power_ratio_mode=POWER_RATIO_SELECT_BIT;
						//vol
						ratio_bit[0]=(u16)(ratio_vol_temp*1000)/1000;
						ratio_bit[1]=(u16)(ratio_vol_temp*1000)%1000/100;
						ratio_bit[2]=(u16)(ratio_vol_temp*1000)%100/10;
						ratio_bit[3]=(u16)(ratio_vol_temp*1000)%10;
						//cur
						ratio_bit[4]=(u16)(ratio_cur_temp*1000)/1000;
						ratio_bit[5]=(u16)(ratio_cur_temp*1000)%1000/100;
						ratio_bit[6]=(u16)(ratio_cur_temp*1000)%100/10;
						ratio_bit[7]=(u16)(ratio_cur_temp*1000)%10;
						break;
					case POWER_RATIO_SELECT_BIT:
						refresh_prd=REFRESH_PRD_SLOW;
		        //should_clr=1;
					  refresh_inverse=1;
					  if(ratio_which_bit>=8)
						{
							should_clr=1;
							ratio_which_bit=0;
							power_ratio_mode=POWER_RATIO_VIEW;
							ratio_vol_temp=ratio_bit[0]+(ratio_bit[1]*100+ratio_bit[2]*10+ratio_bit[3])*1.0/1000.0;
							ratio_cur_temp=ratio_bit[4]+(ratio_bit[5]*100+ratio_bit[6]*10+ratio_bit[7])*1.0/1000.0;
							if(ratio_vol_temp>RATIO_UP_RANGE){
								ratio_vol_temp=RATIO_UP_RANGE;
							}
							if(ratio_vol_temp<=RATIO_DOWN_RANGE){
								ratio_vol_temp=RATIO_DOWN_RANGE;
							}
							if(ratio_cur_temp>RATIO_UP_RANGE){
								ratio_cur_temp=RATIO_UP_RANGE;
							}
							if(ratio_cur_temp<=RATIO_DOWN_RANGE){
								ratio_cur_temp=RATIO_DOWN_RANGE;
							}
						  switch(power_ratio_page)
						  {
							  case RATIO_DC:
								  vol_dc_ratio=ratio_vol_temp;
							    cur_dc_ratio=ratio_cur_temp;
								  break;
								case RATIO_DC1:
								  vol_dc1_ratio=ratio_vol_temp;
							    cur_dc1_ratio=ratio_cur_temp;
								  break;
						  	case RATIO_A:
							  	vol_a_ratio=ratio_vol_temp;
							    cur_a_ratio=ratio_cur_temp;
								  break;
							  case RATIO_B:
							  	vol_b_ratio=ratio_vol_temp;
							    cur_b_ratio=ratio_cur_temp;
							  	break;
							  case RATIO_C:
							  	vol_c_ratio=ratio_vol_temp;
							    cur_c_ratio=ratio_cur_temp;
							  	break;
						  	default:
						  		break;
						  }
							AT24CXX_Read(50+power_ratio_page*2*3+1,byte_vol,3);
							AT24CXX_Read(50+power_ratio_page*2*3+4,byte_cur,3);
							if(ByteArray2Float(byte_vol,3)!=ratio_vol_temp)
				      {
								Float2ByteArray(ratio_vol_temp,byte_vol,3);
					      AT24CXX_Write(50+power_ratio_page*2*3+1,byte_vol,3);
			      	}
							if(ByteArray2Float(byte_cur,3)!=ratio_cur_temp)
				      {
								Float2ByteArray(ratio_cur_temp,byte_cur,3);
					      AT24CXX_Write(50+power_ratio_page*2*3+4,byte_cur,3);
			      	}
						}else{
							//refresh_inverse=1;
							power_ratio_mode=POWER_RATIO_SET_BIT;
						}
						break;
					case POWER_RATIO_SET_BIT:
						//refresh_prd=REFRESH_PRD_SLOW;
		        //should_clr=1;
			      refresh_inverse=1;
					  //ratio_which_bit=0;
				    power_ratio_mode=POWER_RATIO_SELECT_BIT;
						break;
					default:
						break;
					}
				break;
			case POWER_WARN:
				refresh_prd=REFRESH_PRD_SLOW;
				if(warn_select)
				{
					warn_select=0;
					should_clr=1;
					mode=INVERTER_MODE;
					display=INVERTER_DATA_INTERFACE;
					TIM_Cmd(TIM4, ENABLE);  //使能TIMx	
		    	USART_Cmd(USART1, ENABLE);
					inverter_data_page=0;
					Recover_Ratio();
				}else{
					should_clr=1;
			    refresh_inverse=1;
			    display=POWER_SETTING_MODE;
				}
				break;
			default:
				break;
		}
	}
}




