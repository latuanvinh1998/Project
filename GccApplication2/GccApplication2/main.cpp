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
	int amount;
	node *next[];
};
typedef struct node NODE;


NODE* CreateNode(char data, int amount)
{
	NODE *n = (NODE*) malloc(sizeof(struct node));
// 	if(n == NULL)
// 		return NULL;
	n->data = data;
	n->amount = amount;
	for (int i = 0; i < n->amount; i++)
		n->next[i] = NULL;
	return n;
}

void InsertNode(NODE*& root, char x, int y, int *array, int length)
{
	if (root == NULL)
	{
		root = CreateNode(x, y);
		return;
	}
	NODE* p = root;
	int i;
	for(i = 1 ; i < length; i++)
		p = p->next[array[i]];
	p->next[array[i - 1]] = CreateNode(x, y);
}

NODE* Read(NODE* R, int* arr, int length)
{
	NODE* ptr = R;
	int i;
	for (i = 0; i < length; i++)
		ptr = ptr->next[arr[i]];
	return ptr;
}

int main(void)
{
	int arr[] = {2};
	NODE *Root = CreateNode('a', 3);
	InsertNode(Root, 'z', 1, arr, 1);
	NODE *p =Read(Root, arr, 1);
	USART_Init();
    while (1)
	{
		USART_Transmit(p->data);
		_delay_ms(1000);
	}
}

