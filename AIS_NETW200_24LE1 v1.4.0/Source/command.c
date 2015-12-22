#include "reg24le1.h"
#include "nRF24L01.h"
#include "main.h"
#include "uart.h"
#include "config.h"
#include "command.h"
#include "adc.h"
#include "rtc.h"
//#include "led.h"
#include "hal_flash.h"

extern char chargeval;

extern volatile char cmd0x29;
extern volatile char cmd0x1c;

typedef enum
{ 
	off=0,
	on
}ais_status;

static ais_status str_cmp(INT8U* str_src , INT8U* str_dst , INT8U num)  //比较两个数组是否相等  :判断船名钥匙
{
	ais_status is_equal=on;
	while(num--)
	{
		if((*str_src) != (*str_dst))
			is_equal=off;
		str_src++;
		str_dst++;
	}
	return is_equal;
}

INT8U txbuf[30] = {0x00};
INT8U rxbuf[30] = {0x00};
static ais_status str_cmp_flag;//数组是否相等结果，on标志相同；off标志不同。
static INT8U *ptr_cmp1;  //用于比较网位仪船名与钥匙船名的指针
static INT8U *ptr_cmp2;  //用于比较船名的指针
static INT8U cnt_cmp; //用于比较船名的计数值

extern INT8U msg17_state[30];
extern INT8U msg18_state[30];
extern INT16U adc_level;
extern INT16U adc_charge;
extern INT8U charge_state;
extern INT8U config_state;
static INT8U i,j;
//INT8U in0x17=0;
extern volatile INT16U timinteval;
extern INT8U timinteval_1;
extern INT8U timinteval_2;

extern volatile char cmd0x0f;
extern volatile char cmd0x10;

void wakepin(void) //唤醒管脚函数
{
	P1DIR &= ~0x01;
	P10=0;
	delay(0xff);delay(0xff);
	P10=1;
	delay(0xff);delay(0xff);
	P10=0;
	delay(0xff);delay(0xff);
	P10=1;
	delay(0xff);delay(0xff);
	P10=0;
	delay(0xff);delay(0xff);
	//P10=1;
	delay(0xff);delay(0xff);delay(0xff);delay(0xff);
	
// 	P1DIR &= ~0x01;
// 	P10=0;
// 	delay(0xff);
// 	P10=1;		
// 	delay(0xff);
	
// 	P1DIR &= ~0x01;
// 	P10=0;
// 	delay(0xff);
// 	P10=1;			
}
	
