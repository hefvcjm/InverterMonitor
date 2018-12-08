#include "lcd12864.h"
#include "sys.h"
#include "delay.h"
//LCD12864Һ����ʾ����������

//ֻ�����˴���ͨ��ʱ�ĳ���
//ͨ�������SERIAL��PARALLELѡ�񴮿ڻ��߲��ڣ����������������ȴ��ڷ�ʽ

u8 rw_flag=WRITE;
#ifdef SERIAL
u8 address_count=0x80;
u8 is_full=0;
u8 is_address_plus=0;//��ַ�Ƿ�����
#endif

void Init_Write(void);
void Init_Read(void);
void InitSeffDefChar(void);
void Init_Inverse(void);

//��ʱ������������æ����
void delay()
{
	delay_us(10);
}

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//дһ���ֽ�
//byte  ��д�ֽ�
void Write_Byte(u8 byte)
{
	u8 i;
	if(rw_flag==READ)
	{
		Init_Write();
	}
	//����
	#ifdef SERIAL
	CS=1;
	SCK=0;
	delay();
	for(i=0;i<8;i++)
	{
		SID_W=byte/0X80;
		byte<<=1;
		//delay_ms(1);
		SCK=1;
		delay();
		SCK=0;
	}
	CS=0;
	#endif
	//����
	#ifdef PARALLEL
	u32 set;
	u32 clear;
	set=byte;
	clear=(~(byte|0XFFFFFF00))<<16;
	RW=0;
	GPIOF->BSRR=clear|set;
	E=1;
	delay();
	E=0;
	#endif
}

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//��һ���ֽ�
//u8 Read_Byte()
//{
//	u8 i;
//	u8 byte=0;
//	if(rw_flag==WRITE)
//	{
//		Init_Read();
//	}
//	//����
//	#ifdef SERIAL
//	CS=1;
//	SCK=0;
//	delay();
//	for(i=0;i<8;i++)
//	{
//		byte<<=1;
//		byte|=SID_R;
//		delay();
//		SCK=1;
//		delay();
//	  SCK=0;
//	}
//	CS=0;
//	#endif
//	//����
//	#ifdef PARALLEL
//	RW=1;
//	E=1;
//	delay();
//	byte=GPIO_ReadInputData(GPIOF)&0X00FF;
//	delay();
//	E=0;
//	#endif
//	return byte;
//}

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//дָ��
//com  ��дָ��
void Write_COM(u8 com)
{
	//����
	#ifdef SERIAL
	Write_Byte(0xf8);
	Write_Byte(com&0xf0);
	Write_Byte((com<<4)&0xf0);
	#endif
	//����
	#ifdef PARALLEL
	RS=0;
	Write_Byte(com);
	#endif
}

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//д����
//dat  ��д����
void Write_DAT(u8 dat)
{
	//����
	#ifdef SERIAL
	Write_Byte(0xfa);
	Write_Byte(dat&0xf0);
	Write_Byte((dat<<4)&0xf0);
	if(!is_address_plus)
	{
		is_full++;
	}
	if(is_full==2)
	{
		is_full=0;
		address_count++;
	}
	#endif
	//����
	#ifdef PARALLEL
	RS=1;
	Write_Byte(dat);
	#endif
}

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//��æ
//1,æ
//u8 Read_Busy(void)
//{
//	u8 busy=0;
//	#ifdef SERIAL
//	Write_Byte(0XFC);
//	busy=(Read_Byte()>>7)&0X01;
//	Read_Byte();
//	#endif
//	#ifdef PARALLEL
//	RS=0;
//	busy=(Read_Byte()>>7)&0X01;
//	#endif
//	return busy;
//}

//����ַ������
//u8 Read_Address(void)
//{
//	u8 addr=0;
//	#ifdef SERIAL
//	//Write_Byte(0XFC);
//	//addr=(Read_Byte()&0X70)|((Read_Byte()>>4)&0X0F);
//	addr=address_count;
//	#endif
//	#ifdef PARALLEL
//	RS=0;
//	addr=Read_Byte()&0X7F;
//	#endif
//	return addr;
//}

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//������
//u8 Read_DAT(u8 y, u8 x)
//{
//	SetPosition(y,x);
//	Write_Byte(0XFE);
//	return (Read_Byte()&0XF0)|((Read_Byte()>>4)&0X0F);
//}

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//��ʼ��
void Lcd12864_Init(void)
{
	//��ʼ��Ϊд����
	Init_Write();
	Init_Inverse();
	Write_COM(0x30);
	delay_us(20);
	Write_COM(0x0C);
	Write_COM(0x01);
	delay_us(20);
	Write_COM(0x02);
	Write_COM(0x06);
}

