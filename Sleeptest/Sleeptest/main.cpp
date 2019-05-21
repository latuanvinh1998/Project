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
	_delay_ms(500);
	GICR |= (1 << INT0);
	// level interrupt INT0 (low level)
	MCUCR &= ~((1 << ISC01) | (1 << ISC00));

	// infinite main loop
	while (1)
	{
		PORTC = 0x00;
		_delay_ms(500);
		PORTC = 0xFF;
	PORTB = 0x00;
		// enable external interrup

		// set sleep mode
		set_sleep_mode(SLEEP_MODE_PWR_DOWN);

		// sleep_mode() has a possible race condition
		sleep_enable();
		sei();
		sleep_cpu();
		sleep_disable();

		// waking up...
		// disable external interrupt here, in case the external low pulse is too long
		PORTB = 0xFF;
		// disable all interrupts
		cli();
	}
}


ISR(INT0_vect)
{
	// ISR might be empty, but is necessary nonetheless
	PORTC = ~PORTC;    // debugging
}