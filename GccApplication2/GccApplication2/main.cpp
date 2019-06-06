/*
 * GccApplication2.cpp
 *
 * Created: 6/6/2019 10:11:40 AM
 * Author : Elite Phantom
 */ 

#include <avr/io.h>
#include <stdlib.h>

struct node
{
	char data;
	int pointer; 
	int amount;
	void functionlist(){};
	node *next[];
};
typedef struct node NODE;

NODE* GetNode(int data, int amount)
{
	NODE *n = new NODE;
	if(n == NULL)
		return NULL;
	n->data = data;
	n->amount = amount;
}

int main(void)
{
    /* Replace with your application code */
    while (1) 
    {
    }
}

