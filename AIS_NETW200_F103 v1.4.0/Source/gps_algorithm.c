#include "main.h"

//====================================================================//
// �﷨��ʽ: static int Str_To_Int(char *buf)
// ʵ�ֹ��ܣ� ��һ���ַ���ת��������
// ��    �����ַ���
// �� �� ֵ��ת��������ֵ
//====================================================================//
int Str_To_Int(char *buf)
{
	int rev = 0;
	int dat;
	char *str = buf;
	while(*str != '\0')
	{
		switch(*str)
		{
			case '0':
				dat = 0;
				break;
			case '1':
				dat = 1;
				break;
			case '2':
				dat = 2;
				break;		
			case '3':
				dat = 3;
				break;
			case '4':
				dat = 4;
				break;
			case '5':
				dat = 5;
				break;
			case '6':
				dat = 6;
				break;
			case '7':
				dat = 7;
				break;
			case '8':
				dat = 8;
				break;
			case '9':
				dat = 9;
				break;
		}

		rev = rev * 10 + dat;
		str++;
	}

	return rev;
}
//====================================================================//
// �﷨��ʽ: static int Get_Int_Number(char *s)
// ʵ�ֹ��ܣ��Ѹ����ַ�����һ������֮ǰ���ַ�ת��������
// ��    �����ַ���
// �� �� ֵ��ת��������ֵ
//====================================================================//
int Get_Int_Number(char *s)
{
	char buf[10];
	u8 i;
	int rev;
	
	i = GetComma(1, s);
	i = i - 1;
	strncpy(buf, s, i);
	buf[i] = 0;
	rev = Str_To_Int(buf);
	
	return rev;	
}
//====================================================================//
// �﷨��ʽ: static float Str_To_Float(char *buf)
// ʵ�ֹ��ܣ� ��һ���ַ���ת���ɸ�����
// ��    �����ַ���
// �� �� ֵ��ת���󵥾���ֵ
//====================================================================//
float Str_To_Float(char *buf)
{
	float rev = 0;
	float dat;
	int integer = 1;
	char *str = buf;
	int i;
	
	while(*str != '\0')
	{
		switch(*str)
		{
			case '0':
				dat = 0;
				break;
			case '1':
				dat = 1;
				break;
			case '2':
				dat = 2;
				break;		
			case '3':
				dat = 3;
				break;
			case '4':
				dat = 4;
				break;
			case '5':
				dat = 5;
				break;
			case '6':
				dat = 6;
				break;
			case '7':
				dat = 7;
				break;
			case '8':
				dat = 8;
				break;
			case '9':
				dat = 9;
				break;
			case '.':
				dat = '.';
				break;
		}
		if(dat == '.')
		{
			integer = 0;
			i = 1;
			str++;
			continue;
		}
		if( integer == 1 )
		{
			rev = rev * 10 + dat;
		}
		else
		{
			rev = rev + dat / (10 * i);
			i = i * 10 ;
		}
		str++;
	}
	
	return rev;
}
												
//====================================================================//
// �﷨��ʽ: static float Get_Float_Number(char *s)
// ʵ�ֹ��ܣ� �Ѹ����ַ�����һ������֮ǰ���ַ�ת���ɵ�������
// ��    �����ַ���
// �� �� ֵ��ת���󵥾���ֵ
//====================================================================//
float Get_Float_Number(char *s)
{
	char buf[10];
	u8 i;
	float rev;
	
	i = GetComma(1, s);
	i = i - 1;
	strncpy(buf, s, i);
	buf[i] = 0;
	rev = Str_To_Float(buf);
	
	return rev;	
}

