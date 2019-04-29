/*
 * Test_EEPROM.cpp
 *
 * Created: 4/21/2019 8:44:48 PM
 * Author : Elite Phantom
 */ 

#include <avr/io.h>
#include "24c64.h"
#include <util/delay.h>
#include <avr/interrupt.h>

int main(void)
{
	EEOpen();
    /* Replace with your application code */
	EEWriteByte(0x00FF, 255);
	_delay_ms(100);
    while (1) 
	{
			DDRC = 0xFF;
			PORTC = 0x00;
			_delay_ms(100);
			PORTC = 0xFF;
			_delay_ms(100);
	}
	return 0;
}