#include "reg24le1.h"
#include "nRF24L01.h"
#include "config.h"
#include "main.h"
#include "uart.h"
#include "hal_flash.h"
#include "command.h"
#include "rtc.h"
#include "wdt.h"

extern volatile char cmd0x0f;
extern volatile char cmd0x10;
extern volatile char cmd0x29;
extern volatile char cmd0x1c;

extern INT8U charge_state;
extern INT8U config_state;

char fullflag=0; //第八盏灯定时 标记变量
int fullflagnum=0; //计数

void gpio_init(void) //LED
{
	P0CON = 0X00;
	P1CON = 0X00;
	//P0DIR &= ~0XA7;       //set p0.0 p0.1 p0.2 p0.5 p0.7 output
	//P1DIR &= ~0X7B;       //set p1.0 p1.1 p1.3 p1.4 p1.5 p1.6 output
	//P1DIR &= ~0x01;         //set p1.0 output
}

void Get_Power(void) //电量显示
{
	Adc_AIN10(); //电量检测
	adc_charge = readadc(); //检测电量

}

void led_open(void) //开机
{
	Get_Power();
}

void led_off(void) //关机
{
	Get_Power();
}

void led_send(void) //发射
{
	//最上方一对灯闪一下
}

void led_gps_failed(void) //未接收到GPS
{
	//最下方一对灯闪一下
	
}

void led_activate(void) //激活
{
	//最上方一对灯闪一下
	
}

void led_charge(void)  //充电时闪灯  空电 2.5V=3100  满电 3.06V=3800
{
	if(adc_charge <= 1000)  //空电 2.5V=3100
	{
		
	}
	else if(adc_charge > 1000 && adc_charge <= 2000)
	{
		
	}
	else if(adc_charge > 2000 && adc_charge <= 3000)
	{
		
	}
	else  //满电 3.06V=3800
	{
		
	}	
}

////////////////////////////////////////
void delay_ms(INT16U dj)
{
	INT16U di;
	for(;dj;dj--)
	{
		watchdog_load();
    for(di=1095;di;di--) ;
	}
}

void delay_ms2(INT16U dj) 
{
	INT16U di;
	for(;dj;dj--)
	{
		watchdog_load();
    for(di=1095;di;di--) ;
	}
}

void delay_ms3(INT16U dj) 
{
	INT16U di;
	for(;dj;dj--)
	{
		watchdog_load();
    for(di=1095;di;di--) ;
	}
}

void delay_ms4(INT16U dj) 
{
	INT16U di;
	for(;dj;dj--)
	{
		watchdog_load();
    for(di=1095;di;di--) ;
	}
}

void delay_ms5(INT16U dj) 
{
	INT16U di;
	for(;dj;dj--)
	{
		watchdog_load();
    for(di=1095;di;di--) ;
	}
}

// INT8U get_led_charge (void)
// {
// 	INT8U led_charges;
// 	
// 	Adc_AIN10(); 
// 	adc_charge = readadc();

// 	if( adc_charge <= 3300)  														//2.66v
// 		led_charges=POWER_GRADE_1;	
// 	if((adc_charge > 3300) && (adc_charge <= 3400))			
// 		led_charges=POWER_GRADE_2;
// 	if((adc_charge > 3400) && (adc_charge <= 3500))			
// 		led_charges=POWER_GRADE_3;
// 	if((adc_charge > 3500) && (adc_charge <= 3600))			
// 		led_charges=POWER_GRADE_4;
// 	if((adc_charge > 3600) && (adc_charge <= 3700))		 
// 		led_charges=POWER_GRADE_5;
// 	if((adc_charge > 3700) && (adc_charge <= 3800))		
// 		led_charges=POWER_GRADE_6;
// 	if((adc_charge > 3800) && (adc_charge <= 3900))		
// 		led_charges=POWER_GRADE_7;
// 	if((adc_charge > 3900) && (adc_charge <= 4080))		//8.4v满电，对应ADC数值为3909
// 		led_charges=POWER_GRADE_8;
// 	if( adc_charge > 4090) 														
// 		led_charges=POWER_GRADE_FULL;
// 	return	 led_charges;
// }

