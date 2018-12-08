#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"  

extern u16 timer_count;//ÖÐ¶Ï´ÎÊý
extern u16 time_out;
extern u8 state[2];

void TIM2_Int_Init(u16 arr,u16 psc);
void TIM3_Int_Init(u16 arr,u16 psc);
void TIM4_Int_Init(u16 arr,u16 psc);
 
#endif
