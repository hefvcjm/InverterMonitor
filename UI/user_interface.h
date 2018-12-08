#ifndef _USER_INTERFACE_H
#define _USER_INTERFACE_H
#include "sys.h"

#define INVERTER_MODE                    99   //逆变器模式标识
#define INVERTER_DATA_INTERFACE          100  //逆变器模式下数据显示界面标识
#define INVERTER_SETTING_INTERFACE       101  //逆变器模式下设置界面标识
#define INVERTER_SETTING_ON_OFF          102  //启动或停止
#define INVERTER_SETTING_MODE            103  //逆变器模式下模式选择界面标识
#define INVERTER_SETTING_DEADZONE        104  //逆变器模式下死去时间界面标识
#define INVERTER_SETTING_INVALID_PLUSE   105  //逆变器模式下无效脉冲隔离界面标识
#define INVERTER_SETTING_STATE           106  //逆变器模式下工作状态界面标识
#define INVERTER_FAULT_STATE             107  //逆变器模式下故障查看
#define INVERTER_SETTING_TIME            108  //设置时钟
#define INVERTER_SETTING_RATIO           109  //修正系数
#define INVERTER_WARN                    110  //警告

#define INVERTER_DATA_PAGE   6  //主界面数据显示界面个数
#define DATA_DC_DISPLAY    0   //主界面直流数据显示界面ID
#define DATA_DC1_DISPLAY   1   //主界面直流数据显示界面ID
#define DATA_A_DISPLAY     2   //主界面交流A相数据显示界面ID
#define DATA_B_DISPLAY     3   //主界面交流B相数据显示界面ID
#define DATA_C_DISPLAY     4   //主界面交流C相数据显示界面ID
#define DATA_FRE_DISPLAY   5   //电网频率

#define INVERTER_SETTING_CHOICE   7  //逆变器模式下设置选项数目
#define ITEM_ON_OFF               0   //启动与停止
#define ITEM_MODE                 1   //工作模式
#define ITEM_DEADZONE             2   //死区时间
#define ITEN_INVALID_PLUSE        3   //无效脉冲隔离
#define ITEM_STATE                4   //状态
#define ITEM_TIME                 5   //时钟
#define ITEM_RATIO                6   //修正系数

#define INVERTER_ON_OFF_CHOICE   2  
#define ITEM_INVERTER_ON              1   //启动
#define ITEM_INVERTER_OFF             0   //停止

#define INVERTER_MODE_CHOICE   2   //模式选项数目
#define ITEM_INVERTER          0   //逆变器模式
#define ITEM_POWER             1   //功率模块模式

#define INVERTER_DEADZONE_CHOICE   4  //死区时间选项数目
#define ITEM_0     0   //工作模式
#define ITEM_1     1   //死区时间
#define ITEM_2     2   //无效脉冲隔离
#define ITEM_3     3   //状态

#define INVERTER_INVALID_PLUSE_CHOICE   2  //逆变器模式下设置选项数目
#define ITEM_ON         0   //隔离
#define ITEM_OFF        1   //不隔离

#define INVERTER_WORK_STATE_CHOICE   5  //逆变器模式下设置选项数目
#define ITEM_POWER_ON             0x01   //上电
#define ITEM_STAND_BY             0x02   //待机
#define ITEN_SOFT_START           0x04   //软启动
#define ITEM_NORMAL_WORK          0x08   //正常工作
#define ITEM_FAULT                0x10   //故障

#define INVERTER_FAULT_CHOICE   7  //逆变器模式下设置选项数目
#define ITEM_OVER_VOL           0x01   //过压
#define ITEN_UNDER_VOL          0x02   //欠压
#define ITEM_OVER_FRE           0x04   //过频
#define ITEN_UNDER_FRE          0x08   //欠频
#define ITEM_OVER_CUR           0x10   //过流
#define ITEM_OVER_LOAD          0x20   //过载
#define ITEM_SHORT_CIRCUIT      0x40   //短路
//#define ITEM_ORTHER             0x80   //其他

