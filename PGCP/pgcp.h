//ͨ��Э��ͷ�ļ�
#ifndef _PGCP_H
#define _PGCP_H
#include "sys.h"

typedef struct
{
	u8 head[2];//��ʼ����0XAA55
	u8 addr[2];//��ַ��addr[0]Ŀ�Ľڵ㣬addr[1]Դ�ڵ�
	u8 com;//������
	//������
	u8 len;//���ݳ���
	u8 data[255];//����
	//
	u8 check;//У���룬��ַ���������Լ��������ȫ���ֽ��޷��ź͵ĵ�λ�ֽ�
	u8 end[2];//��������0XCC33
	
}_comm_pro;

extern _comm_pro protocol;

#define INFO_DATA_LEN   44
extern u8 info_data[INFO_DATA_LEN];//��صĵ�����ѹ����Ϣ����

#define MIN_PACKAGE_LEN   9    //����֡(��)��С����
#define MAX_PACKAGE_LEN   264  //����֡(��)��С����

//���������ַ��
#define HEAD               0XAA55   //��ʼ��
#define END                0XCC33   //������
#define ADDR_MASTER        0X00     //���������ַ
#define ADDR_DEVICE        0X10     //���װ�õ�ַ
#define ADDR_BOX           0X40     //�ɼ��е�ַ
#define ADDR_DETECTION     0X50     //�����������������ַ
#define ADDR_CONVERTER_1   0X80     //�����1��ַ
#define ADDR_CONVERTER_2   0X81     //�����2��ַ
#define ADDR_CONVERTER_3   0X82     //�����3��ַ

//��������������ͨ������
#define COM_GET_INFO           0XF0   //��ȡ��������������Ϣ

//��������벢�������������ͨ������
#define COM_RESET                  0X90   //��⸴λ
#define COM_POWER_ADJUST_START     0X91   //�����������ʵ���
#define COM_POWER_ADJUST_FINISH    0X92   //���������������ʵ���
#define COM_ISLAND_PREPARE         0X93   //�µ���������׼��
#define COM_ISLAND_TEST            0X94   //�µ���������


//������
#define ERR_CHECK      0XF1   //У���
#define ERR_DEVICE     0XF2   //װ�ô���
//����բ����
#define ERR_SWITCH_3_OFF      0X44    //բ��3��բ����
#define ERR_SWITCH_2_OFF      0X22    //բ��2��բ����
#define ERR_SWITCH_1_OFF      0X11    //բ��1��բ
#define ERR_SWITCH_3_ON       0X04    //բ��1��բ����
#define ERR_SWITCH_2_ON       0X02    //բ��1��բ����
#define ERR_SWITCH_1_ON       0X01    //բ��1��բ����

void Set_Head(void);
void Set_End(void);
void Set_Addr(u8 dst, u8 src);
void Set_Com(u8 com);
void Set_Data(u8 len, u8 *data);
void Set_Check(void);
u8 Check(_comm_pro protocol);
void Package(_comm_pro protocol, u8 *package, u16 *len);
void Unpack(_comm_pro *protocol, u8 *package);

#endif
