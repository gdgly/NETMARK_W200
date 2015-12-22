#include "main.h"

//static ais_status power_full_flag=off; //使能充电状态下的电量指示标志


// /**********************************************
// 函数名：Enter_ChargingMode
// 功能：配置进入充电模式下的状态
// 输入：
// 输出：
// ***********************************************/
// static void Enter_ChargingMode(void)
// {
// 	PA_OFF();
// 	PLL_OFF();
// 	GPS_OFF();
// 	flag_gps_valid=on;//gps无效

// 	TIM3_OFF();	 //关闭T3中断
// 	task_flag2=off; // 充电时不发送AIS消息
// 	
// 	key_on_flag=off;//进入充电模式后开机灯不闪
// 	
// 	if(clk_convert_flag==on)  //切换高速时钟
// 	{
// 		SYS_CLK_UP();
// 		USART1_Config();
// 		USART2_Config();
// 		USART3_Config();
// 		clk_convert_flag=off;
// 	}
// }
// /*******************************************
// 函数名：Exit_ChargingMode
// 功能：配置退出充电模式下的状态
// 输入：
// 输出：
// ********************************************/
// static void Exit_ChargingMode(void)
// {
// // 	GPIO_SetBits(GPIOD, GPIO_Pin_12); //L6,L7 off;
// // 	GPIO_SetBits(GPIOD, GPIO_Pin_11); //L8 0ff;
// // 	GPIO_SetBits(GPIOA, GPIO_Pin_0); //L9 0ff;
// // 	GPIO_SetBits(GPIOC, GPIO_Pin_3); //L3 0ff;
// // 	GPIO_SetBits(GPIOD, GPIO_Pin_10); //L4 0ff;
// // 	GPIO_SetBits(GPIOC, GPIO_Pin_2); //L1 off;
// // 	GPIO_SetBits(GPIOD, GPIO_Pin_9); //L2 off;
// 	
// 	//GPIO_SetBits(GPIOB, GPIO_Pin_9); //L5 off;
// 	
// 	power_full_flag=off;   // 失能充电状态下的电量指示
// }

/**************************************************
 * 函数名：Enter_PowersaveMode
 * 描述  ：进入省电模式
 * 输入  ：无
 * 输出  ：无
 **************************************************/
void Enter_PowersaveMode(void)
{
  PA_OFF();
		PLL_OFF();
		GPS_OFF();
	
	/* Disable the USART2 Interutpt */
	//USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);  
	/* Disable the RTC Alarm */
	//RTC_ITConfig(RTC_IT_ALR, DISABLE);

// 	TIM_ITConfig(TIM3, TIM_IT_Update , DISABLE); //关闭T3
// 	TIM_Cmd(TIM3, DISABLE);	
	
	task_flag2=off; //不发送任务
	flag_gps_valid=off; //GPS无效
	//key_on_flag=off; //要是开灯不闪

// 	/* 关闭外设时钟*/
// 	RCC_APB1PeriphResetCmd(RCC_APB1Periph_ALL,DISABLE);
// 	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ALL,DISABLE);
// 	
// 	/* 打开GPIOA~E时钟，打开ADC时钟*/
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_USART1, ENABLE);
// 	
// 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
// 	

// 	//进入睡眠
// 	SCB->SCR &= (uint32_t)~((uint32_t)SCB_SCR_SLEEPDEEP);	//睡眠模式
// 	SCB->SCR |= (uint32_t)SCB_SCR_SLEEPONEXIT;				//SLEEP-ON-EXIT模式
// 	__WFI();
	
}