INT8U get_led_charge (void)   //非充电时的阈值
{
	INT8U led_charges;
	
	Adc_AIN10(); 
	adc_charge = readadc();
	
// 	adc_charge = readadc();  //采集三次，试图判断的更准一点
// 	delay_ms3(200);
// 	adc_charge = adc_charge + readadc();
// 	delay_ms3(200);
// 	adc_charge = adc_charge + readadc();
// 	adc_charge = adc_charge/3;
	
	//putch((INT8U)(adc_charge >> 8)); putch((INT8U)(adc_charge));
	
	if( adc_charge <= 3257)  												
		led_charges=POWER_GRADE_1;	
	if((adc_charge > 3257) && (adc_charge <= 3350))		
		led_charges=POWER_GRADE_2;
	if((adc_charge > 3350) && (adc_charge <= 3480))			//1led
		led_charges=POWER_GRADE_3;
	if((adc_charge > 3480) && (adc_charge <= 3580))			
		led_charges=POWER_GRADE_4;
	if((adc_charge > 3580) && (adc_charge <= 3680))		 
		led_charges=POWER_GRADE_5;
	if((adc_charge > 3680) && (adc_charge <= 3770))		  //4
		led_charges=POWER_GRADE_6;
	if((adc_charge > 3770) && (adc_charge <= 3830))     //5
		led_charges=POWER_GRADE_7;
	if((adc_charge > 3830) && (adc_charge <= 3910))			//6
		led_charges=POWER_GRADE_8;
	if((adc_charge > 3910) && (adc_charge <= 4000))			//7								
		led_charges=POWER_GRADE_9;
	if( adc_charge > 4000) 											        //8
		led_charges=POWER_GRADE_10;
		//led_charges=POWER_GRADE_FULL;   
	return	 led_charges;
}

INT8U get_led_charging (void)   //充电时的阈值
{
	INT8U led_charges;
	
// 	Adc_AIN10(); 
// 	adc_charge = readadc();
	
	//putch((INT8U)(adc_charge >> 8)); putch((INT8U)(adc_charge));
	
	if( adc_charge <= 3257+70)
		led_charges=POWER_GRADE_1;	
	if((adc_charge > 3257+70) && (adc_charge <= 3350+70))
		led_charges=POWER_GRADE_2;
	if((adc_charge > 3350+70) && (adc_charge <= 3480+70))			//1led
		led_charges=POWER_GRADE_3;
	if((adc_charge > 3480+70) && (adc_charge <= 3580+70))
		led_charges=POWER_GRADE_4;
	if((adc_charge > 3580+70) && (adc_charge <= 3680+70))
		led_charges=POWER_GRADE_5;
	if((adc_charge > 3680+70) && (adc_charge <= 3770+70))		  //4
		led_charges=POWER_GRADE_6;
	if((adc_charge > 3770+70) && (adc_charge <= 3830+70))     //5
		led_charges=POWER_GRADE_7;
	if((adc_charge > 3830+70) && (adc_charge <= 3910+70))			//6
		led_charges=POWER_GRADE_8;
	if((adc_charge > 3910+70) && (adc_charge <= 4000+70))			//7
		led_charges=POWER_GRADE_9;
	if( adc_charge > 4000+70) 											        //8
		led_charges=POWER_GRADE_10;	                      
		//led_charges=POWER_GRADE_FULL;   
	return	 led_charges;
}

// INT8U get_led_charge (void)  //0.355
// {
// 	INT8U led_charges;
// 	
// 	Adc_AIN10(); 
// 	adc_charge = readadc();

