#include "main.h"

extern u8 t_cnt;  //声明在exti.c

static u8 msg_tmp[512]={0};  
static u8 msg_data[512]={0};//存储高斯插值前NRZI编码的消息
static u8 TS[3]={0x55,0x55,0x55};//训练序列
static u8 FLAG=0x7e;//帧头帧尾
static u16 crc_reg=0x0000; //存储CRC校验结果
static u8 valid_cnt_m18; // 消息中断有效bit数
static u8 valid_cnt_m24A;
static u8 valid_cnt_m24B;
//static u16 valid_cnt_m21;
static u8 num_cnt = 0;   //记录发送消息18的次数。
static u16 num_slot = 0;   //时隙号 
static u16 current_slot1=0 ; //当前时隙号，随机化后赋值给
static u16 next_slot1=0; //下一个时隙号

static u16 msequence_reg=0x1001;//PN序列初值

//extern u16 t_convert;  //gps.c中声明

extern u8 TIM3_CAM1_flag; //防止反复进入
extern u8 TIM3_CAM3_flag; //防止反复进入
extern u8 TIM3_CAM5_flag; //防止反复进入

extern u16 tim3_cnt;
extern u8 swchflag;

#define CH_SW   2 //定义信道切换宏
/************************* *********************************
 * ???:msg_crc
 * ??  : ??????crc?????
 * ??  :????????ptr,????num
 * ??  : 16?CRC????
 **********************************************************/
u16 msg_crc(u8 *ptr,u8 num)
{
	u16 crc=0xffff;
	u8 i;
	u16 gx=0x1021;
	while(num--)
	{
		for(i=0x01;i!=0;i<<=1)
		{
			if((crc&0x8000)!=0)
			{
				crc<<=1;
				crc^=gx;
			}
			else
			{
				crc<<=1;
			}
			if(((*ptr)&i)!=0)
			{
				crc^=gx;
			}
		}
		ptr++;
	}
	return ~crc;
}
/**********************************************************
 * ???:bit_to_bytes_msg
 * ??  :???????bit??????crc??????????bit?????,??????msg_tmp[]?40???,
					 ??0~7????????,8~31???????,32~39???????,40~40+num1*8-1???,
					 40+num1*8~40+num1*8+15?crc???,40+num1*8+16~40+num1*8+16+7??????
 * ??  :??????????
 * ??  : ?
 * ??  :
 **********************************************************/
void bit_to_byte_msg(u8 *msg_ptr)
{	
	u8 i,num1;
	u16 crc=crc_reg;
	num1=len;	
	for(i=0;i<(num1)*8+55+24;i++)	
	{
		if(i<8)//????
			msg_tmp[i]=0;
		else if(i>=8&&i<32)//????
			msg_tmp[i]=TS[(i-8)/8]>>(7-(i-8)%8)&0x01;
		else if(i>=32&&i<40)//????
			msg_tmp[i]=FLAG>>(7-(i-32)%8)&0x01;
 		else if(i>=40&&i<40+num1*8)
			msg_tmp[i]=(msg_ptr[(i-40)/8]>>((i-40)%8))&0x01;  //??bit??,????
		else if(i>=40+num1*8&&i<40+num1*8+16)
			msg_tmp[i]=(u8)(crc>>(15-(i-40-num1*8)%16)&0x0001);
		else if(i>=40+num1*8+16&&i<40+num1*8+16+8)
			msg_tmp[i]=FLAG>>(7-(i-40-num1*8-16)%8)&0x01;
	}
}
/******************************************************
 * ???:bit_stuffing
 * ??  :?????????:??????????
 * ??  : ????20/21bytes
 * ??  : ?
 * ??  :
 ******************************************************/
