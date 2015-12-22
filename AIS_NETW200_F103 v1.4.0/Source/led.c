#include "main.h"

void LedDelay()
{
			GPIO_SetBits(GPIOC, GPIO_Pin_13); //LED
			Delay(30000);Delay(30000);Delay(30000);Delay(30000);Delay(30000);Delay(30000);Delay(30000);Delay(30000);
			Delay(30000);Delay(30000);Delay(30000);Delay(30000);Delay(30000);Delay(30000);Delay(30000);Delay(30000);
			Delay(30000);Delay(30000);Delay(30000);Delay(30000);Delay(30000);Delay(30000);Delay(30000);Delay(30000);
			Delay(30000);Delay(30000);Delay(30000);Delay(30000);Delay(30000);Delay(30000);Delay(30000);Delay(30000);
			Delay(30000);Delay(30000);Delay(30000);Delay(30000);Delay(30000);Delay(30000);Delay(30000);Delay(30000);
			Delay(30000);Delay(30000);Delay(30000);Delay(30000);Delay(30000);Delay(30000);Delay(30000);Delay(30000);
			Delay(30000);Delay(30000);Delay(30000);Delay(30000);Delay(30000);Delay(30000);Delay(30000);Delay(30000);
			Delay(30000);Delay(30000);Delay(30000);Delay(30000);Delay(30000);Delay(30000);Delay(30000);Delay(30000);
			Delay(30000);Delay(30000);Delay(30000);Delay(30000);Delay(30000);Delay(30000);Delay(30000);Delay(30000);
			Delay(30000);Delay(30000);Delay(30000);Delay(30000);Delay(30000);Delay(30000);Delay(30000);Delay(30000);
			GPIO_ResetBits(GPIOC, GPIO_Pin_13);	
}