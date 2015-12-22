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
INT16U cunt=0; //rtc����
INT16U adc_level=0;//��ƽֵ

INT16U adc_charge=0;//����ֵ
INT8U charge_state=0; //���״̬ 0:�ǳ��  1:���
INT8U config_state=0; //����״̬ 0��������״̬  1������״̬

INT8U min30reset[17] =  {0x19,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //30min reset
INT8U msg17_state[17] = {0x17,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //
INT8U msg18_state[17] = {0x18,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //
INT8U msg1A_state[17] = {0x1A,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //����/�˳����  msg1A_state[1] :  0:�ǳ��  1:���
	
INT8U flag;
//extern INT8U in0x17;
	
#define ADCGATE  2480 //3600
INT8U connect_state_num=0; //�ж������ϵ����ļ���
INT8U charge_state_num=0; //�жϳ��״̬�ļ���
	
char chargeval; //�����ʱ����
extern volatile char cmd0x0f;
extern volatile char cmd0x10;
extern volatile char cmd0x1c;
extern volatile char cmd0x29;
extern volatile char cmd0x29num;
	
extern char fullflag;
extern int fullflagnum;
	
// void rtc2_init(void)
// {
// 	RTC2CMP0=0xff;						   	// ��ʱʱ��2��
// 	RTC2CMP1=0xff;
// // 	RTC2CMP0=0xff;						   	// ��ʱʱ��1��
// // 	RTC2CMP1=0x7f;
// 	
// 	RTC2CON=0x07;							// �Ƚ�ģʽ������RTC2
// 	WUIRQ=1;								// ʹ��TICK�ж�
// }

void timer1_init(void)
{
	TMOD=0X11;
	TH1=(65536-TIM1CNT)/256;
	TL1=(65536-TIM1CNT)%256;
	ET1=1;
	TR1=1;  //��ʱ������
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
		
		watchdog_load(); //���ÿ��Ź�
	
	 	//CLKCTRL = 0x28;
	
		Is_Charge(); //�ж��Ƿ��ڳ�������   

		//Adc_AIN10();
		//adc_charge = readadc();
	  //putch((INT8U)(adc_charge >> 8)); putch((INT8U)(adc_charge));

		if(config_state == 1)   //����
		{
		}
		else if(charge_state == 1)  //���
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
			led_get_power(chargeval); //��ʾ����
			
			//TR0 = 0;
		}
		else  //�ǳ�������
		{
			//powrdownmodeset(); //����˯��
			L01_Init( );
			L01_SetRXMode( );    //Set as RX mode
			L01_FlushRX( );
			rf_flag = 0;
			
			delay_ms2(200);
			L01_SetOFFMode();			
			
			if(cmd0x29 == 1)  //ҹ����˸��ʱ
				cmd0x29num++;
// 			else if(cmd0x0f == 0)  //��ܹ��������к�Ƴ���
// 			{
// 				P0DIR |= 0XA7;       //�ر�GPIO
// 				P1DIR |= 0X7A;   
// 				
// 				P1DIR &= ~0x01;   //��P10
// 			}
			
						
		}
		
// 		if(taskstart == 1)	
// 			TR0 = 1;		
	}

	ET1 = 1;
		
}

void Is_Charge(void)  //�ж��Ƿ��ڳ��
{
	Adc_AIN6(); //��ƽ���	�����Ƿ�γ�
	adc_level = readadc(); //����ƽ
	delay_ms3(100);
	adc_level = adc_level + readadc();
	delay_ms3(100);
	adc_level = adc_level + readadc();
	adc_level = adc_level/3;
	
	if(adc_level <= CHARGEGATE)  //3100=2.5V  δ���ڵ�����
	{
		charge_state_num = 0;
		
		if(charge_state == 1)  //ֻ����һ��
		{	
			connect_state_num++;

			if(connect_state_num == 7)  //�����ж�4�β��˳����
			{
				charge_state = 0;
				connect_state_num = 0;
				
				//wakepin();
// 			msg1A_state[1] = 0; 
//				putstr(msg1A_state); //���ڳ��
				
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
				if(config_state == 1)   //����
				{
					sendmsg18();  //RF���ͶϿ�������  putstr(msg18_state);
					
					//ALL_LED_DOWN();///
					config_state = 0;
				}
				
				if(taskstart == 0) //���ڹػ�ʱ����
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
					putstr(msg1A_state); //�ػ�			
				}
				else if(taskstart == 1)  //���ڿ���ʱ����
				{
					cmd0x0f = 1;
					cmd0x10 = 0;
					//cmd0x1c = 0;
					
					fullflag = 0; //�˳���綨ʱ
					fullflagnum = 0;
					msg1A_state[1] = 0;
					msg1A_state[2] = 1; 
					putstr(msg1A_state); //����
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
			abc(); //rf���͸���λ�������� ������
			connect_state_num = 0;  //����
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
				putstr(msg1A_state);   //���ڳ��
				
				//led_get_power((get_led_charge()-1)); //��ʾ����
				
				//TR0 = 0; //�رն�ʱ��
			}			
		}
	}
}

void sendmsg18(void)  //RF���ͶϿ�������
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
// 	putstr(msg1A_state);  //���ڳ��
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



