#include "main.h"

//static ais_status power_full_flag=off; //ʹ�ܳ��״̬�µĵ���ָʾ��־


// /**********************************************
// ��������Enter_ChargingMode
// ���ܣ����ý�����ģʽ�µ�״̬
// ���룺
// �����
// ***********************************************/
// static void Enter_ChargingMode(void)
// {
// 	PA_OFF();
// 	PLL_OFF();
// 	GPS_OFF();
// 	flag_gps_valid=on;//gps��Ч

// 	TIM3_OFF();	 //�ر�T3�ж�
// 	task_flag2=off; // ���ʱ������AIS��Ϣ
// 	
// 	key_on_flag=off;//������ģʽ�󿪻��Ʋ���
// 	
// 	if(clk_convert_flag==on)  //�л�����ʱ��
// 	{
// 		SYS_CLK_UP();
// 		USART1_Config();
// 		USART2_Config();
// 		USART3_Config();
// 		clk_convert_flag=off;
// 	}
// }
// /*******************************************
// ��������Exit_ChargingMode
// ���ܣ������˳����ģʽ�µ�״̬
// ���룺
// �����
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
// 	power_full_flag=off;   // ʧ�ܳ��״̬�µĵ���ָʾ
// }

/**************************************************
 * ��������Enter_PowersaveMode
 * ����  ������ʡ��ģʽ
 * ����  ����
 * ���  ����
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

// 	TIM_ITConfig(TIM3, TIM_IT_Update , DISABLE); //�ر�T3
// 	TIM_Cmd(TIM3, DISABLE);	
	
	task_flag2=off; //����������
	flag_gps_valid=off; //GPS��Ч
	//key_on_flag=off; //Ҫ�ǿ��Ʋ���

// 	/* �ر�����ʱ��*/
// 	RCC_APB1PeriphResetCmd(RCC_APB1Periph_ALL,DISABLE);
// 	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ALL,DISABLE);
// 	
// 	/* ��GPIOA~Eʱ�ӣ���ADCʱ��*/
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_USART1, ENABLE);
// 	
// 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
// 	

// 	//����˯��
// 	SCB->SCR &= (uint32_t)~((uint32_t)SCB_SCR_SLEEPDEEP);	//˯��ģʽ
// 	SCB->SCR |= (uint32_t)SCB_SCR_SLEEPONEXIT;				//SLEEP-ON-EXITģʽ
// 	__WFI();
	
}