u16 bit_stuffing(void)
{
	u16 i;
	u16 j,num;
	u16 cnt=len*8+16; 
	for(i=0;i<40;i++)
	{
		msg_data[i]=msg_tmp[i];
	}
	for(i=40,j=44;(i-40)<cnt;i++,j++)
	{
		while((i-40)<=3)
		{
			msg_data[i]=msg_tmp[i];
			i++;
		}
		msg_data[j]=msg_tmp[i];
		if((msg_data[j-4]==1)&&(msg_data[j-3]==1)&&(msg_data[j-2]==1)&&(msg_data[j-1]==1)&&(msg_data[j]==1))
		{
			j++;
			msg_data[j]=0;
		}			
	}
	while(i>=cnt+40&&i<cnt+40+8)
	{	
		msg_data[j]=msg_tmp[i];
		i++;
		j++;
	}
	num=j; //
	return num;
}
/******************************************************
 * ???:nrzi
 * ??  :?????????:??????????
 * ??  : ?
 * ??  : ?
 * ??  :
 ******************************************************/
void nrzi(void)
{
	u16 i;
	for(i=8;i<256;i++)
	{
		if(msg_data[i]==0)
		{
			msg_data[i]=1-msg_data[i-1];
		}
		else
		{
			msg_data[i]=msg_data[i-1];
		}
	}
}
/****************************************************************
 * 函数名：msg18_generating 
 * 描述  : 生成消息18的时隙，存储在msg18_add[]中
 * 输入  : 无 
 * 输出  : 无
 * 调用  ：
 *****************************************************************/
void msg18_generating(void)
{
	u16 i;
	msg18();
	crc_reg=msg_crc(msg,len);
	bit_to_byte_msg(msg);
	valid_cnt_m18=bit_stuffing();
	nrzi();
	for(i=0;i<256;i++)
	{
		if(i==0)
		{
			msg18_add[i]=msg_data[i+1]+msg_data[i]*2;
		}
		else if( i > 0 && i <= valid_cnt_m18)
		{
			msg18_add[i]=msg_data[i-1]*4+msg_data[i]*2+msg_data[i+1];
		}
		else
		{
			msg18_add[i]=8;
		}
	}
}
/****************************************************************
 * 函数名：msg24A_generating 
 * 描述  : 生成消息24A的时隙，存储在msg24A_add[]中
 * 输入  : 无 
 * 输出  : 无
 * 调用  ：
 *****************************************************************/
void msg24A_generating(void)
{
	u16 i;
	msg24A();
	crc_reg=msg_crc(msg,len);
	bit_to_byte_msg(msg);
	valid_cnt_m24A=bit_stuffing(); //记录有效消息bit数
	nrzi();
	for(i=0;i<256;i++)
	{
				if(i==0)
				{
					msg24A_add[i]=msg_data[i+1]+msg_data[i]*2;
				}
				else if( i>0 && i<=valid_cnt_m24A)
				{
					msg24A_add[i]=msg_data[i-1]*4+msg_data[i]*2+msg_data[i+1];
				}
				else
				{
					msg24A_add[i]=8;
				}
	}	
}
/****************************************************************
 * 函数名：msg24B_generating 
 * 描述  : 生成消息24B的时隙，存储在msg24B_add[]中
 * 输入  : 无 
 * 输出  : 无
 * 调用  ：
 *****************************************************************/
void msg24B_generating(void)
{
	u16 i;
	msg24B();
	crc_reg=msg_crc(msg,len);
	bit_to_byte_msg(msg);
	valid_cnt_m24B=bit_stuffing(); //记录有效消息bit数
	nrzi();
	for(i=0;i<256;i++)
	{
				if(i==0)
				{
					msg24B_add[i]=msg_data[i+1]+msg_data[i]*2;
				}
				else if( i>0 && i<=valid_cnt_m24B)
				{
					msg24B_add[i]=msg_data[i-1]*4+msg_data[i]*2+msg_data[i+1];
				}
				else
				{
					msg24B_add[i]=8;
				}
	}	
}

/****************************************************************
 * 函数名：msg_send 
 * 描述  : 将消息18、24A/B对应的数据写入msg18_24AB_add[]，
					 根据flag_m24的状态只写入消息18，或者三个消息都写入。
 * 输入  : 无 
 * 输出  : 无
 * 调用  ：
 *****************************************************************/
