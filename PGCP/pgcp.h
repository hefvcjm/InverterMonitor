//通信协议头文件
#ifndef _PGCP_H
#define _PGCP_H
#include "sys.h"

typedef struct
{
	u8 head[2];//起始符，0XAA55
	u8 addr[2];//地址域，addr[0]目的节点，addr[1]源节点
	u8 com;//命令域
	//数据域
	u8 len;//数据长度
	u8 data[255];//数据
	//
	u8 check;//校验码，地址域、命令域以及数据域的全部字节无符号和的低位字节
	u8 end[2];//结束符，0XCC33
	
}_comm_pro;

extern _comm_pro protocol;

#define INFO_DATA_LEN   44
extern u8 info_data[INFO_DATA_LEN];//相关的电流电压等信息数据

#define MIN_PACKAGE_LEN   9    //数据帧(包)最小长度
#define MAX_PACKAGE_LEN   264  //数据帧(包)最小长度

//定义命令、地址等
#define HEAD               0XAA55   //起始符
#define END                0XCC33   //结束符
#define ADDR_MASTER        0X00     //监控主机地址
#define ADDR_DEVICE        0X10     //监控装置地址
#define ADDR_BOX           0X40     //采集盒地址
#define ADDR_DETECTION     0X50     //并网检测屏控制器地址
#define ADDR_CONVERTER_1   0X80     //逆变器1地址
#define ADDR_CONVERTER_2   0X81     //逆变器2地址
#define ADDR_CONVERTER_3   0X82     //逆变器3地址

//监控主机与逆变器通信命令
#define COM_GET_INFO           0XF0   //获取逆变器相关数据信息

//监控主机与并网检测屏控制器通信命令
#define COM_RESET                  0X90   //检测复位
#define COM_POWER_ADJUST_START     0X91   //逆变器输出功率调整
#define COM_POWER_ADJUST_FINISH    0X92   //结束逆变器输出功率调整
#define COM_ISLAND_PREPARE         0X93   //孤单保护测试准备
#define COM_ISLAND_TEST            0X94   //孤岛保护测试


//错误码
#define ERR_CHECK      0XF1   //校验错
#define ERR_DEVICE     0XF2   //装置错误
//跳合闸出错
#define ERR_SWITCH_3_OFF      0X44    //闸刀3跳闸出错
#define ERR_SWITCH_2_OFF      0X22    //闸刀2跳闸出错
#define ERR_SWITCH_1_OFF      0X11    //闸刀1跳闸
#define ERR_SWITCH_3_ON       0X04    //闸刀1合闸出错
#define ERR_SWITCH_2_ON       0X02    //闸刀1合闸出错
#define ERR_SWITCH_1_ON       0X01    //闸刀1合闸出错

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
