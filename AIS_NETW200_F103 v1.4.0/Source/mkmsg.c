#include "main.h"

/***********************************************************
 * 函数名msg21
 * 描述  ：生成消息21
 * 输入  : 无
 * 输出  : 无
 ***********************************************************/
void msg21(void)
{	
	u8 i;
	
// 	jingdu=GPS.longitude; //更新经纬度信息
// 	weidu=GPS.latitude;
	Read_GPS_Info();
	
	for(i=0;i<45;i++)  msg[i] = 0;
	
	msg[ 0] = 0x54;	         //6bit消息ID:21;  2bit转发指示符：0
	msg[ 1] = (MMSI >> 22);	     //30bit用户ID，MMSI编号
	msg[ 2] = (MMSI >> 14);
	msg[ 3] = (MMSI >>  6);
	msg[ 4] = (MMSI <<  2)        + (dev_type     >> 3);  //低2bit为 助航设备类型的高2bit
	msg[ 5] = (dev_type     << 5) + (dev_name[ 1] >> 3);  //高3bit为助航设备类型，助航设备名称。。。
	msg[ 6] = (dev_name[ 1] << 5) + (dev_name[ 2] >> 3);
	msg[ 7] = (dev_name[ 2] << 5) + (dev_name[ 3] >> 3);
	msg[ 8] = (dev_name[ 3] << 5) + (dev_name[ 4] >> 3);
	msg[ 9] = (dev_name[ 4] << 5) + (dev_name[ 5] >> 3);
	msg[10] = (dev_name[ 5] << 5) + (dev_name[ 6] >> 3);
	msg[11] = (dev_name[ 6] << 5) + (dev_name[ 7] >> 3);
	msg[12] = (dev_name[ 7] << 5) + (dev_name[ 8] >> 3);
	msg[13] = (dev_name[ 8] << 5) + (dev_name[ 9] >> 3);
	msg[14] = (dev_name[ 9] << 5) + (dev_name[10] >> 3);
	msg[15] = (dev_name[10] << 5) + (dev_name[11] >> 3);
	msg[16] = (dev_name[11] << 5) + (dev_name[12] >> 3);
	msg[17] = (dev_name[12] << 5) + (dev_name[13] >> 3);
	msg[18] = (dev_name[13] << 5) + (dev_name[14] >> 3);
	msg[19] = (dev_name[14] << 5) + (dev_name[15] >> 3);
	msg[20] = (dev_name[15] << 5) + (jingdu >> 24); //+位置准确度：0 bit4; + 经度
	msg[21] = (jingdu >> 16);
	msg[22] = (jingdu >>  8);
	msg[23] = (jingdu      );
	msg[24] = (weidu  >> 19);
	msg[25] = (weidu  >> 11);
	msg[26] = (weidu  >>  3);
	msg[27] = (weidu  <<  5)      + (dev_local[0] >> 1); //尺寸位置参考点
	msg[28] = (dev_local[0] << 7) + (dev_local[1] << 1) + (dev_local[2] >> 5);
	msg[29] = (dev_local[2] << 3) + (dev_local[3] >> 3);
	msg[30] = (dev_local[3] << 5) + (dev_local[4] >> 1); 
	msg[31] = (dev_local[4] << 7) + (dev_time     >> 3) + (dev_etype    << 5); //+电子定位装置类型
	msg[32] = (dev_time     << 5) + (dev_state    >> 4) + 0x10 ; //偏置位置指示符：1；
	msg[33] = (dev_state    << 4) + 0; //RAIM标志:0 ；+ 虚拟AtoN标志；+指配模式标志;+备用
	len=34;
	switch((dev_name[0])/6) //助航设备名称扩展
	{
		case 16:
				msg[34] = dev_name[16];
				msg[34]&= ~0x03;
				len=35;
		break;
		
		case 17:
				msg[34] = dev_name[16];
				msg[35] = dev_name[17];
				msg[33]&= ~0x0f;				
				len=36;
		break;
		
		case 18:
				msg[34] = dev_name[16];
				msg[35] = dev_name[17];
				msg[36] = dev_name[18];
				len=37;
		break;
		
		case 19:
				msg[34] = dev_name[16];
				msg[35] = dev_name[17];
				msg[36] = dev_name[18];
				msg[37] = dev_name[19];
				msg[37]&= ~0x03;
				len=38;
		break;
		
		case 20:
				msg[34] = dev_name[16];
				msg[35] = dev_name[17];
				msg[36] = dev_name[18];
				msg[37] = dev_name[19];
				msg[38] = dev_name[20];
				msg[33]&= ~0x0f;	
				len=39;
		break;
		
		case 21:
				msg[34] = dev_name[16];
				msg[35] = dev_name[17];
				msg[36] = dev_name[18];
				msg[37] = dev_name[19];
				msg[38] = dev_name[20];
				msg[39] = dev_name[21];
				len=40;
		break;
		
		case 22:
				msg[34] = dev_name[16];
				msg[35] = dev_name[17];
				msg[36] = dev_name[18];
				msg[37] = dev_name[19];
				msg[38] = dev_name[20];
				msg[39] = dev_name[21];
				msg[40] = dev_name[22];
				msg[40]&= ~0x03;
				len=41;
		break;
		
		case 23:
				msg[34] = dev_name[16];
				msg[35] = dev_name[17];
				msg[36] = dev_name[18];
				msg[37] = dev_name[19];
				msg[38] = dev_name[20];
				msg[39] = dev_name[21];
				msg[40] = dev_name[22];
				msg[41] = dev_name[23];
				msg[33]&= ~0x0f;	
				len=42;
		break;
		
		case 24:
				msg[34] = dev_name[16];
				msg[35] = dev_name[17];
				msg[36] = dev_name[18];
				msg[37] = dev_name[19];
				msg[38] = dev_name[20];
				msg[39] = dev_name[21];
				msg[40] = dev_name[22];
				msg[41] = dev_name[23];
				msg[42] = dev_name[24];
				len=43;
		break;
		
		case 25:
				msg[34] = dev_name[16];
				msg[35] = dev_name[17];
				msg[36] = dev_name[18];
				msg[37] = dev_name[19];
				msg[38] = dev_name[20];
				msg[39] = dev_name[21];
				msg[40] = dev_name[22];
				msg[41] = dev_name[23];
				msg[42] = dev_name[24];
				msg[43] = dev_name[25];
				msg[43]&= ~0x03;				
				len=44;
		break;
		
		case 26:
				msg[34] = dev_name[16];
				msg[35] = dev_name[17];
				msg[36] = dev_name[18];
				msg[37] = dev_name[19];
				msg[38] = dev_name[20];
				msg[39] = dev_name[21];
				msg[40] = dev_name[22];
				msg[41] = dev_name[23];
				msg[42] = dev_name[24];
				msg[43] = dev_name[25];
				msg[44] = dev_name[26];	
				msg[33]&= ~0x0f;	
				len=45;
		break;
		
		default:
		break;
	}


}
/***********************************************************
 * 函数名msg18
 * 描述  ：生成消息18
 * 输入  : 无
 * 输出  : 无
 ***********************************************************/