// 	if( adc_charge <= 3084)  														//7v
// 		led_charges=POWER_GRADE_1;	
// 	if((adc_charge > 3084) && (adc_charge <= 3260))			
// 		led_charges=POWER_GRADE_2;
// 	if((adc_charge > 3260) && (adc_charge <= 3348))			
// 		led_charges=POWER_GRADE_3;
// 	if((adc_charge > 3348) && (adc_charge <= 3436))			
// 		led_charges=POWER_GRADE_4;
// 	if((adc_charge > 3436) && (adc_charge <= 3524))		 
// 		led_charges=POWER_GRADE_5;
// 	if((adc_charge > 3524) && (adc_charge <= 3612))		
// 		led_charges=POWER_GRADE_6;
// 	if((adc_charge > 3612) && (adc_charge <= 3700))
// 		led_charges=POWER_GRADE_7;
// 	if( adc_charge > 3700) 									//8.4v满电				
// 		led_charges=POWER_GRADE_8;	
// 		//led_charges=POWER_GRADE_FULL;   
// 	return	 led_charges;
// }

void ALL_LED_UP(void)          //所有LED点亮
{
	P0DIR &= ~0XA7;       //set p0.0 p0.1 p0.2 p0.5 p0.7 output
	P1DIR &= ~0X7A; 	
	
  LED1_ON();LED2_ON();LED3_ON();LED4_ON();LED5_ON();LED6_ON();LED7_ON();LED8_ON();	 
	
// 	P0DIR |= 0XA7;       //关闭GPIO
// 	P1DIR |= 0X7A;       
}

void ALL_LED_DOWN(void)  
{
	P0DIR &= ~0XA7;       //set p0.0 p0.1 p0.2 p0.5 p0.7 output
	P1DIR &= ~0X7A; 	
	
  LED1_OFF();LED2_OFF();LED3_OFF();LED4_OFF();LED5_OFF();LED6_OFF();LED7_OFF();LED8_OFF();
	
	P0DIR |= 0XA7;       //关闭GPIO
	P1DIR |= 0X7A;       
}
void led_round(INT8U TIMES,INT16U speed)
{
 	INT8U	times=TIMES;
	for(;times;times--)
	{
		LED1_ON();		delay_ms(speed);		   LED1_OFF();
		LED2_ON();		delay_ms(speed);		   LED2_OFF();
		LED3_ON();		delay_ms(speed);		   LED3_OFF();
		LED4_ON();		delay_ms(speed);		   LED4_OFF();
		LED5_ON();		delay_ms(speed);		   LED5_OFF();			
		LED6_ON();		delay_ms(speed);		   LED6_OFF();
		LED7_ON();		delay_ms(speed);		   LED7_OFF();
		LED8_ON();		delay_ms(speed);		   LED8_OFF();
	}
}
void all_led_flash(INT8U TIMES,INT16U up_time,INT16U down_time)
{
	INT8U	times=TIMES;
	for(;times;times--)
	{
		ALL_LED_UP();		
		delay_ms4(up_time);			   
		ALL_LED_DOWN();	
		delay_ms4(down_time);
  }
}

