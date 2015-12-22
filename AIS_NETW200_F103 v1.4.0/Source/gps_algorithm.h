#ifndef __ALGORITHM_H_
#define __ALGORITHM_H_

#include "stm32f10x.h"//#include "stm32f10x_type.h"


u8 GetComma(u8 num,char* str);
int Get_Int_Number(char *s);
double Get_Double_Number(char *s);
double Get_Double_Number_sog(char *s); 
double Get_Double_Number_direction(char *s);
double Get_Double_Number_time(char *s);
float Get_Float_Number(char *s);

int Str_To_Int(char *buf);
float Str_To_Float(char *buf);
double Str_To_Double(char *buf);
void Int_To_Str(int x,char *Str);

#endif /*__ALGORITHM_H_*/