void msg18(void)
{	
// 	jingdu=GPS.longitude; //更新经纬度信息
// 	weidu=GPS.latitude;
	
	Read_GPS_Info();
	
	msg[0] =0x48 ;	         //6bit消息ID:18;  2bit转发指示符：0
	msg[1] = MMSI >>22;	     //30bit用户ID，MMSI编号
	msg[2] = MMSI >>14;
	msg[3] = MMSI >> 6;
	msg[4] = MMSI << 2;  //低2bit备用为0
	msg[5] = sog >> 8;  //高6bit备用为0，低2bit地面航速为0
	msg[6] = sog;			 //地面航速为0
	msg[7] = (jingdu >> 21)| 0x80; //最高位为位置准确度1，，低7位开始为经度的最高位
	msg[8] = (jingdu) >> 13;   
	msg[9] = (jingdu) >> 5;
	msg[10] = ((jingdu << 3) & 0xF8 )+ ((weidu>>24 )& 0x07); //经度数据低5位，纬度信息高3bit.
	msg[11] = (weidu) >> 16; 
	msg[12] = (weidu) >> 8;
	msg[13] = (weidu) >> 0;
	msg[14] = (u8)(direction >> 4); //0xE1;	   //COG:3600 高8bit
	msg[15] = (u8)(direction << 4); //0x40 + (u8)(direction >> 5);		   //COG高4位 + 实际航向高4位 实际航向设置为全0
	msg[16] = 0x07; //(u8)(direction << 3) + 0x07;		   //实际航向的低5位+时间戳高3bit.实际航向和时间戳都不可用
	msg[17] = 0x84;	   //时间戳低3bit:100;备用2bit:00;B类装置标志1bit:1;B类显示器标志1bit:0;B类DSC标志: 0  
	msg[18] = 0x08+((slot_increacement>>12)&0x01); // B类带宽标志： 0；通信状态高3bit:000  ITDMA
	msg[19]=slot_increacement>>4;
	msg[20]=(slot_increacement<<4)+0x04;

	len=21;
}
/***********************************************************
 * 函数名msg24A
 * 描述  ：生成消息24A
 * 输入  : 无
 * 输出  : 无
 ***********************************************************/