#define INVERTER_TIME_CHOICE        7   //可以设置的位置数
#define INVERTER_TIME_SELECT_BIT    0    //当前为选位操作
#define INVERTER_TIME_SET_BIT       1    //当前为设置位操作

#define INVERTER_RATIO_PAGE         5    //页数
#define RATIO_DC                    0    //光伏直流
#define RATIO_DC1                   1    //光伏直流
#define RATIO_A                     2    //A相
#define RATIO_B                     3    //B相
#define RATIO_C                     4    //C相

#define INVERTER_RATIO_VIEW         0    //当前为查看
#define INVERTER_RATIO_SELECT_BIT   1    //当前为位选
#define INVERTER_RATIO_SET_BIT      2    //当前为设置位

#define LED_NORMAL    0  //正常运行
#define LED_WARN      1  //警告
#define LED_ERROR     2  //错误


#define POWER_MODE                       199  //功率模块模式标识
#define POWER_DATA_INTERFACE             200  //功率模块下数据显示界面标识
#define POWER_SETTING_INTERFACE          201  //功率模块下设置界面标识
#define POWER_SETTING_MODE               202  //功率模块下模式选择界面标识
#define POWER_SETTING_PRODUCTION         203  //功率模块下死去时间界面标识
#define POWER_SETTING_STATE              204  //功率模块下工作状态界面标识
#define POWER_SETTING_TIME               205  //设置时钟
#define POWER_SETTING_RATIO              206  //修正系数
#define POWER_WARN                       207  //警告

#define POWER_DATA_PAGE    6  //主界面数据显示界面个数
#define DATA_DC_DISPLAY    0   //主界面直流数据显示界面ID
#define DATA_DC1_DISPLAY   1   //主界面直流数据显示界面ID
#define DATA_A_DISPLAY     2   //主界面交流A相数据显示界面ID
#define DATA_B_DISPLAY     3   //主界面交流B相数据显示界面ID
#define DATA_C_DISPLAY     4   //主界面交流C相数据显示界面ID
#define DATA_FRE_DISPLAY   5   //电网频率

#define POWER_SETTING_CHOICE    5   //功率模块模式下设置选项数目
#define OPTION_MODE          0     //模式
#define OPTION_PRODUCTION    1     //发电量
#define OPTION_STATE         2     //状态
#define OPTION_TIME          3   //时钟
#define OPTION_RATIO         4   //修正系数

#define POWER_MODE_CHOICE   2  //模式选项数目
#define OPTION_INVERTER          0   //逆变器模式
#define OPTION_POWER             1   //功率模块模式

#define POWER_STATE_CHOICE   4  //逆变器模式下设置选项数目
#define OPTION_NORMAL             0   //正常
#define OPTION_OVER_VOL           1   //过压
#define OPTION_UNDER_VOL          2   //欠压
#define OPTION_OVER_CUR           3   //过流

#define POWER_TIME_CHOICE        7   //可以设置的位置数
#define POWER_TIME_SELECT_BIT    0    //当前为选位操作
#define POWER_TIME_SET_BIT       1    //当前为设置位操作

#define POWER_RATIO_PAGE            5    //页数
#define RATIO_DC                    0    //光伏直流
#define RATIO_DC1                   1    //光伏直流
#define RATIO_A                     2    //A相
#define RATIO_B                     3    //B相
#define RATIO_C                     4    //C相

#define POWER_RATIO_VIEW         0    //当前为查看
#define POWER_RATIO_SELECT_BIT   1    //当前为位选
#define POWER_RATIO_SET_BIT      2    //当前为设置位


#define OK_DOWN   1  //确认键按下
#define OK_UP     0  //确认键未按下

#define CANCEL_DOWN   1  //取消键按下
#define CANCEL_UP     0  //取消键未按下

#define WARN_YES     1    //确认（是）
#define WARN_NO      0    //取消（否）

