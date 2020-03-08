#include "Utilites.h"



void IntToHexStr(uint32 num,char *str)
{
	str[0]=(uint8)num;
	str[1]=(uint8)(num>>8);
	str[2]=(uint8)(num>>8);
	str[3]=(uint8)(num>>8);
	str[2]=0;
}

uint32 hexStrToInt(char *str,uint8 datatypeLength)
{
	
	uint32 num=0;

	while(datatypeLength > 0)
	{
		 num =(num << 8) + str[--datatypeLength];
	}
	return num;
}