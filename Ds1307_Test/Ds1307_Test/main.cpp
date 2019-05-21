/*
 * Ds1307_Test.cpp
 *
 * Created: 5/16/2019 1:05:20 AM
 * Author : Elite Phantom
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include "myDS1307RTC.h"
volatile uint8_t	Second=55, Minute=59, Hour=11, Day=4, Date=15, Month=5, Year=19, Mode=1, AP=1;
volatile uint8_t tData[7];
uint8_t length = 7;
uint8_t BCD2Dec(uint8_t BCD){
	uint8_t L, H;
	L=BCD & 0x0F;
	H=(BCD>>4)*10;
	return (H+L);
}


uint8_t Dec2BCD(uint8_t Dec){
	uint8_t L, H;
	L=Dec % 10;
	H=(Dec/10)<<4;
	return (H+L);
}


int main(void)
{
    /* Replace with your application code */
	DDRD = 0xFF;
	 tData[0]=Dec2BCD(Second);
	 tData[1]=Dec2BCD(Minute);
	 if (Mode!=0) tData[2]=Dec2BCD(Hour)|(Mode<<6)|(AP<<5); //mode 12h
	 else tData[2]=Dec2BCD(Hour);
	 tData[3]=Dec2BCD(Day);
	 tData[4]=Dec2BCD(Date);
	 tData[5]=Dec2BCD(Month);
	 tData[6]=Dec2BCD(Year);
	 TWI_Init();
	 TWI_DS1307_wblock(0x00,tData,length);
	 _delay_ms(10);
	 TWI_DS1307_wadr(0x00);
	 _delay_ms(10);
	 TWI_DS1307_rblock(tData,length);
	 Date   	= BCD2Dec(tData[4]);
	 if(Date == 15)
		PORTD = 0xFF;
    while (1) 
    {

    }
}

