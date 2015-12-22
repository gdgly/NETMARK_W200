#ifndef __USART3_H
#define	__USART3_H

#include "stm32f10x.h"
#include <stdio.h>
#include "misc.h"

void USART3_NVIC_Configuration(void);
void USART3_Config(void);
void sendback(void);
void Usart3GetCommand(void);

#endif /* __USART3_H */
