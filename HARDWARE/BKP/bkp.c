#include "bkp.h"
#include "sys.h"
#include "led.h"
#include "user_interface.h"
//后备寄存器存储和恢复

void PWR_ON_Recover(void);

void EXTI_Config(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;

  /* Configure EXTI Line16(PVD Output) to generate an interrupt on rising and
     falling edges */
  EXTI_ClearITPendingBit(EXTI_Line16);
  EXTI_InitStructure.EXTI_Line = EXTI_Line16;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
}

void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
  /* Enable the PVD Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = PVD_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

//初始化后备寄存器
void BKP_Init(void)
{
	  /* Enable PWR and BKP clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

  /* Enable write access to Backup domain */
  PWR_BackupAccessCmd(ENABLE);

  /* Clear Tamper pin Event(TE) pending flag */
  BKP_ClearFlag();

  /* Check if the Power On Reset flag is set */
  if(RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
  {
    /* Clear reset flags */
    RCC_ClearFlag();
  }
	
	/* Configure EXTI Line to generate an interrupt on falling edge */
  EXTI_Config();

  /* NVIC configuration */
  NVIC_Config();
 
  /* Configure the PVD Level to 2.9V */
  PWR_PVDLevelConfig(PWR_PVDLevel_2V9);

  /* Enable the PVD Output */
  PWR_PVDCmd(ENABLE);
	
	if(BKP_ReadBackupRegister(BKP_DR1)==0x5050)		//从指定的后备寄存器中读出数据:读出了与写入的指定数据不相乎
	{	
	  PWR_ON_Recover();
	}
}

//写后备寄存器
void Write_BKP(u16 BKP_DR, u16 DAT)
{
	/* Enable write access to Backup domain */
  PWR_BackupAccessCmd(ENABLE);
	BKP_WriteBackupRegister(BKP_DR,DAT);
}

//读后备寄存器数据
u16 Read_BKP(u16 BKP_DR)
{
	/* Enable write access to Backup domain */
  PWR_BackupAccessCmd(ENABLE);
	return BKP_ReadBackupRegister(BKP_DR);
}

//掉电备份
void PVD_Barkup(void)
{
	u16 ph,pl,dec;
	Write_BKP(BKP_DR3,inverter_deadzone_select);
	Write_BKP(BKP_DR4,inverter_invalid_pluse_select);
	//下面是发电量，用到3个寄存器保存数据,整数部分用2个
	ph=(u32)production/0X10000;
	pl=(u32)production%0X10000;
	dec=(production-(u32)production)*1000;
	Write_BKP(BKP_DR5,ph);
	Write_BKP(BKP_DR6,pl);
	Write_BKP(BKP_DR7,dec);
}

//上电恢复
void PWR_ON_Recover(void)
{
	//if(Read_BKP(BKP_DR3)<INVERTER_DEADZONE_CHOICE)
	//{
	//	inverter_deadzone_select=Read_BKP(BKP_DR3);
	//}
	//if(Read_BKP(BKP_DR4)<INVERTER_INVALID_PLUSE_CHOICE)
	//{
	//	inverter_invalid_pluse_select=Read_BKP(BKP_DR4);
	//}
	production=((u32)Read_BKP(BKP_DR5))*0X10000+Read_BKP(BKP_DR6)+Read_BKP(BKP_DR7)/1000.0;
}

//PVD中断函数，断电保存重要参数
void PVD_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line16); 
  if(PWR_GetFlagStatus(PWR_FLAG_PVDO)) //
  {
		PWR_ClearFlag(PWR_FLAG_PVDO);
		PVD_Barkup();
  }	
}