char part_led_flash_open(INT8U xLED,INT8U TIMES,INT16U up_time,INT16U down_time)
{
  INT8U	times=TIMES;
	
	for(;times;times--)
	{
		//EA=0;		
		
		P0DIR &= ~0XA7;       //set p0.0 p0.1 p0.2 p0.5 p0.7 output
		P1DIR &= ~0X7A;
		
		if(cmd0x10 == 1)  //关机
		{	
			cmd0x10 = 0;
			cmd0x0f = 0;
			//EA=1;		
			P0DIR |= 0XA7;       //关闭GPIO
			P1DIR |= 0X7A;
			
			return 0; //break;
		}					

		/*if(cmd0x1c == 1)  //gps搜到 1:有效 0：无效
		{	
			cmd0x10 = 0;
			cmd0x0f = 0;
			//EA=1;		
			P0DIR |= 0XA7;       //关闭GPIO
			P1DIR |= 0X7A;   
			
			return 0; //break;
		}*/
		
		switch(xLED)
		{
			 case 0:
			 {
				LED1_ON();
				delay_ms(up_time);
				if(config_state == 0 || charge_state == 0 )
				{LED1_OFF();LED2_OFF();LED3_OFF();LED4_OFF();LED5_OFF();LED6_OFF();LED7_OFF();LED8_OFF();}
				delay_ms(down_time);
				break;
			 }
			 case 1:
			 {
				LED1_ON();
				delay_ms(up_time);				   
				if(config_state == 0 || charge_state == 0 )
				{LED1_OFF();LED2_OFF();LED3_OFF();LED4_OFF();LED5_OFF();LED6_OFF();LED7_OFF();LED8_OFF();}
				delay_ms(down_time);
				break;
			 }
			 case 2:
			 {
				LED1_ON();LED2_ON();
				delay_ms(up_time);				   
				if(config_state == 0 || charge_state == 0 )
				{LED1_OFF();LED2_OFF();LED3_OFF();LED4_OFF();LED5_OFF();LED6_OFF();LED7_OFF();LED8_OFF();}
				delay_ms(down_time);
				break;
			 }
			 case 3:
			 {
				LED1_ON();LED2_ON();LED3_ON();
				delay_ms(up_time);				   
				if(config_state == 0 || charge_state == 0 )
				{LED1_OFF();LED2_OFF();LED3_OFF();LED4_OFF();LED5_OFF();LED6_OFF();LED7_OFF();LED8_OFF();}
				delay_ms(down_time);
			 break;
			 }
			 case 4:
			 {
				LED1_ON();LED2_ON();LED3_ON();LED4_ON();
				delay_ms(up_time);				   
				if(config_state == 0 || charge_state == 0 )
				{LED1_OFF();LED2_OFF();LED3_OFF();LED4_OFF();LED5_OFF();LED6_OFF();LED7_OFF();LED8_OFF();}
				delay_ms(down_time);
				break;
			 }
			 case 5:
			 {
				LED1_ON();LED2_ON();LED3_ON();LED4_ON();LED5_ON();
				delay_ms(up_time);				   
				if(config_state == 0 || charge_state == 0 )
				{LED1_OFF();LED2_OFF();LED3_OFF();LED4_OFF();LED5_OFF();LED6_OFF();LED7_OFF();LED8_OFF();}
				delay_ms(down_time);
			 break;
			 }
			 case 6:
			 {
				LED1_ON();LED2_ON();LED3_ON();LED4_ON();LED5_ON();LED6_ON();
				delay_ms(up_time);				   
				if(config_state == 0 || charge_state == 0 )
				{LED1_OFF();LED2_OFF();LED3_OFF();LED4_OFF();LED5_OFF();LED6_OFF();LED7_OFF();LED8_OFF();}
				delay_ms(down_time);
			 break;
			 }
			 case 7:
			 {
				LED1_ON();LED2_ON();LED3_ON();LED4_ON();LED5_ON();LED6_ON();LED7_ON(); 
				delay_ms(up_time);				   
				if(config_state == 0 || charge_state == 0 )
				{LED1_OFF();LED2_OFF();LED3_OFF();LED4_OFF();LED5_OFF();LED6_OFF();LED7_OFF();LED8_OFF();}
				delay_ms(down_time);
			 break;
			 }
			 case 8:
			 {
				LED1_ON();LED2_ON();LED3_ON();LED4_ON();LED5_ON();LED6_ON();LED7_ON();LED8_ON();
				delay_ms(up_time);				   
				if(config_state == 0 || charge_state == 0 )
				{LED1_OFF();LED2_OFF();LED3_OFF();LED4_OFF();LED5_OFF();LED6_OFF();LED7_OFF();LED8_OFF();}
				delay_ms(down_time);
			 break;
			 }
			 case 9:
			 {
				LED1_ON();LED2_ON();LED3_ON();LED4_ON();LED5_ON();LED6_ON();LED7_ON();LED8_ON();
				delay_ms(up_time);				   
				if(config_state == 0 || charge_state == 0 )
				{LED1_OFF();LED2_OFF();LED3_OFF();LED4_OFF();LED5_OFF();LED6_OFF();LED7_OFF();LED8_OFF();}
				delay_ms(down_time);
			 break;
			 }
			 case 10:
			 {
				LED1_ON();LED2_ON();LED3_ON();LED4_ON();LED5_ON();LED6_ON();LED7_ON();LED8_ON();
				delay_ms(up_time);				   
				if(config_state == 0 || charge_state == 0 )
				{LED1_OFF();LED2_OFF();LED3_OFF();LED4_OFF();LED5_OFF();LED6_OFF();LED7_OFF();LED8_OFF();}
				delay_ms(down_time);
			 break;
			 }
		}
		
		//EA=1;
		
// 		P0DIR |= 0XA7;       //关闭GPIO
// 		P1DIR |= 0X7A;   	
		
 	}
// 	
// 	if(times == 0)
// 	{
//  		P0DIR |= 0XA7;       //关闭GPIO
//  		P1DIR |= 0X7A;   
// 	}
	return 1;
 }

