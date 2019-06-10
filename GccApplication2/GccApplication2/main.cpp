/*
 * GccApplication2.cpp
 *
 * Created: 6/6/2019 10:11:40 AM
 * Author : Elite Phantom
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>
#include <string.h>


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


//////////////////////////////////////////////////////////////////////


struct node
{
	char data;
	int *pointer; 
	int amount;
	node *next;
};
typedef struct node NODE;


NODE* CreateNode(char data, int amount)
{
	NODE *n = (NODE*) malloc(sizeof(struct node));
// 	if(n == NULL)
// 		return NULL;
	n->data = data;
	n->amount = amount;
	n->next = NULL;
	return n;
}

NODE* CreateNode(NODE* &n, char data, int amount)
{
	n->data = data;
	n->amount = amount;
	n->next = NULL;
	return n;
}

void add(NODE* &R, char Data, int Amount)
{
	NODE *p = R;
	p = p->next;
	p = CreateNode(Data, Amount);
}

int main(void)
{
	NODE *Root = CreateNode('a', 1);
	add(Root, 'r', 1);
	NODE *p = Root;
// 	p = p->next;
	USART_Init();
    while (1)
	{
		USART_Transmit(p->data);
		_delay_ms(1000);
	}
}