void msg_send()   
{
		u16 i=0,j=0,k=0;
		if(flag_m24==on)
		{
			var_m24=512*5-1;        //消息24B只生成，没有进行发射
			//var_m24=256*3*5-1;    //发射24B消息
 		  msg24A_generating();
			msg18_generating();
 			//msg24B_generating();	
			for(i=0;i<768;i++)
			{
				j=i%256;
				k=i/256;
				switch (k)
				{
					case 0:
							if(j<valid_cnt_m18)
							{
								msg18_24AB_add[i]=msg18_add[j];
							}
							else
							{
								msg18_24AB_add[i]=16;
							}
						break;
					case 1:
							if(j<valid_cnt_m24A)
							{
								msg18_24AB_add[i]=msg24A_add[j];
							}
							else
							{
								msg18_24AB_add[i]=16;
							}
						break;
					case 2:
							if(j<valid_cnt_m24B)
							{
								msg18_24AB_add[i]=msg24B_add[j];
							}
							else
							{
								msg18_24AB_add[i]=16;
							}
						break;
					
// 					case 1:
// 							if(j<valid_cnt_m18)
// 							{
// 								msg18_24AB_add[i]=msg18_add[j];
// 							}
// 							else
// 							{
// 								msg18_24AB_add[i]=16;
// 							}
// 						break;
// 					case 0:
// 							if(j<valid_cnt_m24A)
// 							{
// 								msg18_24AB_add[i]=msg24A_add[j];
// 							}
// 							else
// 							{
// 								msg18_24AB_add[i]=16;
// 							}
// 						break;
// 					case 2:
// 							if(j<valid_cnt_m24B)
// 							{
// 								msg18_24AB_add[i]=msg24B_add[j];
// 							}
// 							else
// 							{
// 								msg18_24AB_add[i]=16;
// 							}
// 						break;
					default:
						break;
				}		
			}
		}
		else
		{
			var_m24=256*5-1;
			//msg18_generating();
			msg24A_generating();
			for(i=0;i<256;i++)
			{
				if(i<valid_cnt_m24A) 
				{
					msg18_24AB_add[i]=msg18_add[i];
				}
				else
				{
					msg18_24AB_add[i]=16;
				}
			}
		}
}
void msg_send1()   
{
		u16 i=0,j=0,k=0;
		if(flag_m24==on)
		{
			var_m24=256*5-1;        //消息24B只生成，没有进行发射
			//var_m24=256*3*5-1;    //发射24B消息
			msg18_generating();
 		  //msg24A_generating();
 			//msg24B_generating();	
			for(i=0;i<256;i++)
			{
				j=i%256;
				k=i/256;
				switch (k)
				{
					case 0:
							if(j<valid_cnt_m18)
							{
								msg18_24AB_add[i]=msg18_add[j];
							}
							else
							{
								msg18_24AB_add[i]=16;
							}
						break;
					case 1:
							if(j<valid_cnt_m24A)
							{
								msg18_24AB_add[i]=msg24A_add[j];
							}
							else
							{
								msg18_24AB_add[i]=16;
							}
						break;
					case 2:
							if(j<valid_cnt_m24B)
							{
								msg18_24AB_add[i]=msg24B_add[j];
							}
							else
							{
								msg18_24AB_add[i]=16;
							}
						break;
					default:
						break;
				}		
			}
		}
		else
		{
			var_m24=256*5-1;
			msg18_generating();
			//msg24A_generating();
			for(i=0;i<256;i++)
			{
				if(i<valid_cnt_m18) 
				{
					msg18_24AB_add[i]=msg18_add[i];
				}
				else
				{
					msg18_24AB_add[i]=16;
				}
			}
		}
}