void part_led_flash(INT8U xLED,INT8U TIMES,INT16U up_time,INT16U down_time)
{
  INT8U	times=TIMES;
	 
	for(;times;times--)
	{
		
	P0DIR &= ~0XA7;       //set p0.0 p0.1 p0.2 p0.5 p0.7 output
	P1DIR &= ~0X7A;
 	//EA=0;		
		
 	switch(xLED)
	{
	   case 0:
	   {
			LED1_ON();
			delay_ms2(up_time);				   ALL_LED_DOWN();		 delay_ms2(down_time);
			break;
	   }
	   case 1:
	   {
			LED1_ON();
			delay_ms2(up_time);				   ALL_LED_DOWN();		 delay_ms2(down_time);
			break;
	   }
	   case 2:
	   {
			LED1_ON();LED2_ON();
			delay_ms2(up_time);				   ALL_LED_DOWN();		 delay_ms2(down_time);
			break;
	   }
	   case 3:
	   {
			LED1_ON();LED2_ON();LED3_ON();
			delay_ms2(up_time);				   ALL_LED_DOWN();		 delay_ms2(down_time);
		 break;
	   }
	   case 4:
	   {
			LED1_ON();LED2_ON();LED3_ON();LED4_ON();
			delay_ms2(up_time);				   ALL_LED_DOWN();		 delay_ms2(down_time);
		  break;
	   }
	   case 5:
	   {
			LED1_ON();LED2_ON();LED3_ON();LED4_ON();LED5_ON();
			delay_ms2(up_time);				   ALL_LED_DOWN();		 delay_ms2(down_time);
		  break;
	   }
	   case 6:
	   {
			LED1_ON();LED2_ON();LED3_ON();LED4_ON();LED5_ON();LED6_ON();
			delay_ms2(up_time);				   ALL_LED_DOWN();		 delay_ms2(down_time);
		  break;
	   }
	   case 7:
	   {
			LED1_ON();LED2_ON();LED3_ON();LED4_ON();LED5_ON();LED6_ON();LED7_ON(); 
			delay_ms2(up_time);				   ALL_LED_DOWN();	 	 delay_ms2(down_time);
		 break;
	   }
	   case 8:
	   {
			LED1_ON();LED2_ON();LED3_ON();LED4_ON();LED5_ON();LED6_ON();LED7_ON();LED8_ON();
			delay_ms2(up_time);				   ALL_LED_DOWN();		 delay_ms2(down_time);
		 break;
	   }
	   case 9:
	   {
			LED1_ON();LED2_ON();LED3_ON();LED4_ON();LED5_ON();LED6_ON();LED7_ON();LED8_ON();
			delay_ms2(up_time);				   ALL_LED_DOWN();		 delay_ms2(down_time);
		 break;
	   }
	   case 10:
	   {
			LED1_ON();LED2_ON();LED3_ON();LED4_ON();LED5_ON();LED6_ON();LED7_ON();LED8_ON();
			delay_ms2(up_time);				   ALL_LED_DOWN();		 delay_ms2(down_time);
		 break;
	   }
	}
	//EA=1;
	P0DIR |= 0XA7;       //关闭GPIO
	P1DIR |= 0X7A;   	
	
	}

	
 }