void msg24A(void)
{
	u8 i;
	msg[0] =0x60 ;	         //6bit消息ID:24;  2bit转发指示符：00
	msg[1] = MMSI >>22;	     //30bit用户ID，MMSI编号
	msg[2] = MMSI >>14 ;
	msg[3] = MMSI >> 6;
	msg[4] = MMSI << 2;  //低2bit为部分编号：00
	for(i=0;i<15;i++)   
		msg[5+i] = boatnum[i+1];	 //MMSI船只名称,ASCII码
	len=20;
}
/***********************************************************
 * 函数名msg24B
 * 描述  ：生成消息24B
 * 输入  : 无
 * 输出  : 无
 ***********************************************************/
void msg24B(void)
{
	msg[0] =0x60 ;	         //6bit消息ID:24;  2bit转发指示符：00
	msg[1] = MMSI >>22;	     //30bit用户ID，MMSI编号
	msg[2] = MMSI >>14 ;
	msg[3] = MMSI >> 6;
	msg[4] = (MMSI << 2)|0x01;  //低2bit为部分编号：01
	msg[5] = 0x00;	 //船舶类型和货物类型	
	msg[6] = Vender_ID[0]; //供应商ID 1:8
	msg[7] = Vender_ID[1]; // 9:16
	msg[8] = Vender_ID[2];  //  17:24
	msg[9] = Vender_ID[3];   //25:32
	msg[10] = Vender_ID[4]; 		//33:40
	msg[11] = Vender_ID[5] | Call_sign[0];    //供应商ID 41:42;呼号 1:6
	msg[12] = Call_sign[1];       //呼号 7:14
	msg[13] = Call_sign[2];       //呼号 15:22
	msg[14] = Call_sign[3];								//呼号 23:30
	msg[15] = Call_sign[4];       //呼号 31:38
	msg[16] = Call_sign[5] | boatsize_a >> 5;   //呼号 39:42 // 船舶尺寸  A: 1-4
	msg[17] =	boatsize_a << 3 | boatsize_b >> 6; //A: 5-9 // B: 1-3  
	msg[18] = boatsize_b << 2 | boatsize_c >> 4;  // B: 4-9 // C:1-2
	msg[19] = boatsize_c << 4| boatsize_d >> 2;   // C: 3-6 // D: 1- 4
	msg[20] = boatsize_d << 6;										// D: 5,6        
	len=21;
}
