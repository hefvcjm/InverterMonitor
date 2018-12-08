#ifndef _LCD12864_H
#define _LCD12864_H
#include "sys.h"

//IO�ڲ����궨��
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO�ڵ�ַӳ��
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 

//IO�ڲ���,ֻ�Ե�һ��IO��!
//ȷ��n��ֵС��16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //��� 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //���� 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //��� 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //���� 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //��� 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //���� 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //��� 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //���� 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //��� 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //����

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //��� 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //����

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //��� 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //����

#define SERIAL     //����
#define PARALLEL   //����

//����ʹ�ô���
//3���߷ֱ�ʹ��PB 0,1,2
#ifdef SERIAL
#undef PARALLEL    //ȡ�����岢��
#endif

//�����ʹ�ô���
#ifdef SERIAL
//CS
#define CS      PBout(0)
//SID
#define SID_W   PBout(1)
#define SID_R   PBin(1)
//SCK
#define SCK     PBout(2)
#endif

//�����ʹ�ò���
//3�������߷ֱ�ʹ��PB 0,1,2
//8������������ʹ��PF0~PF7
#ifdef  PARALLEL
//RS
#define RS   PBout(0)
//RW
#define RW   PBout(1)
//SCK
#define E    PBout(2)
#endif

#define WRITE  0  //��ǰΪд����
#define READ   1  //��ǰΪ������

extern u8 zm_dong[];
extern u8 zm_feng[];
extern u8 zm_po[];
extern u8 zm_heart[];
extern u8 is_address_plus;//��ַ�Ƿ�����

void Write_COM(u8 com);
void Write_DAT(u8 dat);
//u8 Read_Busy(void);
//u8 Read_Address(void);
//u8 Read_DAT(u8 y, u8 x);
void Lcd12864_Init(void);
void SetPosition(u8 y, u8 x);
void DispChar(u8 y, u8 x, u8 ch);
void DispCharNext(u8 ch);
void DispStr(u8 y, u8 x, u8 *str);
void DispStrNext(u8 *str);
void DispFigureNext(u32 figure);
void DispFigure(u8 y ,u8 x, u32 figure);
void DispDecimalNext(double decimal, u8 dec);
void DispDecimal(u8 y ,u8 x, double decimal, u8 dec);
void DispStrSingleLine(u8 y, u8 x, u8 *str);
void DispStrSingleLineNext(u8 *str);
void DispBlankRemainLast(void);
void Init_Inverse(void);
void InverseContinue(u8 y, u8 x, u8 len);
void FinishInverseContinue(void);
void Inverse(u8 y, u8 x, u8 len);
void Clear_Inverse(u8 y, u8 x, u8 len);
void Lcd_Clr(void);


#endif