void led_get_power(INT8U ENERGY)
{
	P0DIR &= ~0XA7;       //set p0.0 p0.1 p0.2 p0.5 p0.7 output
	P1DIR &= ~0X7A; 	
	
 	//EA=0;
	
	switch(ENERGY)
	{
		 case 0:
	   {
			 if(fullflag == 0)
			 {			 
					LED1_ON();delay_ms3(1000);LED1_OFF();
// 					delay_ms3(880);				   ALL_LED_DOWN();		 delay_ms3(600);
// 				 
// 					P0DIR |= 0XA7;       //关闭GPIO
// 					P1DIR |= 0X7A;
			 }
				break;
	   }
		 case 1:
	   {
			 if(fullflag == 0)
			 {
					LED1_ON();delay_ms3(1000);LED1_OFF();
// 					delay_ms3(880);				   ALL_LED_DOWN();		 delay_ms3(600);
// 				 
// 					P0DIR |= 0XA7;       //关闭GPIO
// 					P1DIR |= 0X7A;
			 }
				break;
	   }
		 case 2:
	   {
			 if(fullflag == 0)
			 {
				 LED1_ON(); 
				 LED2_ON();delay_ms3(1000);LED2_OFF();
// 					delay_ms3(900);				   ALL_LED_DOWN();		 delay_ms3(600);
// 				 
// 					P0DIR |= 0XA7;       //关闭GPIO
// 					P1DIR |= 0X7A;
			 }
				break;
	   }
		 case 3:
	   {
			 if(fullflag == 0)
			 {
				 LED1_ON();LED2_ON();
				 LED3_ON();delay_ms3(1000);LED3_OFF();
// 					delay_ms3(920);				   ALL_LED_DOWN();		 delay_ms3(600);
// 				 
// 					P0DIR |= 0XA7;       //关闭GPIO
// 					P1DIR |= 0X7A;
			 }
				break;
	   }
		 case 4:
	   {
			 if(fullflag == 0)
			 {
					LED1_ON();LED2_ON();LED3_ON();
					LED4_ON();delay_ms3(1000);LED4_OFF();
				 
// 					P0DIR |= 0XA7;       //关闭GPIO
// 					P1DIR |= 0X7A;
			 }
				break;
	   }
		 case 5:
	   {
			 if(fullflag == 0)
			 {
					LED1_ON();LED2_ON();LED3_ON();LED4_ON();
					LED5_ON();delay_ms3(1000);LED5_OFF();
// 					delay_ms3(960);				   ALL_LED_DOWN();		 delay_ms3(600);
// 				 
// 					P0DIR |= 0XA7;       //关闭GPIO
// 					P1DIR |= 0X7A;
			 }
				break;
	   }
		 case 6:
	   {
			 if(fullflag == 0)
			 {
					LED1_ON();LED2_ON();LED3_ON();LED4_ON();LED5_ON();
					LED6_ON();delay_ms3(1000);LED6_OFF();
// 					delay_ms3(980);				   ALL_LED_DOWN();		 delay_ms3(600);			
// 				 
// 					P0DIR |= 0XA7;       //关闭GPIO
// 					P1DIR |= 0X7A;
			 }
				break;
	   }
	   case 7:
	   {
			 if(fullflag == 0)
			 {
					LED1_ON();LED2_ON();LED3_ON();LED4_ON();LED5_ON();LED6_ON();
				  LED7_ON();delay_ms3(1000);LED7_OFF();
					//delay_ms3(1000);				   ALL_LED_DOWN();		 delay_ms3(600);
				 
// 					P0DIR |= 0XA7;       //关闭GPIO
// 					P1DIR |= 0X7A;
								
					fullflagnum++;
					if(fullflagnum >= 350)
					{
						fullflagnum = 0;
						fullflag = 1;
					}
			  }
		
				break;
	   }
	   case 8:
	   {
			 ALL_LED_UP();
			 fullflag = 1;
			 break;
	   }
	   case 9: 
	   {
			 ALL_LED_UP();
			 fullflag = 1;
			 break;
	   }
	   case 10: 
	   {
			 ALL_LED_UP();
			 fullflag = 1;
			 break;
	   }
	}
	
	if(fullflag == 1)  //第八盏灯，20分钟后亮
	{
		ALL_LED_UP();
		
	}
	
 	//EA=1;
	
// 	P0DIR |= 0XA7;       //关闭GPIO
// 	P1DIR |= 0X7A;       
}

