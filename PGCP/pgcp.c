#include "pgcp.h"
#include "rs232.h"

_comm_pro protocol;
u8 info_data[INFO_DATA_LEN];

//设置起始符，需要固定为0XAA55
void Set_Head(void)
{
	protocol.head[0]=0XAA;
	protocol.head[1]=0X55;
}

//设置结束符，需要固定为0XCC33
void Set_End(void)
{
	protocol.end[0]=0XCC;
	protocol.end[1]=0X33;
}

//设置地址域
//dst，目的节点号；src，源节点
void Set_Addr(u8 dst, u8 src)
{
	protocol.addr[0]=dst;
	protocol.addr[1]=src;
}

//设置命令
void Set_Com(u8 com)
{
	protocol.com=com;
}

//设置数据长度
void Set_Len(u8 len)
{
	protocol.len=len;
}

//设置数据
void Set_Data(u8 len, u8 *data)
{
	u8 i=0;
	Set_Len(len);
	for(i=0;i<protocol.len;i++)
	{
		protocol.data[i]=data[i];
	}
}

//设置校验和
void Set_Check(void)
{
	u32 temp;
	u8 i=0;
	temp=protocol.addr[0]+protocol.addr[1]+protocol.com+protocol.len;
	for(i=0;i<protocol.len;i++)
	{
		temp+=protocol.data[i];
	}
	protocol.check=temp%0X100;
}

//校验
u8 Check(_comm_pro protocol)
{
	u32 temp;
	u8 i=0;
	temp=protocol.addr[0]+protocol.addr[1]+protocol.com+protocol.len;
	for(i=0;i<protocol.len;i++)
	{
		temp+=protocol.data[i];
	}
	if(protocol.check==(temp%0X100))
	{
		return 1;
	}
	return 0;
}

//打包数据
//protocol 需要打包的内容
//package  存放打包后的数据数组
//len      存放数据包长度(byte)
void Package(_comm_pro protocol, u8 *package, u16 *len)
{
	u8 i=0;
	package[0]=protocol.head[0];
	package[1]=protocol.head[1];
	package[2]=protocol.addr[0];
	package[3]=protocol.addr[1];
	package[4]=protocol.com;
	package[5]=protocol.len;
	for(i=0;i<protocol.len;i++)
	{
		package[6+i]=protocol.data[i];
	}
	package[protocol.len+6]=protocol.check;
	package[protocol.len+7]=protocol.end[0];
	package[protocol.len+8]=protocol.end[1];
	*len=protocol.len+9;
}

//解包
//protocol  存放解包内容
//package   数据包
void Unpack(_comm_pro *protocol, u8 *package)
{
	u8 i=0;
	protocol->head[0]=package[0];
	protocol->head[1]=package[1];
	protocol->addr[0]=package[2];
	protocol->addr[1]=package[3];
	protocol->com=package[4];
	protocol->len=package[5];
	for(i=0;i<protocol->len;i++)
	{
		protocol->data[i]=package[6+i];
	}
	protocol->check=package[protocol->len+6];
	protocol->end[0]=package[protocol->len+7];
	protocol->end[1]=package[protocol->len+8];
}




