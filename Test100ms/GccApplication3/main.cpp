#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
int main()
{
	DDRB= 0xFF;
	while(1)
	{
		PORTB = 0x00;
		_delay_ms(100);
		PORTB = 0xFF;
		_delay_ms(100);
	}
}