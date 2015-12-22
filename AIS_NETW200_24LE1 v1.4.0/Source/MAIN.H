#ifndef _MAIN_H_
#define _MAIN_H_  

#include "mytypedef.h"
#include "wdt.h"
#include "adc.h"	
#include "led.h"
#include "random.h"
#include "rtc.h"

#define CHARGEGATE 1700 //1.4v
//#define SHUTDOWN_NUM 
#define START_NUM 2


void delay(unsigned int dj);
extern INT8U interval_s;
extern INT8U boatnum[16];
extern volatile INT16U timinteval;
extern INT8U timinteval_1;
extern INT8U timinteval_2;
extern INT8U txbuf[30];
extern INT8U rxbuf[30];
extern INT8U codeflag;
extern volatile INT8U taskstart;

#endif
