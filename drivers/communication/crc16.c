//////////////////////////////////////////////////////////////////////////////////////////////////////////
// CRC-16
//////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "crc16.h"

/******************************************************
* Set the checksum to its initial value               *
* CALL THIS FUNCTION BEFORE YOU CALL "crc16_shift"    *
* THE FIRST TIME!                                     *
******************************************************/
void hs485_crc16_init( unsigned short* crc16_register )
{
	*crc16_register=0xffff;
}

/******************************************************
* Generate a CRC-16 checksum                          *
* This function include a given byte into the         *
* calculation                                         *
* CALL THIS FUNCTION 2x WITH ZERO AS PARAMETER TO     *
* COMPLETE THE CALCULATION!!!                         *
******************************************************/
void hs485_crc16_shift( unsigned short* crc16_register, unsigned char w)
{
	unsigned char q;
	unsigned char flag;
	for(q=0;q<8;q++)
	{
		flag=(*crc16_register & 0x8000)!=0;
		*crc16_register<<=1;
		if(w&0x80)
		{
			*crc16_register|=1;
		}
		w<<=1;
		if(flag)*crc16_register ^= CRC16_POLYNOM;
	}
}
