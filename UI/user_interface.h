#ifndef _USER_INTERFACE_H
#define _USER_INTERFACE_H
#include "sys.h"

#define INVERTER_MODE                    99   //�����ģʽ��ʶ
#define INVERTER_DATA_INTERFACE          100  //�����ģʽ��������ʾ�����ʶ
#define INVERTER_SETTING_INTERFACE       101  //�����ģʽ�����ý����ʶ
#define INVERTER_SETTING_ON_OFF          102  //������ֹͣ
#define INVERTER_SETTING_MODE            103  //�����ģʽ��ģʽѡ������ʶ
#define INVERTER_SETTING_DEADZONE        104  //�����ģʽ����ȥʱ������ʶ
#define INVERTER_SETTING_INVALID_PLUSE   105  //�����ģʽ����Ч�����������ʶ
#define INVERTER_SETTING_STATE           106  //�����ģʽ�¹���״̬�����ʶ
#define INVERTER_FAULT_STATE             107  //�����ģʽ�¹��ϲ鿴
#define INVERTER_SETTING_TIME            108  //����ʱ��
#define INVERTER_SETTING_RATIO           109  //����ϵ��
#define INVERTER_WARN                    110  //����

#define INVERTER_DATA_PAGE   6  //������������ʾ�������
#define DATA_DC_DISPLAY    0   //������ֱ��������ʾ����ID
#define DATA_DC1_DISPLAY   1   //������ֱ��������ʾ����ID
#define DATA_A_DISPLAY     2   //�����潻��A��������ʾ����ID
#define DATA_B_DISPLAY     3   //�����潻��B��������ʾ����ID
#define DATA_C_DISPLAY     4   //�����潻��C��������ʾ����ID
#define DATA_FRE_DISPLAY   5   //����Ƶ��

#define INVERTER_SETTING_CHOICE   7  //�����ģʽ������ѡ����Ŀ
#define ITEM_ON_OFF               0   //������ֹͣ
#define ITEM_MODE                 1   //����ģʽ
#define ITEM_DEADZONE             2   //����ʱ��
#define ITEN_INVALID_PLUSE        3   //��Ч�������
#define ITEM_STATE                4   //״̬
#define ITEM_TIME                 5   //ʱ��
#define ITEM_RATIO                6   //����ϵ��

#define INVERTER_ON_OFF_CHOICE   2  
#define ITEM_INVERTER_ON              1   //����
#define ITEM_INVERTER_OFF             0   //ֹͣ

#define INVERTER_MODE_CHOICE   2   //ģʽѡ����Ŀ
#define ITEM_INVERTER          0   //�����ģʽ
#define ITEM_POWER             1   //����ģ��ģʽ

#define INVERTER_DEADZONE_CHOICE   4  //����ʱ��ѡ����Ŀ
#define ITEM_0     0   //����ģʽ
#define ITEM_1     1   //����ʱ��
#define ITEM_2     2   //��Ч�������
#define ITEM_3     3   //״̬

#define INVERTER_INVALID_PLUSE_CHOICE   2  //�����ģʽ������ѡ����Ŀ
#define ITEM_ON         0   //����
#define ITEM_OFF        1   //������

#define INVERTER_WORK_STATE_CHOICE   5  //�����ģʽ������ѡ����Ŀ
#define ITEM_POWER_ON             0x01   //�ϵ�
#define ITEM_STAND_BY             0x02   //����
#define ITEN_SOFT_START           0x04   //������
#define ITEM_NORMAL_WORK          0x08   //��������
#define ITEM_FAULT                0x10   //����

#define INVERTER_FAULT_CHOICE   7  //�����ģʽ������ѡ����Ŀ
#define ITEM_OVER_VOL           0x01   //��ѹ
#define ITEN_UNDER_VOL          0x02   //Ƿѹ
#define ITEM_OVER_FRE           0x04   //��Ƶ
#define ITEN_UNDER_FRE          0x08   //ǷƵ
#define ITEM_OVER_CUR           0x10   //����
#define ITEM_OVER_LOAD          0x20   //����
#define ITEM_SHORT_CIRCUIT      0x40   //��·
//#define ITEM_ORTHER             0x80   //����

