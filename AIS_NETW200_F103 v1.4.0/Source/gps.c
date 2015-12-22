#include "main.h"
#include "math.h"

static u8 t_convert=0;   //��¼�յ���Чλ����Ϣ�Ĵ���

int GPS_RMC_Parse(char *line, GPS_INFO *GPS)
{

	char *buf = line;
	u8 ch, status;
	u16 time_n,delta_time;
	double tantemp1,tantemp2,tantemp3,tantemp4,tantemp5;
	double delta_jingdu,delta_weidu;
	double sogtemp;
	
	ch = buf[5];
	status = buf[GetComma(2, buf)];
	
	if (ch == 'C')  //���������ַ���C��($GPRMC)
	{
//  		USART_SendData(USART1, status);
//   		while (!(USART1->SR & USART_FLAG_TXE)) ;
		
		if (status == 'A')  //���������Ч�������
		{
			t_convert++;
			if(t_convert==3)
			{
				txbuf[0]='$'; txbuf[1]=0x1C; txbuf[2]=0x01; //GPS��Чָ��
				sendback(); //
				
				GPS->latitude = Get_Double_Number(&buf[GetComma( 3, buf)]);
				GPS->longitude = Get_Double_Number(&buf[GetComma( 5, buf)]);

				GPS->second = ((buf[7]-'0')*10+(buf[8]-'0'))*3600/2 + ((buf[9]-'0')*10+(buf[10]-'0'))*60/2 + ((buf[11]-'0')*10+(buf[12]-'0'))/2;	//��2��Ϊ��λ

				GPS->NS = buf[GetComma(4, buf)];
				GPS->EW = buf[GetComma(6, buf)];
		
				if(GPS -> NS == 'N') GPS->latitude  = GPS->latitude*1;  //γ��
				else GPS->latitude  = GPS->latitude*(-1);
				
				if(GPS -> EW == 'E') GPS->longitude = GPS->longitude*1;	//����
				else GPS->longitude  = GPS->longitude*(-1);

				//����
				time_n = GPS->second;	
				tantemp4 = jingdu;  tantemp5 = weidu; //��
				jingdu=GPS->longitude; //���¾�γ����Ϣ
				weidu=GPS->latitude;
				tantemp1 = weidu; tantemp2 = jingdu; //��
				delta_jingdu = tantemp2 - tantemp4;
				delta_weidu = tantemp1 - tantemp5;		
				if(time_n > time_o)  
				{
					delta_time = time_n-time_o;
					delta_jingdu = delta_jingdu/10000; delta_jingdu = delta_jingdu/60;
					delta_weidu = delta_weidu/10000; delta_weidu = delta_weidu/60;
					sogtemp = sqrt(delta_jingdu*delta_jingdu + delta_weidu*delta_weidu);
					sogtemp = sogtemp*57267.6886495/(delta_time*2);
					sog = (u16)(sogtemp*10);
				}
				else sog = 0;

				time_o = time_n;
				//sog = Get_Double_Number_sog(&buf[GetComma( 7, buf)]);
				
				//����
				Read_Flash_Cogsel();
			 if (cogsel == 0)
				{
						if(delta_jingdu > 0 && delta_weidu > 0)
							tantemp3 = atan2(delta_jingdu,delta_weidu) * 1800 / 3.14159;
						else if(delta_jingdu > 0 && delta_weidu < 0)
							tantemp3 = 1800 - atan2(delta_jingdu,(-1)*(delta_weidu)) * 1800 / 3.14159;
						else if(delta_jingdu < 0 && delta_weidu < 0)
							tantemp3 = 1800 + atan2((-1)*(delta_jingdu),(-1)*(delta_weidu)) * 1800 / 3.14159;
						else if(delta_jingdu < 0 && delta_weidu > 0)
							tantemp3 = 3600 - atan2((-1)*(delta_jingdu),delta_weidu) * 1800 / 3.14159;

						direction = (u16)tantemp3;
						
						if(delta_jingdu == 0 && delta_weidu > 0) direction = 0;
						else if(delta_jingdu == 0 && delta_weidu < 0) direction = 1800;
						else if(delta_jingdu == 0 && delta_weidu == 0) direction = 0;
						else if(delta_jingdu > 0 && delta_weidu == 0) direction = 900;
						else if(delta_jingdu < 0 && delta_weidu == 0) direction = 2700;
				}
				else
				{
					 direction = 0;
				}
						
				//direction = 0; //�˰�ǿ��Ҫ����Ϊ��
				
				//direction = Get_Double_Number_direction(&buf[GetComma( 8, buf)]);
				
				//ҹ����
				gpstime = Get_Double_Number_time(&buf[GetComma( 1, buf)]); 
				if(gpstime >= 93000 && gpstime < 220000)  //��6-��6
				{
					txbuf[0]='$'; txbuf[1]=0x29; txbuf[2]=0x01; //����ҹ������
					sendback(); //
				}
				else if(gpstime >= 220000)
				{
					txbuf[0]='$'; txbuf[1]=0x29; txbuf[2]=0x00; //�ر�ҹ������
					sendback(); //
				}
				else if(gpstime < 93000)
				{
					txbuf[0]='$'; txbuf[1]=0x29; txbuf[2]=0x00; //�ر�ҹ������
					sendback(); //
				}

				Write_GPS_Info();
				
				TIM_Cmd(TIM2, DISABLE);
				GPS_OFF(); //gps off,GPS����һ����Ϣ���ر�
				//PLL_OFF();

				gps_off_flag=off;
				flag_fre_check=off;
				gps_off_cnt=0;
					
				
				//task_flag2=on;//�����������
								
				interval_s = intervalA; //�����ָ�����ֵ
				BKP_WriteBackupRegister(BKP_DR3,interval_s);
							
			 if(charging_flag == off)
			 {
				 RTC_Init();
				 PWR_WakeUpPinCmd(ENABLE);
				 PWR_EnterSTANDBYMode();
			 }
		 }
		}
		
	}
	
	return 1;
}