void msg_send2()   
{
		u16 i=0,j=0,k=0;
		if(flag_m24==on)
		{
			var_m24=512*5-1;        //消息24B只生成，没有进行发射
			//var_m24=256*3*5-1;    //发射24B消息
			//msg18_generating();
 		 msg24A_generating();
 			msg24B_generating();	
			for(i=0;i<512;i++)
			{
				j=i%256;
				k=i/256;
				switch (k)
				{
//					case 0:
//							if(j<valid_cnt_m18)
//							{
//								msg18_24AB_add[i]=msg18_add[j];
//							}
//							else
//							{
//								msg18_24AB_add[i]=16;
//							}
//						break;
					case 0:
							if(j<valid_cnt_m24A)
							{
								msg18_24AB_add[i]=msg24A_add[j];
							}
							else
							{
								msg18_24AB_add[i]=16;
							}
						break;
					case 1:
							if(j<valid_cnt_m24B)
							{
								msg18_24AB_add[i]=msg24B_add[j];
							}
							else
							{
								msg18_24AB_add[i]=16;
							}
						break;
					default:
						break;
				}		
			}
		}
		else
		{
			var_m24=256*5-1;
			//msg18_generating();
			msg24A_generating();
			for(i=0;i<256;i++)
			{
				if(i<valid_cnt_m24A) 
				{
					msg18_24AB_add[i]=msg24A_add[i];
				}
				else
				{
					msg18_24AB_add[i]=16;
				}
			}
		}
}
/****************************************************************
 * 函数名：msequence 
 * 描述  : 随机数生成
 * 输入  : 无 
 * 输出  : 16位随机数
 * 调用  ：
 *****************************************************************/
u16 msequence(void)
{
	u8 j;
	u16 tmp1,tmp2;
	tmp2=msequence_poly&msequence_reg;
	for(j=0,tmp1=0;j<=15;j++)
	{
		tmp1+=(u16)((tmp2>>j)&0x0001);
	}
	tmp1=tmp1%2;
	msequence_reg<<=1;
	msequence_reg=msequence_reg+tmp1;
	return (msequence_reg);
}
/****************************************************************
 * 函数名：slot_random
 * 描述  : 时隙随机化
 * 输入  : 无 
 * 输出  : 无
 * 调用  ：
 *****************************************************************/
void slot_random(void)
{
	u16 temp=msequence();	
	slot_increacement=(0x00ff&temp)+interval_s-128;

	if(flag_initial_slot==on)
	{	
		current_slot1=(Get_RTC_Counter()+375)%SEND_INTERVAL; //10秒，第一次发射间隔
		next_slot1=(current_slot1+slot_increacement)%SEND_INTERVAL;
		num_slot=current_slot1;
		
		TIM3_CAM1=(num_slot-2)%SEND_INTERVAL;    //功放提前打开的时间
		TIM3_CAM2=num_slot;
		TIM3_CAM3=(num_slot-32)%SEND_INTERVAL; //提前32时隙打开PLL
		TIM3_CAM4=(num_slot+2)%SEND_INTERVAL;
		TIM3_CAM5=(num_slot-14)%SEND_INTERVAL;  //提前14时隙配置PLL
		flag_initial_slot=off;	
	}
	else
	{
		current_slot1=next_slot1;           //功放提前打开的时间
		next_slot1=(current_slot1+slot_increacement)%SEND_INTERVAL;
		num_slot=current_slot1;
		
		TIM3_CAM1=(num_slot-2)%SEND_INTERVAL;
		TIM3_CAM2=num_slot;
		TIM3_CAM3=(num_slot-32)%SEND_INTERVAL; //提前32时隙打开PLL
		TIM3_CAM4=(num_slot+2)%SEND_INTERVAL;
		TIM3_CAM5=(num_slot-14)%SEND_INTERVAL;  //提前14时隙配置PLL
	}
}
/****************************************************************
 * 函数名：fre_check_on
 * 描述  : 开启频偏矫正
 * 输入  : 无 
 * 输出  : 无
 * 调用  ：
 *****************************************************************/
void fre_check_on(void)
{
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_10); //dds on
	Delay(1000);
	//dds_wr_parrel(0x0f00,0x7600,0x3d00,0xd800);//写入单频信号9.059696MHz
	flag_fre_check=on;
	t_cnt=0;
}
/****************************************************************
 * 函数名：fre_check_off
 * 描述  : 关闭频偏矫正
 * 输入  : 无 
 * 输出  : 无
 * 调用  ：
 *****************************************************************/