void Command(void)  //RF接收后由串口发送
{
	switch(rxbuf[0])
	{		
		case 0x01://向网位仪写船名
 		if(config_state == 1)
 		{
			//wakepin();
			
			for(i=0;i<16;i++)
				boatnum[i] = rxbuf[i+1];
			
			WriteflashBoatnum();
			
			putstr(rxbuf);
		}
		break;
		case 0x02://向网位仪写MMSI号
		if(config_state == 1)
 		{
			//wakepin();
			putstr(rxbuf);
			}
		break;
		case 0x04://向网位仪读取助航设备名称
		if(config_state == 1)
 		{
			//wakepin();
			putstr(rxbuf);
		}
		break;
		case 0x05://向网位仪读取MMSI号
		if(config_state == 1)
 		{
			//wakepin();
			putstr(rxbuf);
		}
		break;
		case 0x0A://向网位仪加密
		if(config_state == 1)
 		{
			//wakepin();
			putstr(rxbuf);
		}
		break;
		case 0x0B://向网位仪解密
		if(config_state == 1)
 		{
			//wakepin();
			putstr(rxbuf);
		}
		break;
		case 0x0C://恢复出厂设置，清除密码
		if(config_state == 1)
 		{
			hal_flash_page_erase(34);
			PCON &= ~0x10;//PMW
			
			taskstart = 1;
//			TR0 = 1;
//			hal_flash_bytes_write(0xFC31,&taskstart,1);
			
			//wakepin();
			putstr(rxbuf);
		}
		break;
		case 0x0f://钥匙开启!
							if(config_state == 0 && charge_state == 0)
								{
											ptr_cmp1=&rxbuf[2];  //钥匙船名第一位指针
											ptr_cmp2=&boatnum[1];     //网位仪船名第一位指针
											//if(boatnum[boatnum[0]/6-1]=='-')  //判断网号个数 1位
											//{
												if(rxbuf[1]==boatnum[0]-6)
												{	
													wakepin();
													cnt_cmp=(boatnum[0]-6)/8;
													str_cmp_flag=str_cmp(ptr_cmp1,ptr_cmp2,cnt_cmp);
													
													if(str_cmp_flag==on)     //船名判断一致
													{
														wakepin();
														putstr(rxbuf);
														
														cmd0x0f = 1;
													}
												}
												else if(rxbuf[1]==boatnum[0]-12)
												{
													wakepin();
													cnt_cmp=(boatnum[0]-12)/8;
													str_cmp_flag=str_cmp(ptr_cmp1,ptr_cmp2,cnt_cmp);
													
													if(str_cmp_flag==on)     //船名判断一致
													{
														wakepin();
														putstr(rxbuf);
														
														cmd0x0f = 1;
													}
												}
								//}
							}
							break;
		case 0x10://钥匙关闭!
// 			P1DIR &= ~0x01;
// 			P10=0;
// 			delay(0xff);delay(0xff);
// 			P10=1;
// 		 delay(0xff);delay(0xff);
// 			P10=0;
// 			delay(0xff);delay(0xff);
// 			P10=1;
// 			delay(0xff);delay(0xff);
// 			P10=0;
// 			delay(0xff);delay(0xff);
// 			P10=1;
// 			delay(0xff);delay(0xff);delay(0xff);delay(0xff);
		if(config_state == 0 && charge_state == 0)
 	{
		  	ptr_cmp1=&rxbuf[2];  //钥匙船名第一位指针
			  ptr_cmp2=&boatnum[1];     //网位仪船名第一位指针
		  	if(rxbuf[1]==boatnum[0]-6)
	  		{
			   	cnt_cmp=(boatnum[0]-6)/8;
			   	str_cmp_flag=str_cmp(ptr_cmp1,ptr_cmp2,cnt_cmp);
				
			   	if(str_cmp_flag==on)     //船名判断一致
			   	{
			    	 wakepin();
				    	putstr(rxbuf);
				   }
			  }
			  else if(rxbuf[1]==boatnum[0]-12)
		  	{
				   cnt_cmp=(boatnum[0]-12)/8;
			   	str_cmp_flag=str_cmp(ptr_cmp1,ptr_cmp2,cnt_cmp);
				
				   if(str_cmp_flag==on)     //船名判断一致
				   {
				    	wakepin();
				    	putstr(rxbuf);
				   }
			  }
 	}
		break;
		case 0x11://写入pn码生成多项式
		if(config_state == 1)
 		{
			//wakepin();
			putstr(rxbuf);
		}
		break;
		case 0x12://发射间隔设置
		if(config_state == 1)
 		{
			WriteflashInterval();
			
			//wakepin();
			putstr(rxbuf);
		}
		break;
		case 0x16://ID激活
			//wakepin();
			putstr(rxbuf);
		break;
		case 0x17://网位仪连接 上位机!
			
// 			P1DIR &= ~0x01;
// 			P10=0;
// 			delay(0xff);
// 			P10=1;

// 			Ord17func();
// 			ALL_LED_UP();
// 		if(charge_state == 1)
// 		{
// 			in0x17 =1;
// 			EA = 0;
			//adc_square();
// 			EA = 1;
// 			in0x17 =0;
//		}
 		if(config_state == 1)
 		{		
			//wakepin();
			delay(0xff);
		
			msg17_state[1] = 1;
			putstr(msg17_state);
			config_state = 1;
		}
		break;
		case 0x18://网位仪断开!
 		if(config_state == 1)
 		{		
			//wakepin();
			delay(0xff);
		
			putstr(rxbuf);
			ALL_LED_DOWN();
		
			config_state = 0;
		}
		break;
		case 0x19:	//尺寸位置参考点注入
		if(config_state == 1)
 		{
			//wakepin();
			putstr(rxbuf);
		}
		break;
		case 0x20:	//尺寸位置参考点读取
		if(config_state == 1)
 		{
			//wakepin();
			putstr(rxbuf);
		}
		break;
		case 0x21://电子定位装置类型 注入
		if(config_state == 1)
 		{
			//wakepin();
			putstr(rxbuf);
		}
		break;
		case 0x22://电子定位装置类型 读取
		if(config_state == 1)
 		{
			//wakepin();
			putstr(rxbuf);
		}
		break;
		case 0x23://助航设备类型 注入
		if(config_state == 1)
 		{
			//wakepin();
			putstr(rxbuf);
		}
		break;
		case 0x25://助航设备类型 读取
		if(config_state == 1)
 		{
			//wakepin();
			putstr(rxbuf);
		}
		break;
		case 0x26: //发射时更改GPS计数
		if(config_state == 1)
 		{
			//wakepin();
			putstr(rxbuf);
		}
		break;
		case 0x27://钥匙 省电
		if(config_state == 0 && charge_state == 0)
 		{		
			ptr_cmp1=&rxbuf[2];  //钥匙船名第一位指针
			ptr_cmp2=&boatnum[1];     //网位仪船名第一位指针
			if(rxbuf[1]==boatnum[0]-12)
			{
				cnt_cmp=(boatnum[0]-12)/8;
				str_cmp_flag=str_cmp(ptr_cmp1,ptr_cmp2,cnt_cmp);
				
				if(str_cmp_flag==on)     //船名判断一致
				{
//RF off
					RFCON = 0x00;
					RFCE = 0;
					RFCKEN = 0;
					RF = 0;
					
					ALL_LED_DOWN();
					P0DIR |= 0XA7;       //关闭GPIO
					P1DIR |= 0X7A;
					
					CLKCTRL = 0x10;
					CLKLFCTRL = 0x01;
				}
			}
		}
		break;
		case 0x2A://流网或张网选择
		if(config_state == 1)
 		{
			//wakepin();
			putstr(rxbuf);
		}
		break;
		case 0x2B://船尺寸设置
		if(config_state == 1)
 		{
			//wakepin();
			putstr(rxbuf);
		}
		break;
		default:
		break;
	}	
}

