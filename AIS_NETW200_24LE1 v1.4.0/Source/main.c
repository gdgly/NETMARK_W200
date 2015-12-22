#include "reg24le1.h"
#include "nRF24L01.h"
#include "config.h"
#include "main.h"
#include "uart.h"
#include "hal_flash.h"
#include "command.h"
#include "rtc.h"
#include "timer.h"

INT8U txbuf1[30] = {0x00};
INT8U interval_s;
INT8U boatnum[16];
INT8U codeflag; //�Ƿ���ܵı���
volatile INT8U taskstart=1; //���ػ�״̬����������Ϊ1���ػ�Ϊ0
INT8U msg0f_state[17] = {0x0f,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //��������

static INT8U i;

void rtc2_init(void);
extern char chargeval;
volatile char cmd0x0f=0;
volatile char cmd0x10=0;
volatile char cmd0x29=0;
volatile char cmd0x1c=0;
volatile char cmd0x29num=0;
	
void delay(unsigned int dj)
{
	unsigned char di;
	for(;dj>0;dj--)
   for(di=120;di>0;di--);
	
	//watchdog_load();
}

void main( void )
{
	EA = 0;  

	CLKCTRL = 0x28;  
	CLKLFCTRL =  0x01;  //32k���� for RTC
	gpio_init();  //led	
	ALL_LED_DOWN();
	
	uart_init();
	//rtc2_init();
	RF_config();
	//timer_init();
	timer1_init();
	Adc_AIN6(); //��ƽ���
	radmcof(); //���������
	
	FlashInit(); //flash����
	
// 	L01_Init( );	       //����ģ���ʼ��
// 	L01_SetRXMode( );    //Set as RX mode
//	L01_SetOFFMode();
// 	L01_FlushTX( );
// 	L01_FlushRX( );
//  	rf_flag = 0;

	//P10=1;
	delay(0xff);
 	//retentionpinset();		//���� RFrx&RTC ˯��
 	EA = 1; //���жϿ�
	
	watchdog_load(); //���ÿ��Ź�
	//powrdownmodeset(); //����˯��
	
	
//	TR0 = 1;
	taskstart = 1;
	
	while(1)
	{
		if(cmd0x0f == 1)  //������˸
		{
			cmd0x10 = 0;
			
// 			P0DIR &= ~0XA7;       //set p0.0 p0.1 p0.2 p0.5 p0.7 output
// 			P1DIR &= ~0X7A;
// 			led_round(1,200);
			
			taskstart = 1;
//			TR0 = 1;
			
			delay_ms(1000);
			chargeval = get_led_charge() - START_NUM +1;
			if(chargeval < 0)   chargeval = 0;
			led_startup(chargeval);
			
			cmd0x0f = 0;
		}
		else if(taskstart == 1 && cmd0x29 == 1 && cmd0x29num == 22) //ҹ����˸  //else if(taskstart == 1 && cmd0x29 == 1 && cmd0x29num == 22) //
		{
			daynight_flash();
			
			cmd0x29num = 0;
		}
		
	}
}