void fre_check_off(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_10); //dds off
	flag_fre_check=off;
}
/****************************************************************
 * 函数名：freq_value_update
 * 描述  : 频偏矫正
 * 输入  : 无 
 * 输出  : 无
 * 调用  ：
 *****************************************************************/
void freq_value_update(void)
{
//	u8 i,j;
//	u32 val_check=((unsigned long)fre_cnt) << 9;  //移位数是根据dds输出频率计算出来的
// 	if(freq_pn_flag==on) //频偏为正，在基准频率字上减去矫正值
// 	{
// 		for(i=0;i<16;i++)
// 		{
// 			for(j=0;j<5;j++)
// 				dds_word_update[i][j].dds_data32=dds_word_real[i][j]-val_check;
// 		}
// 	}
// 	else
// 	{
// 		for(i=0;i<16;i++)
// 		{
// 			for(j=0;j<5;j++)
// 				dds_word_update[i][j].dds_data32=dds_word_real[i][j]+val_check;
// 		}
// 	}
// 	for(i=0;i<16;i++)//更新频率字查找表
// 	{
// 		for(j=0;j<5;j++)
// 			dds_w1[i][j] = ( (u16)dds_word_update[i][j].dds_data8[3] ) << 8 ;
// 	}
// 	
// 	for(i=0;i<16;i++)
// 	{
// 		for(j=0;j<5;j++)
// 			dds_w2[i][j] = ( (u16)dds_word_update[i][j].dds_data8[2] ) << 8 ;
// 	}
// 	
// 	for(i=0;i<16;i++)
// 	{
// 		for(j=0;j<5;j++)
// 			dds_w3[i][j] = ( (u16)dds_word_update[i][j].dds_data8[1] ) << 8 ;
// 	}
// 	
// 	for(i=0;i<16;i++)
// 	{
// 		for(j=0;j<5;j++)
// 			dds_w4[i][j] = ( (u16)dds_word_update[i][j].dds_data8[0] ) << 8 ;
// 	}
	
}
/****************************************************************
 * 函数名：send_on
 * 描述  : AIS发射函数，完成消息生成，分配时隙，定时器开启，等待发射。
 * 输入  : 无 
 * 输出  : 无
 * 调用  ：
 *****************************************************************/
void send_on(void)
{
//	u8 i;
	
	num_cnt++;//记录执行函数的次数
	if(num_slot>=SEND_INTERVAL)//防止发射时隙溢出
	{
		num_slot=SEND_INTERVAL/2;
	}
	
	if(task_flag2==on)	//发射条件：上次任务完成，GPS信息有效
	//if((task_flag2==on) && (flag_gps_valid==on))	//发射条件：上次任务完成，GPS信息有效
	{			
// 		for(i=0;i<=8;i++)//LED闪烁指示
// 		{
// 			GPIO_WriteBit(GPIOC, GPIO_Pin_13,	(BitAction)(1-GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13)) );   //测试用蓝色LED
// 			Delay(300000);
// 		}
		LedDelay();
			
		TIM3_OFF();	//消息生成前关闭定时器T3

		//slot_random();     //时隙随机化，产生发射时隙号。
		
// 		if((num_cnt%CH_SW)==0)// 信道切换,没发射两次AIS消息切换信道
// 		{
//  			flag_channel = (1-flag_channel); //flag_channel=(AIS_CHANNEL)(1-flag_channel);
// 			Write_TX_Channel();
// 		}
		
		swchflag = 0;
		task_flag1=off;
		task_flag2=off;

		gps_off_cnt=0;
		
		TIM3_CAM5_flag = 0;  //TIM3标记变量清零
		TIM3_CAM3_flag= 0;
		TIM3_CAM1_flag= 0;
		tim3_cnt=0;
		//RTC_Init();     //初始化RTC时钟
		TIM3_Configuration();  //打开定时器T3
		
	}
}