//д����ʱ����
void Init_Write(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	rw_flag=WRITE;
	//����
	#ifdef SERIAL
	//ʹ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	#endif
	//����
	#ifdef PARALLEL
	//ʹ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOF, ENABLE);
	//����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 
	                            | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	#endif
}

//������ʱ����
void Init_Read(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	rw_flag=READ;
	//����
	#ifdef SERIAL
	//ʹ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	#endif
	//����
	#ifdef PARALLEL
	//ʹ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOF, ENABLE);
	//����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 
	                            | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	#endif
}

//��������
//y   0-3����������
//x   0-7����������
void SetPosition(u8 y, u8 x)
{
	y=y&0x3;
	x=x&0x7;
	switch(y)
	{
	  case 0:
		  y=0x80;break;
	  case 1:
		  y=0x90;break;
	  case 2:
		  y=0x88;break;
	  case 3:
		  y=0x98;break;
	  default:
		  break;
	}
	#ifdef SERIAL
	is_full=0;
	address_count=x+y;
	#endif
	Write_COM(x+y);
}

//д�ַ�
//y   0-3����������
//x   0-7����������
//ch  ��д�ַ�
void DispChar(u8 y, u8 x, u8 ch)
{
	SetPosition(y,x);
	Write_DAT(ch);
}

//����ǰһ�β����������ַ������д�ַ�
//ch   ����д�ַ�
void DispCharNext(u8 ch)
{
	Write_DAT(ch);
}

//д�ַ���
//y     0-3��������
//x     0-7��������
//str   ��д�ַ�������д�����ұߣ�����Զ�����
void DispStr(u8 y, u8 x, u8 *str)
{
	u8 flag;
	u8 i;
	SetPosition(y,x);
	switch(y)
	{
	case 0:
		y=0x80;break;
	case 1:
		y=0x90;break;
	case 2:
		y=0x88;break;
	case 3:
		y=0x98;break;
	default:
		break;
	}
	flag=x+y;
    for(i=0;*str;i++)
    {
    	Write_DAT(*str);
    	str++;
    	if(i%2)
    	{
    		flag++;
    		switch (flag)
    		{
    		case 0x90:
    			SetPosition(3,0);flag=0x98;break;
    		case 0x88:
    			SetPosition(1,0);flag=0x90;break;
    		case 0x98:
				  SetPosition(2,0);flag=0x88;break;
    		case 0xa0:
				  SetPosition(0,0);flag=0x80;break;
    		default:
    			break;
    		}
    	}
    }
}

//����ǰһ�β����������ַ������д�ַ���
//str   ����д�ַ������Զ�����
void DispStrNext(u8 *str)
{
	u8 i;
	for(i=0;*str;i++)
	{
		Write_DAT(*str);
		str++;
	}
}

//����ǰһ�β����������ַ������д��ʾ�޷�������
//figure   ����д����
void DispFigureNext(u32 figure)
{
	u8 len=1;
	u8 str[17]={'\0'};
	u32 temp=figure;
	while(temp/10)
	{
		len++;
		temp=temp/10;
	}
	temp=figure;
	while(len)
	{
		len--;
	  str[len]=temp%10+'0';
	  temp=temp/10;
	}
	DispStrNext(str);
}

//��ʾ�޷�������
//y   0-3��������
//x   0-7��������
//figure   ��д����
void DispFigure(u8 y ,u8 x, u32 figure)
{
	SetPosition(y,x);
	DispFigureNext(figure);
}

//��дС��
//decimal   ����дС��
//decNum    ����С��λ�������4λ
void DispDecimalNext(double decimal, u8 decNum)
{
	u8 pointIndex;//С����λ��
	u8 lenInt=1;//��������
	u8 str[17]={'\0'};
	u32 tempInt;
	u16 tempDec;
	u8 dec=decNum;
	u32 K=1;
	if(decimal<0)
	{
		DispCharNext('-');
		decimal=-decimal;
	}
	tempInt=decimal;
	decimal+=0.0000001;
	if(dec>4)
	{
		dec=4;//���ܳ���4λС��
	}
	while(decNum)
	{
		K*=10;
		decNum--;
	}
	tempDec=(decimal-tempInt)*K;//decλС��
	while(tempInt/10)
	{
		lenInt++;
		tempInt=tempInt/10;
	}
	tempInt=decimal;
	if(K!=1)
	{
		str[lenInt]='.';
	}
	pointIndex=lenInt;
	while(lenInt)
	{
		lenInt--;
	  str[lenInt]=tempInt%10+'0';
	  tempInt=tempInt/10;
	}
	while(dec)
	{
		dec--;
	  str[dec+pointIndex+1]=tempDec%10+'0';
	  tempDec=tempDec/10;
	}
	DispStrNext(str);
}