//====================================================================//
// �﷨��ʽ: static double Str_To_Double(char *buf)
// ʵ�ֹ��ܣ� ��һ���ַ���ת���ɸ�����
// ��    �����ַ���
// �� �� ֵ��ת����˫����ֵ
//====================================================================//
double Str_To_Double(char *buf)
{
	double rev = 0;
	double dat;
	int integer = 1;
	char *str = buf;
	int i;
	
	while(*str != '\0')
	{
		switch(*str)
		{
			case '0':
				dat = 0;
				break;
			case '1':
				dat = 1;
				break;
			case '2':
				dat = 2;
				break;		
			case '3':
				dat = 3;
				break;
			case '4':
				dat = 4;
				break;
			case '5':
				dat = 5;
				break;
			case '6':
				dat = 6;
				break;
			case '7':
				dat = 7;
				break;
			case '8':
				dat = 8;
				break;
			case '9':
				dat = 9;
				break;
			case '.':
				dat = '.';
				break;
		}
		if(dat == '.')
		{
			integer = 0;
			i = 1;
			str++;
			continue;
		}
		if( integer == 1 )
		{
			rev = rev * 10 + dat;
		}
		else
		{
			rev = rev + dat / (10 * i);
			i = i * 10 ;
		}
		str++;
	}
	
	return rev;
}
												
//====================================================================//
// �﷨��ʽ: static double Get_Double_Number(char *s)
// ʵ�ֹ��ܣ��Ѹ����ַ�����һ������֮ǰ���ַ�ת����˫������[���޸�Ϊ����ֵΪlong������]
// ��    �����ַ���
// �� �� ֵ��ת����˫����ֵ
//====================================================================//
double Get_Double_Number(char *s)
{
	char buf[10];
	u8 i;
	double rev;
	long a,temp;
	
	i = GetComma(1, s);
	i = i - 1;
	strncpy(buf, s, i);
	buf[i] = 0;
	rev = Str_To_Double(buf);
	
	temp = rev/100;
	a = temp*600000 + (rev - temp*100)*10000;
	
	return a;	
}

double Get_Double_Number_sog(char *s)  //for sog
{
	char buf[10];
	u8 i;
	double rev;
	unsigned int a;
	
	i = GetComma(1, s);
	i = i - 1;
	strncpy(buf, s, i);
	buf[i] = 0;
	rev = Str_To_Double(buf);
	
	a = rev*10/1;
	
	return a;	
}

double Get_Double_Number_direction(char *s)  //for direction
{
	char buf[10];
	u8 i;
	double rev;
	unsigned int a;
	
	i = GetComma(1, s);
	i = i - 1;
	strncpy(buf, s, i);
	buf[i] = 0;
	rev = Str_To_Double(buf);
	
	a = rev/1 *10;
	
	return a;	
}

double Get_Double_Number_time(char *s)  //for direction
{
	char buf[10];
	u8 i;
	double rev;
	unsigned long a;
	
	i = GetComma(1, s);
	i = i - 1;
	strncpy(buf, s, i);
	buf[i] = 0;
	rev = Str_To_Double(buf);
	
	a = rev/1;
	
	return a;	
}
//====================================================================//
// �﷨��ʽ��static uchar GetComma(uchar num,char *str)
// ʵ�ֹ��ܣ������ַ����и������ŵ�λ��
// ��    �������ҵĶ����ǵڼ����ĸ�������Ҫ���ҵ��ַ���
// �� �� ֵ��0
//====================================================================//
u8 GetComma(u8 num, char *str)
{
	u8 i,j = 0;
	int len = strlen(str);

	for(i = 0; i < len; i++)
	{
		if(str[i] == ',')
			j++;
		if(j == num)
			return i + 1;	
	}

	return 0;	
}

//====================================================================//
//	�﷨��ʽ��	Int_To_Str(int x,char *Str)
//	ʵ�ֹ��ܣ�	ת������ֵΪ�ַ�����ʽ
//	������		x: ת��������
//				Str:ת������ַ���
//	����ֵ��	��
//====================================================================//
void Int_To_Str(int x, char *Str)
{
	int t;
	char *Ptr,Buf[5];
	int i = 0;
	
	Ptr = Str;
	if(x < 10)		// ������С��10ʱ,ת��Ϊ"0x"�ĸ�ʽ
	{
		*Ptr++ = '0';
		*Ptr++ = x+0x30;
	}
	else
	{
		while(x > 0)
		{
			t = x % 10;
			x = x / 10;
			Buf[i++] = t+0x30;	// ͨ�����������ת����ASCII����ʽ
		}
		i-- ;
		for(;i >= 0;i--) 		// ���õ����ַ�������
		{
			*(Ptr++) = Buf[i];
		}
	}
	*Ptr = '\0';
}
