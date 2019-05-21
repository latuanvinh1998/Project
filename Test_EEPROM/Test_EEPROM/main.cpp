#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
#include "24c64.h"
int main()
{
	DDRD = 0xFF;
	EEOpen();
	EEWriteByte(0x000F, 7);
	if(EEReadByte(0x000F) == 7)
	{
		_delay_ms(100);
		PORTD = 0xFF;
	}
	while(1);
}