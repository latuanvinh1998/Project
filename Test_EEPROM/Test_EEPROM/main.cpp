#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
#include "24c64.h"
#include "USART.h"


int main()
{
	EEOpen();
	USART_Init();
	EEWriteByte(0x000F, 'c');
	while(1)
	{
		USART_Transmit(EEReadByte(0x000F));
		_delay_ms(1000);
	}
}