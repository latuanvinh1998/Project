#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main(void)
{
	DDRC = 0xFF;
	DDRD = 0x00;
	PORTD = 0x00;
	MCUCR|=(1<<ISC11)|(1<<ISC01); //c? 2 ng?t là ng?t c?nh xu?ng
	GICR    |=(1<<INT1)|(1<<INT0);
	// infinite main loop
	sei();
	while (1)
	{
		_delay_ms(500);
		PORTC = ~PORTC;
	}
}


ISR(INT0_vect)
{
	// ISR might be empty, but is necessary nonetheless
	PORTC = 0x00;    // debugging
	_delay_ms(1000);
}
ISR(INT1_vect)
{
			
			set_sleep_mode(SLEEP_MODE_PWR_DOWN);
			sleep_enable();
			sleep_cpu();
			sleep_disable();
			cli();
}