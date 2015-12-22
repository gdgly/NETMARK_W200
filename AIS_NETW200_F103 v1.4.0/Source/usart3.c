
//#include <stdarg.h>
#include "main.h"
// #include "stm32f10x_pwr.h"
// #include "stm32f10x_rcc.h"

// static ais_status str_cmp_flag;//数组是否相等结果，on标志相同；off标志不同。
// static u8 *ptr_cmp1;  //用于比较网位仪船名与钥匙船名的指针
// static u8 *ptr_cmp2;  //用于比较船名的指针
// static u8 cnt_cmp; //用于比较船名的计数值。

static u8 com3_start;
//static u8 com3_stop;
static u8 com3_num;
static u8 com3_rxbuf[31];  //串口3接收缓存
u8 txbuf[31]; 	   //串口3发送缓冲
static u8 i;
extern u8 swchflag;

// /********************************************************
//  * 函数名：str_cmp
//  * 描述  ：比较两个数组是否相等；
//  * 输入  ：str_src,str_dst
//  * 输出  ：is_equal
// *********************************************************/
// static ais_status str_cmp(u8* str_src , u8* str_dst , u8 num)
// {
// 	ais_status is_equal=on;
// 	while(num--)
// 	{
// 		if((*str_src) != (*str_dst))
// 			is_equal=off;
// 		str_src++;
// 		str_dst++;
// 	}
// 	return is_equal;
// }

/*********************************************************
 * 函数名：sendback
 * 描述  ：无线发送指令至底座
 * 输入  ：无
 * 输出  ：无
 *********************************************************/
void sendback(void)   
{
	u8 i;
	
	for(i=0;i<18;i++)	
	{
		USART_SendData(USART3, txbuf[i]);
		while (!(USART3->SR & USART_FLAG_TXE)) ;
	}
					
}

void USART3_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  													
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * 函数名：USART3_Config
 * 描述  ：USART3 GPIO 配置,工作模式配置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */
void USART3_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	USART3_NVIC_Configuration();
	
	/* config USART3 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	/* USART3 GPIO config */
   /* Configure USART2 Tx (PB.10) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	    
  /* Configure USART2 Rx (PB.11) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	  
	/* USART3 mode config */
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);  //设置串口接收中断 
		
  USART_Cmd(USART3, ENABLE);
}

void USART3_IRQHandler(void)
{   
	u8 ch; 
	code_cnt=0;

	if(USART3->SR & USART_FLAG_RXNE)//接收中断
	{
		USART3->SR &= ~USART_FLAG_RXNE;//清零
		ch = USART3->DR & 0x1ff;
		
 		USART_SendData(USART1, ch);
 		while (!(USART1->SR & USART_FLAG_TXE)) ;
		
		if('$'==ch && com3_num == 0)
		{
			com3_start = 1;
			//com3_stop  = 0;
		}
		if (1==com3_start)  
		{
			com3_rxbuf[com3_num++] = ch;  
			
			if (com3_num == 18)  //串口最大长度为18
			{
				com3_start = 0;
				//com3_stop  = 1;
				Usart3GetCommand();//在串口中执行任务
				com3_num = 0;
			}
		}			
	}
	
}

