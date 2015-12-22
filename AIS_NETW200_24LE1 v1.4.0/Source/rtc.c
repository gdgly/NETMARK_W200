#include "hal_rtc.h"
#include "nordic_common.h"
#include "rtc.h"
#include "reg24le1.h"
#include "nRF24L01.h"
#include "main.h"
#include "uart.h" 
#include "config.h"
#include "command.h"
#include "led.h"

#define TIM1CNT  33332
INT16U cunt=0; //rtc计数
INT16U adc_level=0;//电平值

INT16U adc_charge=0;//电量值
INT8U charge_state=0; //充电状态 0:非充电  1:充电
INT8U config_state=0; //配置状态 0：非配置状态  1：配置状态

INT8U min30reset[17] =  {0x19,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //30min reset
INT8U msg17_state[17] = {0x17,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //
INT8U msg18_state[17] = {0x18,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //
INT8U msg1A_state[17] = {0x1A,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //进入/退出充电  msg1A_state[1] :  0:非充电  1:充电
	
INT8U flag;
//extern INT8U in0x17;
	
#define ADCGATE  2480 //3600
INT8U connect_state_num=0; //判断连接上底座的计数
INT8U charge_state_num=0; //判断充电状态的计数
	
char chargeval; //充电临时变量
extern volatile char cmd0x0f;
extern volatile char cmd0x10;
extern volatile char cmd0x1c;
extern volatile char cmd0x29;
extern volatile char cmd0x29num;
	
extern char fullflag;
extern int fullflagnum;
	
// void rtc2_init(void)
// {
// 	RTC2CMP0=0xff;						   	// 定时时间2秒
// 	RTC2CMP1=0xff;
// // 	RTC2CMP0=0xff;						   	// 定时时间1秒
// // 	RTC2CMP1=0x7f;
// 	
// 	RTC2CON=0x07;							// 比较模式，启动RTC2
// 	WUIRQ=1;								// 使能TICK中断
// }

void timer1_init(void)
{
	TMOD=0X11;
	TH1=(65536-TIM1CNT)/256;
	TL1=(65536-TIM1CNT)%256;
	ET1=1;
	TR1=1;  //定时器开启
}

//void TICK_ISR( void ) interrupt INTERRUPT_TICK
void sev_timer1() interrupt INTERRUPT_T1
{
  static unsigned char time_number;
	
	ET1 = 0;
	TH1=(65536-TIM1CNT)/256;
	TL1=(65536-TIM1CNT)%256;
	
	time_number++;
	if(time_number==40)
	{
		time_number=0;
		
		watchdog_load(); //设置看门狗
	
	 	//CLKCTRL = 0x28;
	
		Is_Charge(); //判断是否在充电和配置   

		//Adc_AIN10();
		//adc_charge = readadc();
	  //putch((INT8U)(adc_charge >> 8)); putch((INT8U)(adc_charge));

		if(config_state == 1)   //配置
		{
		}
		else if(charge_state == 1)  //充电
		{
			Adc_AIN10();
			adc_charge = readadc();
			delay_ms3(200);
			adc_charge = adc_charge + readadc();
			delay_ms3(200);
			adc_charge = adc_charge + readadc();
			
			adc_charge = adc_charge/3;
			chargeval = get_led_charging() -1;
			if(chargeval < 0)   chargeval = 0;
			cmd0x0f = 0;
			cmd0x10 = 1;
			led_get_power(chargeval); //显示电量
			
			//TR0 = 0;
		}
		else  //非充电非配置
		{
			//powrdownmodeset(); //进入睡眠
			L01_Init( );
			L01_SetRXMode( );    //Set as RX mode
			L01_FlushRX( );
			rf_flag = 0;
			
			delay_ms2(200);
			L01_SetOFFMode();			
			
			if(cmd0x29 == 1)  //夜间闪烁计时
				cmd0x29num++;
// 			else if(cmd0x0f == 0)  //规避工作中所有红灯常亮
// 			{
// 				P0DIR |= 0XA7;       //关闭GPIO
// 				P1DIR |= 0X7A;   
// 				
// 				P1DIR &= ~0x01;   //开P10
// 			}
			
						
		}
		
// 		if(taskstart == 1)	
// 			TR0 = 1;		
	}

	ET1 = 1;
		
}

void Is_Charge(void)  //判断是否在充电
{
	Adc_AIN6(); //电平检测	测量是否拔出
	adc_level = readadc(); //检测电平
	delay_ms3(100);
	adc_level = adc_level + readadc();
	delay_ms3(100);
	adc_level = adc_level + readadc();
	adc_level = adc_level/3;
	
	if(adc_level <= CHARGEGATE)  //3100=2.5V  未插在底座上
	{
		charge_state_num = 0;
		
		if(charge_state == 1)  //只发送一次
		{	
			connect_state_num++;

			if(connect_state_num == 7)  //连续判断4次才退出充电
			{
				charge_state = 0;
				connect_state_num = 0;
				
				//wakepin();
// 			msg1A_state[1] = 0; 
//				putstr(msg1A_state); //不在充电
				
//				P1DIR &= ~0x01;
//				P10=0;
//				delay(0xff);delay(0xff);
//				P10=1;
//				delay(0xff);delay(0xff);
//				P10=0;
//				delay(0xff);delay(0xff);
//				P10=1;
//				delay(0xff);delay(0xff);
//				P10=0;
//				delay(0xff);delay(0xff);
//				P10=1;
//				delay(0xff);delay(0xff);delay(0xff);delay(0xff);
//				delay(0xff);
				if(config_state == 1)   //配置
				{
					sendmsg18();  //RF发送断开的命令  putstr(msg18_state);
					
					//ALL_LED_DOWN();///
					config_state = 0;
				}
				
				if(taskstart == 0) //若在关机时拿下
				{
					//chargeval = get_led_charge() - START_NUM  +1;
					//if(chargeval < 0)   chargeval = 0;
					
					cmd0x0f = 0;
					cmd0x10 = 1;
					cmd0x29 = 0;
					cmd0x1c = 0;
					
					led_shutdown(chargeval);
					msg1A_state[1] = 0;
					msg1A_state[2] = 0; 
					putstr(msg1A_state); //关机			
				}
				else if(taskstart == 1)  //若在开机时拿下
				{
					cmd0x0f = 1;
					cmd0x10 = 0;
					//cmd0x1c = 0;
					
					fullflag = 0; //退出充电定时
					fullflagnum = 0;
					msg1A_state[1] = 0;
					msg1A_state[2] = 1; 
					putstr(msg1A_state); //开机
				}
			}
		}
	}
	else  //>2.5v
	{
		if(connect_state_num !=0 )
		{
			//cmd0x0f = 0;
			//cmd0x10 = 1;			
			abc(); //rf发送给上位机已连接 并闪灯
			connect_state_num = 0;  //清零
		}
		
		if (charge_state == 0)
		    charge_state_num++;
		
		if(charge_state_num == 2)
		{
			charge_state_num = 0;
			
			if(charge_state == 0)
			{
				charge_state = 1;
				msg1A_state[1] = 1;
				
				cmd0x0f = 0;
			 cmd0x10 = 1;	
				
				wakepin();
				putstr(msg1A_state);   //正在充电
				
				//led_get_power((get_led_charge()-1)); //显示电量
				
				//TR0 = 0; //关闭定时器
			}			
		}
	}
}

void sendmsg18(void)  //RF发送断开的命令
{
	putstr(msg18_state); 
	/////////////////
// 	L01_Init( );	              	//Initialize the internal 24L01P
// 	L01_SetTXMode( );               //Set as TX mode
// 	L01_FlushTX( );
// 	L01_FlushRX( );
// 	rf_flag = 0;
// 	delay(1000);
// 	delay(1000);
// 	delay(1000);

// 	for(itmp=2;itmp>1;itmp--)
// 	{
// 			L01_WriteTXPayload_Ack( msg18_state, 17 );
// 			delay(1000);
// 			delay(1000);
// 			delay(1000);
// 			delay(1000);
// 			delay(1000);
// 			delay(1000);
// 	}
// 	
// 	L01_Init( );	
// 	L01_SetRXMode( );               //Set as RX mode
// 	delay(1000);
// 	delay(1000);
// 	delay(1000);
// 	L01_FlushRX( );	
// 	rf_flag = 0;	
// 	/////////////////
// 	charge_state = 0;
// 	msg1A_state[1] = 0; 
// 	putstr(msg1A_state);  //不在充电
}

void abc(void)  //
{
	INT8U j;
	
	txbuf[0] = 0x17; txbuf[1] = 0x01;
	for(j=2;j<17;j++)
		txbuf[j] = 0x00;

	L01_Init( );watchdog_load();	              	//Initialize the internal 24L01P    
	L01_SetTXMode( );               //Set as TX mode
	L01_FlushTX( );
	L01_FlushRX( );
	rf_flag = 0;
	delay(1000);
	delay(1000);
	delay(1000);watchdog_load();

	for(itmp=4;itmp>1;itmp--)
	{
			L01_WriteTXPayload_Ack( txbuf, 17 );
			delay(1000);
			delay(1000);
			delay(1000);watchdog_load();
			delay(1000);
			delay(1000);
			delay(1000);watchdog_load();
	}
	
	L01_Init( );watchdog_load();
	L01_SetRXMode( );               //Set as RX mode
	delay(1000);
	delay(1000);
	delay(1000);watchdog_load();
	L01_FlushRX( );	
	rf_flag = 0;	

	//TR0 = 0;
	led_write_finished();				
	
//	P1DIR &= ~0x01;
//	P10=0;
//	delay(0xff);
//	P10=1;
//	delay(0xff);
	msg17_state[1] = 1;
	putstr(msg17_state); 
	config_state = 1;

}



