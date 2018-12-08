#include "communication.h"
#include "sys.h"
#include "pgcp.h"
#include "rs232.h"
#include "user_interface.h"
#include "led.h"
#include "delay.h"

#define CONVERTER_3

u8 package[MAX_PACKAGE_LEN];
u16 package_length;

//ִ�в���
void Execute(void)
{
	u8 err[1];
	#ifdef CONVERTER_3
	//���Ŀ�ĵ�ַ��������������3������
	if(protocol.addr[0]!=ADDR_CONVERTER_3)
	{
		//RS232_Send_Data(protocol.addr,2);
		//RS232_Send_Data("\r",1);
		//RS232_Send_Data(package,package_length);
		//RS232_Send_Data("\r",1);
		//RS232_Send_Data("no 3\r",5);
		return;
	}
	//RS232_Send_Data("3\r",1);
	if(protocol.com==COM_GET_INFO)
	{
	  //RS232_Send_Data("info\r",5);
	  if(Check(protocol))//У��
	  {
		  //RS232_Send_Data("check\r",6);
		  Set_Addr(protocol.addr[1],protocol.addr[0]);//����Ŀ�ĵ�ַ��Դ��ַ
		  Set_Data(INFO_DATA_LEN,info_data);
			Set_Check();
		  Package(protocol,package,&package_length);
		  RS232_Send_Data(package,package_length);
	  }else{
		  err[0]=ERR_CHECK;
		  Set_Addr(protocol.addr[1],protocol.addr[0]);//����Ŀ�ĵ�ַ��Դ��ַ
		  Set_Data(0X01,err);//У���
			Set_Check();
		  Package(protocol,package,&package_length);
		  RS232_Send_Data(package,package_length);
	  }
	}
	#endif
	#ifdef DETECTION
	//���Ŀ�ĵ�ַ
	if(protocol.addr[0]!=ADDR_DETECTION)
	{
		//RS232_Send_Data(protocol.addr,2);
		//RS232_Send_Data("\r",1);
		//RS232_Send_Data(package,package_length);
		//RS232_Send_Data("\r",1);
		//RS232_Send_Data("no 3\r",5);
		return;
	}
	if(Check(protocol))//У��
	{
		switch(protocol.com)
	  {
		  case COM_RESET:
		  	
			  break;
		  case COM_POWER_ADJUST_START:
		  	
			  break;
		  case COM_POWER_ADJUST_FINISH:
			  
			  break;
		  case COM_ISLAND_PREPARE:
			  
			  break;
		  case COM_ISLAND_TEST:
			  
			  break;
		  default:
			  break;
	  }
		err[0]=0;//�ɹ�
		Set_Addr(protocol.addr[1],protocol.addr[0]);//����Ŀ�ĵ�ַ��Դ��ַ
		Set_Data(0X01,err);
		Set_Check();
		Package(protocol,package,&package_length);
		RS232_Send_Data(package,package_length);
	}else{
		err[0]=ERR_CHECK;
		Set_Addr(protocol.addr[1],protocol.addr[0]);//����Ŀ�ĵ�ַ��Դ��ַ
		Set_Data(0X01,err);//У���
		Set_Check();
		Package(protocol,package,&package_length);
		RS232_Send_Data(package,package_length);
	}
	#endif
}

//����
void Analysis(void)
{
	u16 i=0;
	u8 flag=0;
	u16 sartIndex=0;
	if(RS232_Receive_Data(package,&package_length))//�Ƿ���յ�����
	{
		//RS232_Send_Data("receive data\r",13);
		//RS232_Send_Data(package,package_length);
		//RS232_Send_Data("\r",1);
		if(package_length>=MIN_PACKAGE_LEN)//���ݳ����Ƿ�Ϸ�����С�����������
		{
			for(i=0;i<package_length-1;i++)
			{
				if(((u16)package[i]*0X100+package[i+1])!=0XAA55 && !flag)
				{
					continue;
				}else{
					flag=1;
					sartIndex=i;
					break;
				}
			}
			if(sartIndex!=0)//ȥ��ǰ�����Ч����
			{
				package_length=package_length-sartIndex;
				for(i=0;i<package_length;i++)
				{
					package[i]=package[i+sartIndex];
				}
			}
			if(package_length>=MIN_PACKAGE_LEN)//���ݳ����Ƿ�Ϸ�����С�����������
			{
				if(((u16)package[0]*0X100+package[1])==0XAA55)//������ݰ�ͷ
				{
					//RS232_Send_Data("head\r",5);
					if(((u16)package[package_length-2]*0X100+package[package_length-1])==0XCC33)//������ݰ�β
					{
						//RS232_Send_Data("end\r",4);
						Unpack(&protocol, package);//���
						Execute();//ִ����ز���
					}
				}
		  }
		}
	}
}







