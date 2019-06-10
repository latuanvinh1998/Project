#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>

void USART_Init(){
	UBRRH = 0;
	UBRRL = 51;
	UCSRC |= (1<<URSEL)|(3<<UCSZ0);
	UCSRB |= (1<<TXEN);
}

void USART_Transmit(char data){
	while (!(UCSRA & (1<<UDRE)));
	UDR = data;
}

void USART_SendString(char *data){
	register int i = 0;
	while (data[i]){
		USART_Transmit(data[i]);
		i++;
	}
}
int main(void)
{
	USART_Init();
	while(1)
	{
		USART_Transmit('a');
		_delay_ms(100);
	}
}