void led_startup(INT8U ENERGY)
{
	 	//EA=0;

		part_led_flash_open(ENERGY,200,400,1000); //4
	
		//EA=1;
	
// 	P0DIR |= 0XA7;       //关闭GPIO
// 	P1DIR |= 0X7A;       
}

void led_shutdown(INT8U ENERGY)
{
	P0DIR &= ~0XA7;       //set p0.0 p0.1 p0.2 p0.5 p0.7 output
	P1DIR &= ~0X7A; 	
	
 	//EA=0;

	part_led_flash(ENERGY,8,64,34);
//  	part_led_flash(ENERGY,2,64,34);
// 	   part_led_flash(ENERGY,2,64,34);
//	   part_led_flash(ENERGY,2,32,68);
//	   part_led_flash(ENERGY,2,16,84);
//	   part_led_flash(ENERGY,2,8,92);
//	   part_led_flash(ENERGY,2,4,96);
//	   part_led_flash(ENERGY,2,2,98);
//	   part_led_flash(ENERGY,2,1,99);
	
 	//EA=1;
	
	P0DIR |= 0XA7;       //关闭GPIO
	P1DIR |= 0X7A;       
	
  fullflag = 0; //退出充电定时
	fullflagnum = 0;
}

void led_write_finished(void)
{
	P0DIR &= ~0XA7;       //set p0.0 p0.1 p0.2 p0.5 p0.7 output
	P1DIR &= ~0X7A; 	
	
 	//EA=0;
	
	all_led_flash(2,100,100);
	ALL_LED_UP();
	
 	//EA=1;
	
// 	P0DIR |= 0XA7;       //关闭GPIO
// 	P1DIR |= 0X7A;       
	
}

void FOUR_LED_UP(void)  //夜间亮4盏
{
	P0DIR &= ~0XA7;       //set p0.0 p0.1 p0.2 p0.5 p0.7 output
	P1DIR &= ~0X7A; 	
	
  LED1_ON(); LED3_ON(); LED5_ON(); LED7_ON();
	
// 	P0DIR |= 0XA7;       //关闭GPIO
// 	P1DIR |= 0X7A;       	
}

void FOUR_LED_DOWN(void)  //夜间亮4盏
{
	P0DIR &= ~0XA7;       //set p0.0 p0.1 p0.2 p0.5 p0.7 output
	P1DIR &= ~0X7A; 	
	
  //LED1_OFF();LED2_OFF();LED3_OFF();LED4_OFF();LED5_OFF();LED6_OFF();LED7_OFF();LED8_OFF();
  LED1_OFF(); LED3_OFF(); LED5_OFF(); LED7_OFF();
	
	P0DIR |= 0XA7;       //关闭GPIO
	P1DIR |= 0X7A;
}	

char daynight_flash(void)  //夜间亮灯
{
  INT8U	i;

	for(i=0;i<3;i++)
	{
// 		if(cmd0x10 == 1)
// 		{	
// 			cmd0x10 = 0;
// 			cmd0x29 = 0;
// 			return 0;
// 		}					
		
		ALL_LED_UP();
		delay_ms5(1000);
		ALL_LED_DOWN();
		delay_ms5(1000);
  }	
	
	return 1;
}
	