void Command2(void)  //串口接收后由无线发送或执行
{
	switch(com_buf[1])
	{			
		case 0x01://向网位仪写船名ack
			usart2rf();
			led_write_finished();
		break;
		case 0x02://向网位仪写MMSI号ack
			usart2rf();
			led_write_finished();
		break;
		case 0x04://向网位仪读取船名ack
			usart2rf();
			led_write_finished();
		break;
		case 0x05://向网位仪读取MMSI号ack
			usart2rf();
			led_write_finished();
		break;
		case 0x0A://向网位仪加密ack
			if(com_buf[2] == 0x01)
			{
				codeflag = 1;
				WriteflashCode();
			}
		
			usart2rf();
			led_write_finished();
		break;
		case 0x0B://向网位仪解密ack
			if(com_buf[2] != 0x01)
			{
				codeflag = 0;
				WriteflashCode();
			}
			
			usart2rf();
			led_write_finished();
		break;
		case 0x0C://恢复出厂设置，清除密码ack
			usart2rf();
			led_write_finished();
		break;
		case 0x0f://钥匙开启
//			if(cmd0x0f == 0)
//			{				
	//			WriteStartTask();

	//			delay_ms(300);
	// 			chargeval = get_led_charge() - START_NUM;
	// 			if(chargeval < 0)   chargeval = 0;
			
// 			P0DIR &= ~0XA7;       //set p0.0 p0.1 p0.2 p0.5 p0.7 output
// 			P1DIR &= ~0X7A;
// 			led_round(1,200);		
			
				//led_startup(chargeval);
			
				taskstart = 1;
//				TR0 = 1;
			
				cmd0x0f = 1;
				cmd0x10 = 0;
			  cmd0x1c = 0;
			
				//usart2rf(); /////test
//			}
		break;
		case 0x10://钥匙关闭
//			TR0 = 0;
		 	taskstart = 0;
//			WriteStartTask();
		
			delay_ms2(300);
		
			cmd0x0f = 0;
			cmd0x10 = 1;
			cmd0x29 = 0;
			cmd0x1c = 0;
		
			chargeval = get_led_charge() - START_NUM +1;
			if(chargeval < 0)   chargeval = 0;			 
			led_shutdown(chargeval);
			
			//usart2rf(); /////test
		break;
		case 0x11://写入pn码生成多项式ack
			usart2rf();
		break;
		case 0x12://发射间隔设置ack
			usart2rf();
			led_write_finished();
		break;
		case 0x16://ID激活 led
			//led_activate();
			led_write_finished();
			ALL_LED_DOWN();
		break;
		case 0x17://网位仪连接
// 			usart2rf();//
// 			TR0 = 0;
// 			led_write_finished();
		break;
		case 0x18://网位仪断开
			//usart2rf();//
			//TR0 = 0;
			//led_write_finished();
// 			config_state = 0;  //退出配置模式
// 			charge_state = 1;
		break;
		case 0x19://尺寸位置参考点注入
			usart2rf();
			led_write_finished();
		break;
		case 0x1C://GPS有效标志
			cmd0x1c = com_buf[2];
		break;
		case 0x20://尺寸位置参考点读取
			usart2rf();
			led_write_finished();
		break;
		case 0x21://电子定位装置类型 注入
			usart2rf();
			led_write_finished();
		break;
		case 0x22://电子定位装置类型 读取
			usart2rf();
			led_write_finished();
		break;
		case 0x23://助航设备类型 注入
			usart2rf();
			led_write_finished();
		break;
		case 0x25://助航设备类型 读取
			usart2rf();
			led_write_finished();
		break;
		case 0x29://夜间亮灯
		if(taskstart == 1)
 		{
			if(com_buf[2] == 0x01) //night
			{
				cmd0x29 = 1;
			}
			else if(com_buf[2] == 0x00)  //day
			{
				cmd0x29 = 0;
			}
		}
		break;
		case 0x2A://流网或张网选择
			usart2rf();
			led_write_finished();
		break;
		case 0x2B://船尺寸设置
			usart2rf();
			led_write_finished();
		break;
		default:
		break;	
	}	
}

void usart2rf(void)
{
	for(j=0;j<17;j++)
		txbuf[j] = com_buf[j+1];

	L01_Init( );	              	//Initialize the internal 24L01P    
	L01_SetTXMode( );               //Set as TX mode
	L01_FlushTX( );
	L01_FlushRX( );
	rf_flag = 0;
	delay(1000);
	delay(1000);
	delay(1000);

	for(itmp=2;itmp>1;itmp--)
	{
			L01_WriteTXPayload_Ack( txbuf, 17 );
			delay(1000);
			delay(1000);
			delay(1000);
			delay(1000);
			delay(1000);
			delay(1000);
	}
	
	L01_Init( );	
	L01_SetRXMode( );               //Set as RX mode
	delay(1000);
	delay(1000);
	delay(1000);
	L01_FlushRX( );	
	rf_flag = 0;	
}
	
