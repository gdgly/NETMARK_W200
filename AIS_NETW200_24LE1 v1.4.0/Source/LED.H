#ifndef _LED_H_
#define _LED_H_  

void gpio_init(void); //LED
void Get_Power(void); //电量显示
void led_open(void); //开机
void led_off(void); //关机
void led_send(void); //发射
void led_gps_failed(void); //未接收到GPS
void led_activate(void); //激活
void led_charge(void);  //充电时闪灯

extern INT16U adc_charge;//电量值

////////////////////
#define LED1_ON()   P13=1
#define LED1_OFF()  P13=0
#define LED2_ON()   P14=1
#define LED2_OFF()  P14=0
#define LED3_ON()   P07=1
#define LED3_OFF()  P07=0
#define LED4_ON()   P02=1
#define LED4_OFF()  P02=0
#define LED5_ON()   P16=1
#define LED5_OFF()  P16=0
#define LED6_ON()   P00=1
#define LED6_OFF()  P00=0
#define LED7_ON()   P15=1
#define LED7_OFF()  P15=0
#define LED8_ON()   P01=1
#define LED8_OFF()  P01=0

#define POWER_GRADE_1		   1
#define POWER_GRADE_2		   2
#define POWER_GRADE_3		   3
#define POWER_GRADE_4		   4
#define POWER_GRADE_5		   5
#define POWER_GRADE_6		   6
#define POWER_GRADE_7		   7
#define POWER_GRADE_8		   8
#define POWER_GRADE_9		   9
#define POWER_GRADE_10		 10
#define POWER_GRADE_FULL	   10

void delay_ms(INT16U dj) ;		//ms级的延时
void delay_ms2(INT16U dj) ; 
void delay_ms3(INT16U dj) ; 
void delay_ms4(INT16U dj) ;
void ALL_LED_UP(void);			//所有led点亮
void ALL_LED_DOWN(void);  		//所有led熄灭
void led_round(INT8U TIMES,INT16U speed);	 //led流水灯（圈数，速度）
void all_led_flash(INT8U TIMES,INT16U up_time,INT16U down_time);	 //全部led闪烁（闪烁次数，亮起时间ms，熄灭时间ms）
char part_led_flash_open(INT8U xLED,INT8U TIMES,INT16U up_time,INT16U down_time); //开机闪烁
void part_led_flash(INT8U xLED,INT8U TIMES,INT16U up_time,INT16U down_time);	  //电量相关led闪烁（闪烁次数，亮起时间ms，熄灭时间ms）
void led_get_power(INT8U ENERGY);			   //充电时led的行为（电量1~9可以使用POWER_GRADE_1~8宏和POWER_GRADE_FULL宏）
void led_startup(INT8U ENERGY);				   //开机时led的行为（电量1~8可以使用POWER_GRADE_1~8宏）
void led_shutdown(INT8U ENERGY);			   //关机时led的行为（电量1~8可以使用POWER_GRADE_1~8宏）
void led_write_finished(void);				   //写码成功时led的行为
INT8U get_led_charge (void);   //电量检测
INT8U get_led_charging (void);  //充电时的阈值

void FOUR_LED_UP(void);
void FOUR_LED_DOWN(void);
char daynight_flash(void);
#endif


