#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main(void)
{
	DDRC = 0xFF;
	DDRD = 0x00;
	PORTD = 0x00;
	DDRB = 0xFF;
	GICR |= (1 << INT0);
	MCUCR &= ~((1 << ISC01) | (1 << ISC00));
	sei();
	
	while (1)
	{
		_delay_ms(500);
		PORTC = ~PORTC;
		_delay_ms(500);
		set_sleep_mode(SLEEP_MODE_PWR_DOWN);

		sleep_enable();
		sei();
		sleep_cpu();
		sleep_disable();
		cli();
	}
}


ISR(INT0_vect)
{
	PORTC = 0x00;
	_delay_ms(1000);
}