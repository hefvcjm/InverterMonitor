#ifndef _LCD12864_H
#define _LCD12864_H
#include "sys.h"

//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
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

//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

#define SERIAL     //串口
#define PARALLEL   //并口

//优先使用串口
//3根线分别使用PB 0,1,2
#ifdef SERIAL
#undef PARALLEL    //取消定义并口
#endif

//如果是使用串口
#ifdef SERIAL
//CS
#define CS      PBout(0)
//SID
#define SID_W   PBout(1)
#define SID_R   PBin(1)
//SCK
#define SCK     PBout(2)
#endif

//如果是使用并口
//3根控制线分别使用PB 0,1,2
//8根并口数据线使用PF0~PF7
#ifdef  PARALLEL
//RS
#define RS   PBout(0)
//RW
#define RW   PBout(1)
//SCK
#define E    PBout(2)
#endif

#define WRITE  0  //当前为写配置
#define READ   1  //当前为读配置

extern u8 zm_dong[];
extern u8 zm_feng[];
extern u8 zm_po[];
extern u8 zm_heart[];
extern u8 is_address_plus;//地址是否增加

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