#define INVERTER_TIME_CHOICE        7   //�������õ�λ����
#define INVERTER_TIME_SELECT_BIT    0    //��ǰΪѡλ����
#define INVERTER_TIME_SET_BIT       1    //��ǰΪ����λ����

#define INVERTER_RATIO_PAGE         5    //ҳ��
#define RATIO_DC                    0    //���ֱ��
#define RATIO_DC1                   1    //���ֱ��
#define RATIO_A                     2    //A��
#define RATIO_B                     3    //B��
#define RATIO_C                     4    //C��

#define INVERTER_RATIO_VIEW         0    //��ǰΪ�鿴
#define INVERTER_RATIO_SELECT_BIT   1    //��ǰΪλѡ
#define INVERTER_RATIO_SET_BIT      2    //��ǰΪ����λ

#define LED_NORMAL    0  //��������
#define LED_WARN      1  //����
#define LED_ERROR     2  //����


#define POWER_MODE                       199  //����ģ��ģʽ��ʶ
#define POWER_DATA_INTERFACE             200  //����ģ����������ʾ�����ʶ
#define POWER_SETTING_INTERFACE          201  //����ģ�������ý����ʶ
#define POWER_SETTING_MODE               202  //����ģ����ģʽѡ������ʶ
#define POWER_SETTING_PRODUCTION         203  //����ģ������ȥʱ������ʶ
#define POWER_SETTING_STATE              204  //����ģ���¹���״̬�����ʶ
#define POWER_SETTING_TIME               205  //����ʱ��
#define POWER_SETTING_RATIO              206  //����ϵ��
#define POWER_WARN                       207  //����

#define POWER_DATA_PAGE    6  //������������ʾ�������
#define DATA_DC_DISPLAY    0   //������ֱ��������ʾ����ID
#define DATA_DC1_DISPLAY   1   //������ֱ��������ʾ����ID
#define DATA_A_DISPLAY     2   //�����潻��A��������ʾ����ID
#define DATA_B_DISPLAY     3   //�����潻��B��������ʾ����ID
#define DATA_C_DISPLAY     4   //�����潻��C��������ʾ����ID
#define DATA_FRE_DISPLAY   5   //����Ƶ��

#define POWER_SETTING_CHOICE    5   //����ģ��ģʽ������ѡ����Ŀ
#define OPTION_MODE          0     //ģʽ
#define OPTION_PRODUCTION    1     //������
#define OPTION_STATE         2     //״̬
#define OPTION_TIME          3   //ʱ��
#define OPTION_RATIO         4   //����ϵ��

#define POWER_MODE_CHOICE   2  //ģʽѡ����Ŀ
#define OPTION_INVERTER          0   //�����ģʽ
#define OPTION_POWER             1   //����ģ��ģʽ

#define POWER_STATE_CHOICE   4  //�����ģʽ������ѡ����Ŀ
#define OPTION_NORMAL             0   //����
#define OPTION_OVER_VOL           1   //��ѹ
#define OPTION_UNDER_VOL          2   //Ƿѹ
#define OPTION_OVER_CUR           3   //����

#define POWER_TIME_CHOICE        7   //�������õ�λ����
#define POWER_TIME_SELECT_BIT    0    //��ǰΪѡλ����
#define POWER_TIME_SET_BIT       1    //��ǰΪ����λ����

#define POWER_RATIO_PAGE            5    //ҳ��
#define RATIO_DC                    0    //���ֱ��
#define RATIO_DC1                   1    //���ֱ��
#define RATIO_A                     2    //A��
#define RATIO_B                     3    //B��
#define RATIO_C                     4    //C��

#define POWER_RATIO_VIEW         0    //��ǰΪ�鿴
#define POWER_RATIO_SELECT_BIT   1    //��ǰΪλѡ
#define POWER_RATIO_SET_BIT      2    //��ǰΪ����λ


#define OK_DOWN   1  //ȷ�ϼ�����
#define OK_UP     0  //ȷ�ϼ�δ����

#define CANCEL_DOWN   1  //ȡ��������
#define CANCEL_UP     0  //ȡ����δ����

