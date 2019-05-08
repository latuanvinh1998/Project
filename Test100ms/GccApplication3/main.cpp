#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
int main()
{
	DDRD = 0xFF;
	while(1)
	{
		PORTD = 0x00;
		_delay_ms(1000);
		PORTD = 0xFF;
		_delay_ms(1000);
	}
}