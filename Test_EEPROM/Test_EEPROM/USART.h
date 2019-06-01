/*
 * USART.h
 *
 * Created: 6/1/2019 3:41:13 PM
 *  Author: Elite Phantom
 */ 


#ifndef USART_H_
#define USART_H_
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



#endif /* USART_H_ */