void Usart3GetCommand(void)  //串口3接收
{
	switch(com3_rxbuf[1])	
	{
		case 0x01://向网位仪写船名
							if(wireless_en==1) 
							{
									if(code_flag2==on)  //加密情况
									{
											if(code_flag1==on)  //密码正确
											{
													for(i=0;i<16;i++)
													boatnum[i] = com3_rxbuf[i+2];
													WriteflashBoatnum();//flashw
													ReadflashBoatnum();
													txbuf[0] = '$'; txbuf[1] = 0x01;txbuf[2] = 0x01; //应答 写入船名成功
													for(i=3;i<18;i++)
																txbuf[i]=0x00;

													sendback();	
											}
											else  //密码错误
											{
													txbuf[0] = '$'; txbuf[1] = 0x01;txbuf[2] = 0x02; //应答 请先解密
													for(i=3;i<18;i++)
																txbuf[i]=0x00;
												
													sendback();
													code_flag1=off; //设置密码错误标志。
											}
									}
									else //未加密情况
									{
											for(i=0;i<16;i++)
														boatnum[i] = com3_rxbuf[i+2];
										
											WriteflashBoatnum();//flashw
											ReadflashBoatnum();
											txbuf[0] = '$'; 
											txbuf[1] = 0x01;
											txbuf[2] = 0x01; //应答 写入船名成功
											for(i=3;i<18;i++)
														txbuf[i]=0x00;
									
											sendback();
									}
							}
							break;
							
		case 0x02://向网位仪写MMSI号   02 18 95 5C 8A			
 		    if(wireless_en==1)
 	    	{
			      if(code_flag2==on)   //加密情况
		 	     {
				       if(code_flag1==on)
				       {
													MMSI=0;
													MMSI += (com3_rxbuf[2] << 24); //+ (rxbuf[2] << 16) + (rxbuf[3] << 8) + rxbuf[4];
													MMSI += (com3_rxbuf[3] << 16);
													MMSI += (com3_rxbuf[4] << 8);
													MMSI += com3_rxbuf[5];
													WriteflashMMSI();//flashw
													ReadflashMMSI();
											
													txbuf[0] = '$'; txbuf[1] = 0x02;txbuf[2] = 0x01; //正确写入mmsi
													for(i=3;i<18;i++)
														  txbuf[i]=0x00;

													sendback();
												}
												else
												{
													txbuf[0] = '$'; txbuf[1] = 0x02;txbuf[2] = 0x02; //请输入密码
													for(i=3;i<18;i++)
														  txbuf[i]=0x00;
													
													sendback();
													code_flag1=off; //设置密码错误标志
												}
		 	     }
									else
									{
											MMSI=0;
											MMSI += (com3_rxbuf[2] << 24); //+ (rxbuf[2] << 16) + (rxbuf[3] << 8) + rxbuf[4];
											MMSI += (com3_rxbuf[3] << 16);
											MMSI += (com3_rxbuf[4] << 8);
											MMSI += com3_rxbuf[5];

											WriteflashMMSI();//flashw
											ReadflashMMSI();
											txbuf[0] = '$'; txbuf[1] = 0x02;txbuf[2] = 0x01; //应答
											for(i=3;i<18;i++)
												  txbuf[i]=0x00;
										
									}
				     sendback();
 	    	}
	     	break;
		
		case 0x04://向网位仪读取船名		
			    if (wireless_en == 1)
							{
									txbuf[0] = '$'; 
									txbuf[1] = 0x04;

									for(i=0;i<18;i++)   
									txbuf[i+2]=boatnum[i];

									sendback();
							}
							break;
			
		case 0x05://向网位仪读取MMSI号
							if(wireless_en==1)
							{
							txbuf[0] = '$';txbuf[1] = 0x05;
							for(i=0;i<4;i++)
								 txbuf[i+2] = MMSI >> (24 - i*8);	
							for(i=6;i<18;i++)
								 txbuf[i]=0x00;
							sendback();		
						 }
				   break;

		case 0x0A://向网位仪加密
							if(wireless_en==1)
							{
										Is_Coded();
										if(code_flag2==off)          //若设备未加密，加密。
										{
													for(i=0;i<6;i++)
													   CodeNum[i] = com3_rxbuf[i+2];
											
													WriteflashCode();
													ReadflashCode();
													
													txbuf[0] = '$'; txbuf[1]=0x0A; txbuf[2]=0x01;  //加密成功
													for(i=3;i<18;i++)
														  txbuf[i]=0x00;
											
													code_flag2=on;      //设置网位仪加密状态
													code_flag1=off;
													sendback();	
										}
										else                     //若设备已加密，请勿重复加密。
										{
											txbuf[0] = '$'; txbuf[1]=0x0A; txbuf[2]=0x02;
											for(i=3;i<18;i++)
												 txbuf[i]=0x00; 
											
											sendback();
										}
							}
		     break;
			
		case 0x0B://向网位仪解密
							if(wireless_en==1)
							{
									if(code_flag2==on)
									{
											for(i=0;i<6;i++)
											{
													if(CodeNum[i] == com3_rxbuf[i+2])
													{
														 code_cnt++;
													}
											}

											if(code_cnt==6)
											{
													txbuf[0] = '$'; txbuf[1]=0x0B; txbuf[2]=0x01;   //解密成功，可以写入mmsi或船名
													for(i=3;i<18;i++)
														  txbuf[i]=0x00;

													code_flag1=on;  //设置密码正确标志
													
													for(i=0;i<6;i++)
														  CodeNum[i] = 0xFF;
													WriteflashCode();
													ReadflashCode();
													
													code_flag2=off;      //设置网位仪未加密状态。
													code_flag1=off;  //设置密码错误标志		
													
													sendback();
											}
										 else
										 {
											  txbuf[0] = '$'; txbuf[1]=0x0B; txbuf[2]=0x02;    //设备已加密。输入密码错误
										  	for(i=3;i<18;i++)
												    txbuf[i]=0x00;
	
										  	code_flag1=off;  //设置密码错误标志

											  sendback();
										 }
									}											
							  else
									{
										 txbuf[0] = '$'; txbuf[1]=0x0B; txbuf[2]=0x03;    //设备未加密，询问是否需要加密
											for(i=3;i<18;i++)
												  txbuf[i]=0x00;

										 code_flag1=off;   //默认密码错误状态
									 	sendback();	
									}
							}
		     break;
		
		case 0x0C:   //恢复出厂设置，清除密码
							if(wireless_en==1)
							{
									for(i=0;i<6;i++)
										  CodeNum[i] = 0xFF;
								
									WriteflashCode();
									ReadflashCode();
									
									code_flag2=off;      //设置网位仪未加密状态。
									code_flag1=off;  //设置密码错误标志			
									
									MMSI=00000000; 
									WriteflashMMSI();
									ReadflashMMSI();
								
									for(i=10;i<16;i++)
										  boatnum[i] = 0x00;
								
									WriteflashBoatnum();
									ReadflashBoatnum();
									
									ReadflashMPeriod();
									interval_num = intervalA;
									interval_s = intervalA;
									time_o = 0xffff;
									WriteflashMPeriod();

									flag_channel = 0;
									Write_TX_Channel();
									Read_TX_Channel();
									
									jingdu=0;weidu=0;
									Write_GPS_Info();
									Read_GPS_Info();
									
									txbuf[0] = '$'; txbuf[1]=0x0C; txbuf[2]=0x01;  //成功恢复出厂设置
									for(i=3;i<18;i++)
										  txbuf[i]=0x00;
			
									sendback();	
						}
		    break;
			
		case 0x0F:                //钥匙开
							txbuf[0] = '$'; 
		     txbuf[1]=0x0F; 
		     txbuf[2]=0x01;  //
							for(i=3;i<18;i++)
							{
							  	txbuf[i]=0x00;
							}
							sendback();
							
							swchflag = 0;
							
							ReadBackup();
							interval_num = intervalA;
							interval_s = intervalA;
							time_o = 0xffff;
							WriteBackup();
							Read_Flash_GPS_Info();
							
							task_flag2=on;
							break;
			
		case 0x10:             //钥匙关
							txbuf[0] = '$'; 
		     txbuf[1]=0x10; 
		     txbuf[2]=0x01;  //
							for(i=3;i<18;i++)
							{
										txbuf[i]=0x00;
							}
							sendback();
							
							ReadBackup();
							interval_num = intervalA;
							interval_s = intervalA;
							time_o = 0xffff;
							WriteBackup();
							Write_Flash_GPS_Info();  //存入flash
							
							Enter_PowersaveMode();
		     
		     RCC_RTCCLKCmd(DISABLE);
							PWR_WakeUpPinCmd(ENABLE);
							PWR_EnterSTANDBYMode();
	     	break;
			
		case 0x11:  //写入pn码生成多项式
							if(wireless_en==1)
							{
									msequence_poly=( ((u16)com3_rxbuf[2]) << 8 ) +com3_rxbuf[3];
									
									WriteflashMPoly();
									ReadflashMPoly();
								
									txbuf[0] = '$'; txbuf[1]=0x11; txbuf[2]=0x01;  //写入pn码成功
									for(i=3;i<18;i++)
												txbuf[i]=0x00;

									sendback();
							}
							break;
							
		case 0x12: //发射间隔设置 /间隔计数次数固定值 
							if(wireless_en==1)
							{
									intervalA = com3_rxbuf[6]; //发射间隔固定值
									intervalA = intervalA << 8;
									intervalA = intervalA + com3_rxbuf[7];

									gps_invalid = com3_rxbuf[8]; //gps失效时间2min
									gps_invalid = gps_invalid << 8;
									gps_invalid = gps_invalid + com3_rxbuf[9];

									// 				intervalB = com3_rxbuf[10]; //GPS长时间失效的启动值
									// 				intervalB = intervalB << 8;
									// 				intervalB = intervalB + com3_rxbuf[11];

									interval_num = intervalA;
									interval_s = intervalA;
									//gps_invalid_num = 0;		
									WriteflashMPeriod();
									ReadflashMPeriod();

									//	USART_SendData(USART1, interval_s);
									//	while (!(USART1->SR & USART_FLAG_TXE)) ;
									//	USART_SendData(USART1, interval_num);
									//	while (!(USART1->SR & USART_FLAG_TXE)) 

									// 				for(i=0;i<18;i++)	
									// 				{
									// 					USART_SendData(USART1, com3_rxbuf[i]);
									// 					while (!(USART1->SR & USART_FLAG_TXE)) ;
									// 				}				

									txbuf[0] = '$'; txbuf[1]=0x12; txbuf[2]=0x01; //配置成功
									for(i=3;i<18;i++)
									txbuf[i]=0x00;


									sendback();
						}	
						break;			
		case 0x16: //ID激活
						{
								ChipUniqueID_Flash[0]=*(__IO u32 *)(0X1FFFF7F0);
								ChipUniqueID_Flash[1]=*(__IO u32 *)(0X1FFFF7EC);
								ChipUniqueID_Flash[2]=*(__IO u32 *)(0X1FFFF7E8);
								
								WriteflashMUID();
								ReadflashMUID();	
								
								activation_flag=0x0101;  //激活标志	
								WriteflashMActivation();
							
									txbuf[0] = '$'; txbuf[1] = 0x16;
								for(i=2;i<18;i++)   txbuf[i]=0x00;
								sendback();	
							
								RTC_Init();
								PWR_WakeUpPinCmd(ENABLE);
								PWR_EnterSTANDBYMode();
						}
						break;
		
		case 0x17: //连接网位仪
							Enter_PowersaveMode();
							wireless_en = 1;
		     break;
			
		case 0x18: //断开网位仪
							if(wireless_en==1)
							{
								 wireless_en = 0;
									txbuf[0] = '$'; txbuf[1] = 0x18;
									for(i=2;i<18;i++)   
												txbuf[i]=0x00;
									
									sendback();	
							}
						 break;
// 		case 0x19: //尺寸位置参考点注入
// 			if(charging_flag==on && wireless_en==1)
// 			{
// 				dev_local[0] = com3_rxbuf[2];
// 				dev_local[1] = com3_rxbuf[3];
// 				dev_local[2] = com3_rxbuf[4];
// 				dev_local[3] = com3_rxbuf[5];
// 				dev_local[4] = com3_rxbuf[6];
// 				
// 				WriteflashBoatInfo();
// 				ReadflashBoatInfo();
// 			
// 				txbuf[0] = 0x24; txbuf[1] = 0x19; txbuf[2] = 0x01; 
// 				for(i=3;i<18;i++)  
// 					txbuf[i]=0x00;
// 				sendback();
// 				
// // 				for(i=0;i<16;i++)	
// // 				{
// // 					USART_SendData(USART1, txbuf[i]);
// // 							while (!(USART1->SR & USART_FLAG_TXE)) ;
// // 				}
// 			}
// 		break;
		case 0x1A: //进入/退出充
							{
									if(0 == com3_rxbuf[2])  //退出充电
									{
												if (com3_rxbuf[3] == 1) //充电前状态为开机
												{
														charging_flag = 0;
														interval_num = interval_s;
														task_flag2 = on;
												}
												else  // 充电前的状态为关机
												{
															RCC_RTCCLKCmd(DISABLE);
															PWR_WakeUpPinCmd(ENABLE);
															PWR_EnterSTANDBYMode();
												}
									}
									else  //进入充电
									{
										charging_flag = on;
										TIM3_OFF();
										task_flag2=off;
										
										GPS_OFF();
										TIM_Cmd(TIM2, DISABLE);	
									}
							}
					 	break;
							
// 		case 0x1B: //发射一次结束
// 			
// 		break;
		case 0x1C: //未接收到GPS
		break;

		case 0x2A: //流网或张网选择
							if(wireless_en == 1)
							{
									if(com3_rxbuf[2] == 0x01)  //帆张
										cogsel = 1;
									else  //流刺
										cogsel = 0;
									
									Write_Flash_Cogsel();
									
									txbuf[0] = '$'; txbuf[1] = 0x2A; txbuf[2] = 0x01;
									for(i=3;i<18;i++)   txbuf[i]=0x00;
									sendback();
							}
					  break;
		
		case 0x2B: //船尺寸设置
							if(wireless_en == 1)
							{
									boatlong = (com3_rxbuf[2] << 8) + com3_rxbuf[3];
									boatwidth = com3_rxbuf[4];
									
									WriteflashBoatInfo();
									
					// 					boatsize_a = boatlong/2;
					// 					boatsize_b = boatlong/2;
					// 					boatsize_c = boatwidth/2;
					// 					boatsize_d = boatwidth/2;
									
									txbuf[0] = '$'; txbuf[1] = 0x2B; txbuf[2] = 0x01;
									for(i=3;i<18;i++)   txbuf[i]=0x00;
									sendback();
							}
					  break;
			
		default:
		break;		
	}	
}













