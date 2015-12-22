#include "main.h"

/***********************************************************
 * �������gmsg21
 * ����  ��������Ϣ21
 * ����  : ��
 * ���  : ��
 ***********************************************************/
void msg21(void)
{	
	u8 i;
	
// 	jingdu=GPS.longitude; //���¾�γ����Ϣ
// 	weidu=GPS.latitude;
	Read_GPS_Info();
	
	for(i=0;i<45;i++)  msg[i] = 0;
	
	msg[ 0] = 0x54;	         //6bit��ϢID:21;  2bitת��ָʾ����0
	msg[ 1] = (MMSI >> 22);	     //30bit�û�ID��MMSI���
	msg[ 2] = (MMSI >> 14);
	msg[ 3] = (MMSI >>  6);
	msg[ 4] = (MMSI <<  2)        + (dev_type     >> 3);  //��2bitΪ �����豸���͵ĸ�2bit
	msg[ 5] = (dev_type     << 5) + (dev_name[ 1] >> 3);  //��3bitΪ�����豸���ͣ������豸���ơ�����
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
	msg[20] = (dev_name[15] << 5) + (jingdu >> 24); //+λ��׼ȷ�ȣ�0 bit4; + ����
	msg[21] = (jingdu >> 16);
	msg[22] = (jingdu >>  8);
	msg[23] = (jingdu      );
	msg[24] = (weidu  >> 19);
	msg[25] = (weidu  >> 11);
	msg[26] = (weidu  >>  3);
	msg[27] = (weidu  <<  5)      + (dev_local[0] >> 1); //�ߴ�λ�òο���
	msg[28] = (dev_local[0] << 7) + (dev_local[1] << 1) + (dev_local[2] >> 5);
	msg[29] = (dev_local[2] << 3) + (dev_local[3] >> 3);
	msg[30] = (dev_local[3] << 5) + (dev_local[4] >> 1); 
	msg[31] = (dev_local[4] << 7) + (dev_time     >> 3) + (dev_etype    << 5); //+���Ӷ�λװ������
	msg[32] = (dev_time     << 5) + (dev_state    >> 4) + 0x10 ; //ƫ��λ��ָʾ����1��
	msg[33] = (dev_state    << 4) + 0; //RAIM��־:0 ��+ ����AtoN��־��+ָ��ģʽ��־;+����
	len=34;
	switch((dev_name[0])/6) //�����豸������չ
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
 * �������gmsg18
 * ����  ��������Ϣ18
 * ����  : ��
 * ���  : ��
 ***********************************************************/
void msg18(void)
{	
// 	jingdu=GPS.longitude; //���¾�γ����Ϣ
// 	weidu=GPS.latitude;
	
	Read_GPS_Info();
	
	msg[0] =0x48 ;	         //6bit��ϢID:18;  2bitת��ָʾ����0
	msg[1] = MMSI >>22;	     //30bit�û�ID��MMSI���
	msg[2] = MMSI >>14;
	msg[3] = MMSI >> 6;
	msg[4] = MMSI << 2;  //��2bit����Ϊ0
	msg[5] = sog >> 8;  //��6bit����Ϊ0����2bit���溽��Ϊ0
	msg[6] = sog;			 //���溽��Ϊ0
	msg[7] = (jingdu >> 21)| 0x80; //���λΪλ��׼ȷ��1������7λ��ʼΪ���ȵ����λ
	msg[8] = (jingdu) >> 13;   
	msg[9] = (jingdu) >> 5;
	msg[10] = ((jingdu << 3) & 0xF8 )+ ((weidu>>24 )& 0x07); //�������ݵ�5λ��γ����Ϣ��3bit.
	msg[11] = (weidu) >> 16; 
	msg[12] = (weidu) >> 8;
	msg[13] = (weidu) >> 0;
	msg[14] = (u8)(direction >> 4); //0xE1;	   //COG:3600 ��8bit
	msg[15] = (u8)(direction << 4); //0x40 + (u8)(direction >> 5);		   //COG��4λ + ʵ�ʺ����4λ ʵ�ʺ�������Ϊȫ0
	msg[16] = 0x07; //(u8)(direction << 3) + 0x07;		   //ʵ�ʺ���ĵ�5λ+ʱ�����3bit.ʵ�ʺ����ʱ�����������
	msg[17] = 0x84;	   //ʱ�����3bit:100;����2bit:00;B��װ�ñ�־1bit:1;B����ʾ����־1bit:0;B��DSC��־: 0  
	msg[18] = 0x08+((slot_increacement>>12)&0x01); // B������־�� 0��ͨ��״̬��3bit:000  ITDMA
	msg[19]=slot_increacement>>4;
	msg[20]=(slot_increacement<<4)+0x04;

	len=21;
}
/***********************************************************
 * �������gmsg24A
 * ����  ��������Ϣ24A
 * ����  : ��
 * ���  : ��
 ***********************************************************/
void msg24A(void)
{
	u8 i;
	msg[0] =0x60 ;	         //6bit��ϢID:24;  2bitת��ָʾ����00
	msg[1] = MMSI >>22;	     //30bit�û�ID��MMSI���
	msg[2] = MMSI >>14 ;
	msg[3] = MMSI >> 6;
	msg[4] = MMSI << 2;  //��2bitΪ���ֱ�ţ�00
	for(i=0;i<15;i++)   
		msg[5+i] = boatnum[i+1];	 //MMSI��ֻ����,ASCII��
	len=20;
}
/***********************************************************
 * �������gmsg24B
 * ����  ��������Ϣ24B
 * ����  : ��
 * ���  : ��
 ***********************************************************/
void msg24B(void)
{
	msg[0] =0x60 ;	         //6bit��ϢID:24;  2bitת��ָʾ����00
	msg[1] = MMSI >>22;	     //30bit�û�ID��MMSI���
	msg[2] = MMSI >>14 ;
	msg[3] = MMSI >> 6;
	msg[4] = (MMSI << 2)|0x01;  //��2bitΪ���ֱ�ţ�01
	msg[5] = 0x00;	 //�������ͺͻ�������	
	msg[6] = Vender_ID[0]; //��Ӧ��ID 1:8
	msg[7] = Vender_ID[1]; // 9:16
	msg[8] = Vender_ID[2];  //  17:24
	msg[9] = Vender_ID[3];   //25:32
	msg[10] = Vender_ID[4]; 		//33:40
	msg[11] = Vender_ID[5] | Call_sign[0];    //��Ӧ��ID 41:42;���� 1:6
	msg[12] = Call_sign[1];       //���� 7:14
	msg[13] = Call_sign[2];       //���� 15:22
	msg[14] = Call_sign[3];								//���� 23:30
	msg[15] = Call_sign[4];       //���� 31:38
	msg[16] = Call_sign[5] | boatsize_a >> 5;   //���� 39:42 // �����ߴ�  A: 1-4
	msg[17] =	boatsize_a << 3 | boatsize_b >> 6; //A: 5-9 // B: 1-3  
	msg[18] = boatsize_b << 2 | boatsize_c >> 4;  // B: 4-9 // C:1-2
	msg[19] = boatsize_c << 4| boatsize_d >> 2;   // C: 3-6 // D: 1- 4
	msg[20] = boatsize_d << 6;										// D: 5,6        
	len=21;
}