#define WARN_YES     1    //ȷ�ϣ��ǣ�
#define WARN_NO      0    //ȡ������

#define REFRESH_PRD_FAST    5   //��ˢ������
#define REFRESH_MULTIPLE    5    //����Ƶ������ڿ���Ƶ�ʵı���
#define REFRESH_PRD_SLOW    (REFRESH_PRD_FAST*REFRESH_MULTIPLE)   //��ˢ������
#define PRD_OVERFLOW        (REFRESH_PRD_FAST*(REFRESH_MULTIPLE+2))  //ˢ�����ڼ��������
#define RATIO_UP_RANGE     5.000   //����ϵ������
#define RATIO_DOWN_RANGE   0.001   //����ϵ������

#define EEPROM_INIT_FLAG   0  //��ʼ�����
#define AVERAGE   5    //ADCƽ������

#define FFT_N  128  //fft�任����

extern u8 inverter_data_page;//���������ģʽ��������ʾ��ҳ�루��ҳ��ʾ��
extern u8 inverter_setting_select;//���������ģʽ�µ�ǰ����ѡ��
extern u8 inverter_on_off_select;//����ѡ��������ֹͣ
extern u8 inverter_mode_select;//���������ģʽ��ģʽѡ��ѡ��
extern u8 inverter_deadzone_select;//ѡ������ʱ��
extern u8 inverter_invalid_pluse_select;//ѡ����Ч�������
extern u8 inverter_state_select;//ѡ����ģʽ�����ɲ�����ֻ�ܲ鿴
extern u8 inverter_fault_state_select;//ѡ�����״̬�����ɲ�����ֻ�ܲ鿴
extern u8 inverter_time_select;//ѡ������ʱ��λ��
extern u8 inverter_ratio_page;//����ϵ����ǰҳ��

extern u8 inverter_time_mode;//��ǰ����ʱ��ģʽ���
extern u8 is_time;//�Ƿ�����ʱ����,0,������;1,����
extern u8 is_time_finish;//�Ƿ����ʱ������

extern u8 inverter_ratio_mode;//��ǰΪλѡ
extern u8 ratio_which_bit;//��ǰΪ��һλ
extern u8 is_ratio_finish;//�Ƿ����ϵ�������ò���

extern u8 led_state;//���ڱ�ʾled״̬���Ա��������˸��״̬

extern u8 power_data_page;//���湦��ģ��ģʽ��������ʾ��ҳ�루��ҳ��ʾ��
extern u8 power_setting_select;//���湦��ģ��ģʽ�µ�ǰ����ѡ��
extern u8 power_mode_select;//���湦��ģ��ģʽ��ģʽѡ��ѡ��
extern u8 power_state_select;//ѡ����ģʽ�����ɲ�����ֻ�ܲ鿴
extern u8 power_time_select;//ѡ������ʱ��λ��
extern u8 power_ratio_page;//����ϵ����ǰҳ��

extern u8 power_time_mode;//��ǰ����ʱ��Ϊѡλ����
extern u8 power_ratio_mode;//��ǰΪλѡ

extern u8 should_clr;//����ĳ�������Ƿ�Ҫ������Ҳ�����ǲ��ǵ�һ�ν������ʾ���棬0������������1����Ҫ����
extern u8 refresh_inverse;//��¼�Ƿ��ˢ�·�����ʾ����֮��˸
extern u16 refresh_prd;//��������ֵ

extern u8 ok;//ȷ�ϼ�δ����
extern u8 cancel;//ȡ����δ����
extern u8 warn_select;//����ȷ�ϣ�0����1����

extern u8 display;//������ʾ�����ʶ
extern u8 mode;//�����ģʽ
extern u8 setting;//�����ģʽ������

extern double production;//������

extern u8 is_time;//�Ƿ�����ʱ����,0,������;1,����
extern u8 is_time_finish;//�Ƿ����ʱ������
//����ʱ����Ϣ
extern u16 year;
extern u8 mon;
extern u8 day;
extern u8 hour;
extern u8 min;
extern u8 sec;

extern float ratio_vol_temp;//�����ѹ����ϵ��
extern float ratio_cur_temp;//�����������ϵ��

//����ͨ������ϵ��
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

