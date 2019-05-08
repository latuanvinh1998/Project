#include "N5110.h"
#include <string.h>
#include <avr/sfr_defs.h>
#include <avr/interrupt.h>
volatile int x = 0, select = 0;
char data[10][14] = {"1.vinh20cm","2.vinhcuto","3.vinhcukhung","4.vinhbigdick","5.vinhkhoaito","6.vinhsex","7.thichdit","8.ditditdit","9.ditmemay","10.capdatmaan"};
Nokia5110 lcd;

int main(void)
{
	MCUCR|=(1<<ISC11)|(1<<ISC01);
	GICR    |=(1<<INT1)|(1<<INT0);
	sei();
	DDRC |= (1 << 5);
	PORTC |= (1 << 5);
	lcd.begin();
	lcd.setCursor(0, 0);
	lcd.fillUp("LIST:");
	lcd.PrintArray(data, x, select);
	while(1){
	}
}
ISR(INT0_vect){
	if(select < 5)
		select++;
	else if(select == 5)
	{
		if (x < 5)
		x++;
	}
	lcd.PrintArray(data, x, select);
	_delay_ms(1000);
}
ISR(INT1_vect){
	if(select > 1)
		select--;
	else if(select == 1)
	{
		if(x > 0)
		x--;
	}
	lcd.PrintArray(data, x, select);
	_delay_ms(1000);
}