//��ʾС��
//y         0-3��������
//x         0-7��������
//decimal   ��дС��
//dec       ����С��λ��
void DispDecimal(u8 y ,u8 x, double decimal, u8 dec)
{
	SetPosition(y,x);
	DispDecimalNext(decimal,dec);
}

//��ʾ�ַ����������У���������ֹͣ��ʾ��������
void DispStrSingleLine(u8 y, u8 x, u8 *str)
{
	u8 flag;
	u8 i;
	SetPosition(y,x);
	switch(y)
	{
	case 0:
		y=0x80;break;
	case 1:
		y=0x90;break;
	case 2:
		y=0x88;break;
	case 3:
		y=0x98;break;
	default:
		break;
	}
	flag=x+y;
    for(i=0;*str;i++)
    {
    	Write_DAT(*str);
    	str++;
    	if(i%2)
    	{
    		flag++;
				if(flag==0X80||flag==0X88||flag==0X98||flag==0X90)
				{
					break;
				}
    	}
    }
}

//��д�ַ����������У���������ֹͣ��ʾ��������
//str  ��д�ַ���
void DispStrSingleLineNext(u8 *str)
{
	u8 i;
  for(i=0;*str;i++)
  {
    Write_DAT(*str);
    str++;
		if(address_count==0X80||address_count==0X88
			||address_count==0X98||address_count==0X90)
		{
			break;
		}
  }
}

//������һ��������ʾ�ո񣬼��������ʾ�ĵط��������У��������һ����Ԫ(һ���ֽڣ����֣����ڲ����Ǳ�־λ)
void DispBlankRemainLast()
{
	u8 i;
  for(i=0;i<16;i++)
  {
    Write_DAT(' ');
		if(address_count==0X80-1||address_count==0X88-1
			||address_count==0X98-1||address_count==0X90-1)
		{
			Write_DAT(' ');
			break;
		}
  }
}

//���׳�ʼ��������ָʾѡ��
void Init_Inverse(void)
{
	u8 i=0;
	u8 j=0;
	is_address_plus=1;//���ñ�ǣ���֮address_count����
	Write_COM(0X34);
	for(i=0;i<32;i++)//���
	{
		Write_COM(0X80+i);
		for(j=0;j<16;j++)
		{
			Write_DAT(0X00);
			Write_DAT(0X00);
		}
	}
	Write_COM(0X36);
	Write_COM(0X30);
	is_address_plus=0;//��ձ��
}

//������ԭ���Ļ��������ӷ��׵�λ�ã������԰���Ϊһ��Ԫ
//y    0-3��������
//x    0-15��������
//len  1-16�����׳���
void InverseContinue(u8 y, u8 x, u8 len)
{
	u8 i=0;
	u8 j=0;
	y=y&0X03;
	x=x&0X0F;
	len=len%17;
	is_address_plus=1;//���ñ�ǣ���֮address_count����
	Write_COM(0X34);
	for(i=0;i<16;i++)
	{
		Write_COM(0X80+(y%2)*16+i);//����y����
		if(y<2)
		{
			Write_COM(0X80+x/2);
		}else{
			Write_COM(0X88+x/2);
		}
		if(x%2)//������м俪ʼ
		{
			Write_DAT(0X00);
		}
		for(j=0;j<len;j++)
		{
			Write_DAT(0XFF);
		}
	}
	is_address_plus=0;//��ձ��
}

//���ڽ���InverseContinue()����ʾ
void FinishInverseContinue(void)
{
	Write_COM(0X36);
	Write_COM(0X30);
}

//����,ֻ����������һ���֣����ֿܷ����׶������
//y:0~3, x:0~15, len 1~16
void Inverse(u8 y, u8 x, u8 len)
{
	Init_Inverse();
	InverseContinue(y,x,len);
	FinishInverseContinue();
}

//ȡ������
//y:0~3, x:0~15, len 1~16
void Clear_Inverse(u8 y, u8 x, u8 len)
{
	u8 i=0;
	u8 j=0;
	y=y&0X03;
	x=x&0X0F;
	len=len%17;
	is_address_plus=1;//���ñ�ǣ���֮address_count����
	Write_COM(0X34);
	for(i=0;i<16;i++)
	{
		Write_COM(0X80+(y%2)*16+i);//����y����
		if(y<2)
		{
			Write_COM(0X80+x/2);
		}else{
			Write_COM(0X88+x/2);
		}
		if(x%2)//������м俪ʼ
		{
			Write_DAT(0X00);
		}
		for(j=0;j<len;j++)
		{
			Write_DAT(0X00);
		}
	}
	Write_COM(0X36);
	Write_COM(0X30);
	is_address_plus=0;//��ձ��
}

//����
void Lcd_Clr(void)
{
	//Init_Inverse();
	Write_COM(0X30);
	delay_us(20);
	Write_COM(0X01);
}






