#include "main.h"
//#include "stm32f10x_pwr.h"
//#include "stm32f10x_rcc.h"

/* Cortex System Control register address */
#define SCB_SysCtrl              ((uint32_t)0xE000ED10)

/* SLEEPDEEP bit mask */
#define SysCtrl_SLEEPDEEP_Set    ((uint32_t)0x00000004)
#define SysCtrl_SLEEPONEXIT_Set   ( (uint32_t)0x00000002)

u8 t_cnt=0;//����Ƶƫ����������

//static u8 rxbuf[17];			 //���ջ���
//static u8 i;

static u16 real_cnt=0x3d70; //ddsƵ��Ϊ9.059696MHzʱ������T4���������ֵ


/************************************************************
 * ��������Get_ChipID
 * ����  ����ȡоƬID,��flash�д洢�����ݱȽ��ж��Ƿ�һ��
 * ����  ����
 * ���  ��chip_passed
 ************************************************************/
u8 Get_ChipID(void)
{
	u8 i=0;
	u8 chip_passed=0;
	u32 ChipUniqueID[3];   //оƬ96bit UID
	ChipUniqueID[0] = *(__IO u32 *)(0X1FFFF7F0); // ���ֽ�
	ChipUniqueID[1] = *(__IO u32 *)(0X1FFFF7EC); // 
	ChipUniqueID[2] = *(__IO u32 *)(0X1FFFF7E8); // ���ֽ�
	for(i=0;i<=2;i++)
	{
		if(ChipUniqueID[i]==ChipUniqueID_Flash[i])
			chip_passed++;
	}
	return chip_passed;
}
/********************************************************
 * ��������Is_Coded
 * ����  ���ж��豸�Ƿ����
 * ����  ��
 * ���  ��
*********************************************************/
void Is_Coded(void)
{
	u8 i;
	u8 code_cmp1=0;  //�����ж��Ƿ�д�������
	u8 code_cmp2=0;
	/////////////�ж��Ƿ�д�������//////////////////////
	for(i=0;i<6;i++)
	{
		if(CodeNum[i]==0xff)
			code_cmp1++;
	}
	for(i=0;i<6;i++)
	{
		if(CodeNum[i]==0x00)
			code_cmp2++;
	}
	if(   (code_cmp1==6) || (code_cmp2==6)  )
	{
		code_flag1=off;
		code_flag2=off;
	}
	else
	{
		code_flag1=off;
		code_flag2=on;
	}
	 
// 	USART_SendData(USART1, code_flag2);   //�Ƿ����
//          while (!(USART1->SR & USART_FLAG_TXE));
// 	USART_SendData(USART1, code_flag1);         //�����Ƿ���ȷ
//          while (!(USART1->SR & USART_FLAG_TXE));
}

/*********************************************
 * ��������EXTI0_NVIC_Configuration
 * ����  ������Ƕ�������жϿ�����NVIC
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 *********************************************/
static void EXTI0_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* ����P[A|B|C|D|E]0Ϊ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/*********************************************************
 * ��������EXTI_PB0_Config
 * ����  ������ PB0 Ϊ���жϿڣ��������ж����ȼ�
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 *********************************************************/
void EXTI_PB0_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	/* config the extiline(PB0) clock and AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);

	/* config the NVIC(PB0) */
	EXTI0_NVIC_Configuration();

	/* EXTI line gpio config(PB0) */	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	 // 
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* EXTI line(PB0) mode config */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0); 
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�½����ж�
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
}

/*********************************************************
 * ��������EXTI_OFF
 * ����  ���ر���0�ϵ��ⲿ�����ж�
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 *********************************************************/
void EXTI_OFF(void) //�ر��ж�0
{
	/* Disable the selected external lines */
	*(__IO uint32_t *)(EXTI_BASE + (uint32_t)0x00)&= ~((uint32_t)0x00001);
}
/*********************************************************
 * ��������EXTI_ON
 * ����  ���ر���0�ϵ��ⲿ�����ж�
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 *********************************************************/
void EXTI_ON(void)
{
	/* enable the selected external lines */
	*(__IO uint32_t *)(EXTI_BASE + (uint32_t)0x00)|= ((uint32_t)0x00001);
}
/*********************************************************
 * ��������EXTI0_IRQHandler
 * ����  ��I/O���жϣ��ж���ΪPB0
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 *********************************************************/
void EXTI0_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line0) != RESET) 
  {
	  //	GPIO_WriteBit(GPIOC, GPIO_Pin_2,	(BitAction)(1-GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_2)) ); //L1��������ʱ��ת
			if(flag_fre_check==on)
			{
				
				switch(t_cnt)
				{
					case 0:
							//GPIO_ResetBits(GPIOA, GPIO_Pin_0); //L9 on for test,���������ʾGPS������ʱ��
							GPIO_ResetBits(GPIOB, GPIO_Pin_10); //dds on
							Delay(1000);
							//dds_wr_parrel(0x0f00,0x7600,0x3d00,0xd800);//д�뵥Ƶ�ź�9.059696MHz
							t_cnt=1;
						break;
					case 1:
						TIM4_ON();
						t_cnt=2;
						break;
					case 2:
						fre_cnt=TIM_GetCounter(TIM4);  //fre_cnt��real_cnt--32997(0x80e5������׼ȷʱ�ļ���ֵ)�Ƚ�
						TIM4_OFF();
						GPIO_SetBits(GPIOB, GPIO_Pin_10); //dds off
						t_cnt=3;
						break;
					case 3:						
						if(fre_cnt>=real_cnt)
						{
							fre_cnt=fre_cnt-real_cnt;
							freq_pn_flag=on;       //ƵƫΪ��	
						}
						else
						{
							fre_cnt=real_cnt-fre_cnt;
							freq_pn_flag=off;  //ƵƫΪ��
						}
						if(fre_cnt<10000)   //����1000ppm���ڵ�Ƶƫ
						{
//							freq_value_update();
						}		
						gps_off_flag=on;
						gps_off_cnt=0;
						fre_check_off(); 
						t_cnt=4;
						break;
					default:
						break;
				}		
			}
		
    EXTI_ClearITPendingBit(EXTI_Line0);     //����жϱ�־λ
  }  
}
