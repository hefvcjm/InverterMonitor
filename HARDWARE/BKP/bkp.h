#ifndef _BKP_H
#define _BKP_H
#include "sys.h"

void BKP_Init(void);
void Write_BKP(u16 BKP_DR, u16 DAT);
u16 Read_BKP(u16 BKP_DR);



#endif




