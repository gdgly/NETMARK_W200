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

static ais_status str_cmp(INT8U* str_src , INT8U* str_dst , INT8U num)  //�Ƚ����������Ƿ����  :�жϴ���Կ��
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
static ais_status str_cmp_flag;//�����Ƿ���Ƚ����on��־��ͬ��off��־��ͬ��
static INT8U *ptr_cmp1;  //���ڱȽ���λ�Ǵ�����Կ�״�����ָ��
static INT8U *ptr_cmp2;  //���ڱȽϴ�����ָ��
static INT8U cnt_cmp; //���ڱȽϴ����ļ���ֵ

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

void wakepin(void) //���ѹܽź���
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
	
void Command(void)  //RF���պ��ɴ��ڷ���
{
	switch(rxbuf[0])
	{		
		case 0x01://����λ��д����
 		if(config_state == 1)
 		{
			//wakepin();
			
			for(i=0;i<16;i++)
				boatnum[i] = rxbuf[i+1];
			
			WriteflashBoatnum();
			
			putstr(rxbuf);
		}
		break;
		case 0x02://����λ��дMMSI��
		if(config_state == 1)
 		{
			//wakepin();
			putstr(rxbuf);
			}
		break;
		case 0x04://����λ�Ƕ�ȡ�����豸����
		if(config_state == 1)
 		{
			//wakepin();
			putstr(rxbuf);
		}
		break;
		case 0x05://����λ�Ƕ�ȡMMSI��
		if(config_state == 1)
 		{
			//wakepin();
			putstr(rxbuf);
		}
		break;
		case 0x0A://����λ�Ǽ���
		if(config_state == 1)
 		{
			//wakepin();
			putstr(rxbuf);
		}
		break;
		case 0x0B://����λ�ǽ���
		if(config_state == 1)
 		{
			//wakepin();
			putstr(rxbuf);
		}
		break;
		case 0x0C://�ָ��������ã��������
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
		case 0x0f://Կ�׿���!
							if(config_state == 0 && charge_state == 0)
								{
											ptr_cmp1=&rxbuf[2];  //Կ�״�����һλָ��
											ptr_cmp2=&boatnum[1];     //��λ�Ǵ�����һλָ��
											//if(boatnum[boatnum[0]/6-1]=='-')  //�ж����Ÿ��� 1λ
											//{
												if(rxbuf[1]==boatnum[0]-6)
												{	
													wakepin();
													cnt_cmp=(boatnum[0]-6)/8;
													str_cmp_flag=str_cmp(ptr_cmp1,ptr_cmp2,cnt_cmp);
													
													if(str_cmp_flag==on)     //�����ж�һ��
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
													
													if(str_cmp_flag==on)     //�����ж�һ��
													{
														wakepin();
														putstr(rxbuf);
														
														cmd0x0f = 1;
													}
												}
								//}
							}
							break;
		case 0x10://Կ�׹ر�!
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
		  	ptr_cmp1=&rxbuf[2];  //Կ�״�����һλָ��
			  ptr_cmp2=&boatnum[1];     //��λ�Ǵ�����һλָ��
		  	if(rxbuf[1]==boatnum[0]-6)
	  		{
			   	cnt_cmp=(boatnum[0]-6)/8;
			   	str_cmp_flag=str_cmp(ptr_cmp1,ptr_cmp2,cnt_cmp);
				
			   	if(str_cmp_flag==on)     //�����ж�һ��
			   	{
			    	 wakepin();
				    	putstr(rxbuf);
				   }
			  }
			  else if(rxbuf[1]==boatnum[0]-12)
		  	{
				   cnt_cmp=(boatnum[0]-12)/8;
			   	str_cmp_flag=str_cmp(ptr_cmp1,ptr_cmp2,cnt_cmp);
				
				   if(str_cmp_flag==on)     //�����ж�һ��
				   {
				    	wakepin();
				    	putstr(rxbuf);
				   }
			  }
 	}
		break;
		case 0x11://д��pn�����ɶ���ʽ
		if(config_state == 1)
 		{
			//wakepin();
			putstr(rxbuf);
		}
		break;
		case 0x12://����������
		if(config_state == 1)
 		{
			WriteflashInterval();
			
			//wakepin();
			putstr(rxbuf);
		}
		break;
		case 0x16://ID����
			//wakepin();
			putstr(rxbuf);
		break;
		case 0x17://��λ������ ��λ��!
			
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
		case 0x18://��λ�ǶϿ�!
 		if(config_state == 1)
 		{		
			//wakepin();
			delay(0xff);
		
			putstr(rxbuf);
			ALL_LED_DOWN();
		
			config_state = 0;
		}
		break;
		case 0x19:	//�ߴ�λ�òο���ע��
		if(config_state == 1)
 		{
			//wakepin();
			putstr(rxbuf);
		}
		break;
		case 0x20:	//�ߴ�λ�òο����ȡ
		if(config_state == 1)
 		{
			//wakepin();
			putstr(rxbuf);
		}
		break;
		case 0x21://���Ӷ�λװ������ ע��
		if(config_state == 1)
 		{
			//wakepin();
			putstr(rxbuf);
		}
		break;
		case 0x22://���Ӷ�λװ������ ��ȡ
		if(config_state == 1)
 		{
			//wakepin();
			putstr(rxbuf);
		}
		break;
		case 0x23://�����豸���� ע��
		if(config_state == 1)
 		{
			//wakepin();
			putstr(rxbuf);
		}
		break;
		case 0x25://�����豸���� ��ȡ
		if(config_state == 1)
 		{
			//wakepin();
			putstr(rxbuf);
		}
		break;
		case 0x26: //����ʱ����GPS����
		if(config_state == 1)
 		{
			//wakepin();
			putstr(rxbuf);
		}
		break;
		case 0x27://Կ�� ʡ��
		if(config_state == 0 && charge_state == 0)
 		{		
			ptr_cmp1=&rxbuf[2];  //Կ�״�����һλָ��
			ptr_cmp2=&boatnum[1];     //��λ�Ǵ�����һλָ��
			if(rxbuf[1]==boatnum[0]-12)
			{
				cnt_cmp=(boatnum[0]-12)/8;
				str_cmp_flag=str_cmp(ptr_cmp1,ptr_cmp2,cnt_cmp);
				
				if(str_cmp_flag==on)     //�����ж�һ��
				{
//RF off
					RFCON = 0x00;
					RFCE = 0;
					RFCKEN = 0;
					RF = 0;
					
					ALL_LED_DOWN();
					P0DIR |= 0XA7;       //�ر�GPIO
					P1DIR |= 0X7A;
					
					CLKCTRL = 0x10;
					CLKLFCTRL = 0x01;
				}
			}
		}
		break;
		case 0x2A://����������ѡ��
		if(config_state == 1)
 		{
			//wakepin();
			putstr(rxbuf);
		}
		break;
		case 0x2B://���ߴ�����
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

void Command2(void)  //���ڽ��պ������߷��ͻ�ִ��
{
	switch(com_buf[1])
	{			
		case 0x01://����λ��д����ack
			usart2rf();
			led_write_finished();
		break;
		case 0x02://����λ��дMMSI��ack
			usart2rf();
			led_write_finished();
		break;
		case 0x04://����λ�Ƕ�ȡ����ack
			usart2rf();
			led_write_finished();
		break;
		case 0x05://����λ�Ƕ�ȡMMSI��ack
			usart2rf();
			led_write_finished();
		break;
		case 0x0A://����λ�Ǽ���ack
			if(com_buf[2] == 0x01)
			{
				codeflag = 1;
				WriteflashCode();
			}
		
			usart2rf();
			led_write_finished();
		break;
		case 0x0B://����λ�ǽ���ack
			if(com_buf[2] != 0x01)
			{
				codeflag = 0;
				WriteflashCode();
			}
			
			usart2rf();
			led_write_finished();
		break;
		case 0x0C://�ָ��������ã��������ack
			usart2rf();
			led_write_finished();
		break;
		case 0x0f://Կ�׿���
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
		case 0x10://Կ�׹ر�
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
		case 0x11://д��pn�����ɶ���ʽack
			usart2rf();
		break;
		case 0x12://����������ack
			usart2rf();
			led_write_finished();
		break;
		case 0x16://ID���� led
			//led_activate();
			led_write_finished();
			ALL_LED_DOWN();
		break;
		case 0x17://��λ������
// 			usart2rf();//
// 			TR0 = 0;
// 			led_write_finished();
		break;
		case 0x18://��λ�ǶϿ�
			//usart2rf();//
			//TR0 = 0;
			//led_write_finished();
// 			config_state = 0;  //�˳�����ģʽ
// 			charge_state = 1;
		break;
		case 0x19://�ߴ�λ�òο���ע��
			usart2rf();
			led_write_finished();
		break;
		case 0x1C://GPS��Ч��־
			cmd0x1c = com_buf[2];
		break;
		case 0x20://�ߴ�λ�òο����ȡ
			usart2rf();
			led_write_finished();
		break;
		case 0x21://���Ӷ�λװ������ ע��
			usart2rf();
			led_write_finished();
		break;
		case 0x22://���Ӷ�λװ������ ��ȡ
			usart2rf();
			led_write_finished();
		break;
		case 0x23://�����豸���� ע��
			usart2rf();
			led_write_finished();
		break;
		case 0x25://�����豸���� ��ȡ
			usart2rf();
			led_write_finished();
		break;
		case 0x29://ҹ������
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
		case 0x2A://����������ѡ��
			usart2rf();
			led_write_finished();
		break;
		case 0x2B://���ߴ�����
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
	
