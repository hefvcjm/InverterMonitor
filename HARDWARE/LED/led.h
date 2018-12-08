#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
 
#define LED1 PEout(4)// PE4  LED1
#define LED2 PEout(5)// PE5	 LED2
#define LED3 PEout(6)// PE6	 LED3

#define POWER  LED1
#define WARN   LED2
#define ERROR  LED3

void LED_Init(void);//≥ı ºªØ

		 				    
#endif
