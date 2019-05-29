/*
 * RamTest.cpp
 *
 * Created: 5/27/2019 11:06:25 PM
 * Author : Elite Phantom
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
#include <string.h>
#include "Ram.h"

int main(void)
{
    /* Replace with your application code */
	DDRD = 0xFF;
	PORTD = 0x00;
	_delay_ms(100);
	Init();
	WriteDataByte(0x0F0F, 'c');
	_delay_ms(10);
	PORTD = ReadDataByte(0x0F0F);
    while (1) 
    {
    }
}