#define REFRESH_PRD_FAST    5   //快刷新周期
#define REFRESH_MULTIPLE    5    //慢速频率相对于快速频率的倍数
#define REFRESH_PRD_SLOW    (REFRESH_PRD_FAST*REFRESH_MULTIPLE)   //慢刷新周期
#define PRD_OVERFLOW        (REFRESH_PRD_FAST*(REFRESH_MULTIPLE+2))  //刷新周期计数器溢出
#define RATIO_UP_RANGE     5.000   //修正系数上限
#define RATIO_DOWN_RANGE   0.001   //修正系数下限

#define EEPROM_INIT_FLAG   0  //初始化标记
#define AVERAGE   5    //ADC平均次数

#define FFT_N  128  //fft变换点数

extern u8 inverter_data_page;//保存逆变器模式下数据显示的页码（多页显示）
extern u8 inverter_setting_select;//保存逆变器模式下当前设置选项
extern u8 inverter_on_off_select;//保存选择启动或停止
extern u8 inverter_mode_select;//保存逆变器模式下模式选择选项
extern u8 inverter_deadzone_select;//选择死区时间
extern u8 inverter_invalid_pluse_select;//选择无效脉冲隔离
extern u8 inverter_state_select;//选择工作模式，不可操作，只能查看
extern u8 inverter_fault_state_select;//选择故障状态，不可操作，只能查看
extern u8 inverter_time_select;//选择设置时间位置
extern u8 inverter_ratio_page;//修正系数当前页数

extern u8 inverter_time_mode;//当前设置时间模式标记
extern u8 is_time;//是否设置时间标记,0,不设置;1,设置
extern u8 is_time_finish;//是否完成时间设置

extern u8 inverter_ratio_mode;//当前为位选
extern u8 ratio_which_bit;//当前为哪一位
extern u8 is_ratio_finish;//是否完成系数的设置操作

extern u8 led_state;//用于标示led状态，以便控制其闪烁等状态

extern u8 power_data_page;//保存功率模块模式下数据显示的页码（多页显示）
extern u8 power_setting_select;//保存功率模块模式下当前设置选项
extern u8 power_mode_select;//保存功率模块模式下模式选择选项
extern u8 power_state_select;//选择工作模式，不可操作，只能查看
extern u8 power_time_select;//选择设置时间位置
extern u8 power_ratio_page;//修正系数当前页数

extern u8 power_time_mode;//当前设置时间为选位操作
extern u8 power_ratio_mode;//当前为位选

extern u8 should_clr;//进入某个界面是否要清屏，也就是是不是第一次进入该显示界面，0，不用清屏；1，需要清屏
extern u8 refresh_inverse;//记录是否该刷新反白显示，反之闪烁
extern u16 refresh_prd;//计数周期值

extern u8 ok;//确认键未按下
extern u8 cancel;//取消键未按下
extern u8 warn_select;//警告确认，0，否；1，是

extern u8 display;//保存显示界面标识
extern u8 mode;//逆变器模式
extern u8 setting;//逆变器模式下设置

extern double production;//发电量

extern u8 is_time;//是否设置时间标记,0,不设置;1,设置
extern u8 is_time_finish;//是否完成时间设置
//缓存时间信息
extern u16 year;
extern u8 mon;
extern u8 day;
extern u8 hour;
extern u8 min;
extern u8 sec;

extern float ratio_vol_temp;//缓存电压修正系数
extern float ratio_cur_temp;//缓存电流修正系数

//各个通道修正系数
extern float vol_dc_ratio;
extern float cur_dc_ratio;
extern float vol_dc1_ratio;
extern float cur_dc1_ratio;
extern float vol_a_ratio;
extern float cur_a_ratio;
extern float vol_b_ratio;
extern float cur_b_ratio;
extern float vol_c_ratio;
extern float cur_c_ratio;

extern u8 display_flag;
extern u8 first_time_show;

void Recover_Ratio(void);
void Init_Display(void);
void Display(void);

#endif

