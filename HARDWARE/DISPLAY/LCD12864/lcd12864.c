#include "lcd12864.h"
#include "sys.h"
#include "delay.h"
//LCD12864液晶显示屏驱动程序

//只测试了串口通信时的程序
//通过定义宏SERIAL和PARALLEL选择串口或者并口，两个都定义则优先串口方式

u8 rw_flag=WRITE;
#ifdef SERIAL
u8 address_count=0x80;
u8 is_full=0;
u8 is_address_plus=0;//地址是否增加
#endif

void Init_Write(void);
void Init_Read(void);
void InitSeffDefChar(void);
void Init_Inverse(void);

//延时，用于跳过读忙操作
void delay()
{
	delay_us(10);
}

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//写一个字节
//byte  待写字节
void Write_Byte(u8 byte)
{
	u8 i;
	if(rw_flag==READ)
	{
		Init_Write();
	}
	//串口
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
	//并口
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
//读一个字节
//u8 Read_Byte()
//{
//	u8 i;
//	u8 byte=0;
//	if(rw_flag==WRITE)
//	{
//		Init_Read();
//	}
//	//串口
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
//	//并口
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
//写指令
//com  待写指令
void Write_COM(u8 com)
{
	//串口
	#ifdef SERIAL
	Write_Byte(0xf8);
	Write_Byte(com&0xf0);
	Write_Byte((com<<4)&0xf0);
	#endif
	//并口
	#ifdef PARALLEL
	RS=0;
	Write_Byte(com);
	#endif
}

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//写数据
//dat  待写数据
void Write_DAT(u8 dat)
{
	//串口
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
	//并口
	#ifdef PARALLEL
	RS=1;
	Write_Byte(dat);
	#endif
}

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//读忙
//1,忙
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

//读地址计数器
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
//读数据
//u8 Read_DAT(u8 y, u8 x)
//{
//	SetPosition(y,x);
//	Write_Byte(0XFE);
//	return (Read_Byte()&0XF0)|((Read_Byte()>>4)&0X0F);
//}

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//初始化
void Lcd12864_Init(void)
{
	//初始化为写配置
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

//写操作时配置
void Init_Write(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	rw_flag=WRITE;
	//串口
	#ifdef SERIAL
	//使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	#endif
	//并口
	#ifdef PARALLEL
	//使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOF, ENABLE);
	//配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 
	                            | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	#endif
}

//读操作时配置
void Init_Read(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	rw_flag=READ;
	//串口
	#ifdef SERIAL
	//使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	#endif
	//并口
	#ifdef PARALLEL
	//使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOF, ENABLE);
	//配置
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

//设置坐标
//y   0-3，纵向坐标
//x   0-7，横向坐标
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

//写字符
//y   0-3，纵向坐标
//x   0-7，横向坐标
//ch  待写字符
void DispChar(u8 y, u8 x, u8 ch)
{
	SetPosition(y,x);
	Write_DAT(ch);
}

//接着前一次操作的坐标地址后面续写字符
//ch   待续写字符
void DispCharNext(u8 ch)
{
	Write_DAT(ch);
}

//写字符串
//y     0-3，纵坐标
//x     0-7，横坐标
//str   待写字符串，若写到最右边，则会自动换行
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

//接着前一次操作的坐标地址后面续写字符串
//str   待续写字符串，自动换行
void DispStrNext(u8 *str)
{
	u8 i;
	for(i=0;*str;i++)
	{
		Write_DAT(*str);
		str++;
	}
}

//接着前一次操作的坐标地址后面续写显示无符号整数
//figure   待续写整数
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

//显示无符号整数
//y   0-3，纵坐标
//x   0-7，横坐标
//figure   待写整数
void DispFigure(u8 y ,u8 x, u32 figure)
{
	SetPosition(y,x);
	DispFigureNext(figure);
}

//续写小数
//decimal   待续写小数
//decNum    保留小数位数，最多4位
void DispDecimalNext(double decimal, u8 decNum)
{
	u8 pointIndex;//小数点位置
	u8 lenInt=1;//整数长度
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
		dec=4;//不能超过4位小数
	}
	while(decNum)
	{
		K*=10;
		decNum--;
	}
	tempDec=(decimal-tempInt)*K;//dec位小数
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

//显示小数
//y         0-3，纵坐标
//x         0-7，横坐标
//decimal   待写小数
//dec       保留小数位数
void DispDecimal(u8 y ,u8 x, double decimal, u8 dec)
{
	SetPosition(y,x);
	DispDecimalNext(decimal,dec);
}

//显示字符串，不换行，到最右则停止显示后续部分
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

//续写字符串，不换行，到最右则停止显示后续部分
//str  待写字符串
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

//接着上一步操作显示空格，即清空无显示的地方，不换行，留到最后一个单元(一个字节，半字，用于不覆盖标志位)
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

//反白初始化，用于指示选择
void Init_Inverse(void)
{
	u8 i=0;
	u8 j=0;
	is_address_plus=1;//设置标记，反之address_count增加
	Write_COM(0X34);
	for(i=0;i<32;i++)//清空
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
	is_address_plus=0;//清空标记
}

//继续在原来的基础下增加反白的位置，横向以半字为一单元
//y    0-3，纵坐标
//x    0-15，横坐标
//len  1-16，反白长度
void InverseContinue(u8 y, u8 x, u8 len)
{
	u8 i=0;
	u8 j=0;
	y=y&0X03;
	x=x&0X0F;
	len=len%17;
	is_address_plus=1;//设置标记，反之address_count增加
	Write_COM(0X34);
	for(i=0;i<16;i++)
	{
		Write_COM(0X80+(y%2)*16+i);//设置y坐标
		if(y<2)
		{
			Write_COM(0X80+x/2);
		}else{
			Write_COM(0X88+x/2);
		}
		if(x%2)//假如从中间开始
		{
			Write_DAT(0X00);
		}
		for(j=0;j<len;j++)
		{
			Write_DAT(0XFF);
		}
	}
	is_address_plus=0;//清空标记
}

//用于结束InverseContinue()并显示
void FinishInverseContinue(void)
{
	Write_COM(0X36);
	Write_COM(0X30);
}

//反白,只能连续反白一部分，不能分开反白多个部分
//y:0~3, x:0~15, len 1~16
void Inverse(u8 y, u8 x, u8 len)
{
	Init_Inverse();
	InverseContinue(y,x,len);
	FinishInverseContinue();
}

//取消反白
//y:0~3, x:0~15, len 1~16
void Clear_Inverse(u8 y, u8 x, u8 len)
{
	u8 i=0;
	u8 j=0;
	y=y&0X03;
	x=x&0X0F;
	len=len%17;
	is_address_plus=1;//设置标记，反之address_count增加
	Write_COM(0X34);
	for(i=0;i<16;i++)
	{
		Write_COM(0X80+(y%2)*16+i);//设置y坐标
		if(y<2)
		{
			Write_COM(0X80+x/2);
		}else{
			Write_COM(0X88+x/2);
		}
		if(x%2)//假如从中间开始
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
	is_address_plus=0;//清空标记
}

//清屏
void Lcd_Clr(void)
{
	//Init_Inverse();
	Write_COM(0X30);
	delay_us(20);
	Write_COM(0X01);
}






