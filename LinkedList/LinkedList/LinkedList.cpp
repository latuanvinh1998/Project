#include <iostream>
#include <cstring>
#include <conio.h>
#include <cstdlib>


using namespace std;

struct node
{
	char data;
	int amount;
	node* prev;
	node* next[];
};

typedef struct node NODE;


NODE* CreateNode(char data, int amount, NODE *pre)
{
	NODE* n = new NODE;
	// 	if(n == NULL)
	// 		return NULL;
	n->data = data;
	n->amount = amount;
	n->prev = pre;
	for (int i = 0; i < n->amount; i++)
		n->next[i] = NULL;
	return n;
}


void InsertNode(NODE*& root, char x, int y, int *array, int length)
{
	if (root == NULL)
	{
		root = CreateNode(x, y, NULL);
		return;
	}
	NODE* p = root;
	int i;
	for(i = 1 ; i < length; i++)
		p = p->next[array[i]];
	p->next[array[i - 1]] = CreateNode(x, y, p);
}

NODE* Read(NODE* R, int* arr, int length)
{
	NODE* ptr = R;
	for (int i = 0; i < length; i++)
		ptr = ptr->next[arr[i]];
	return ptr;
}

int main()
{
	int array[5];
	int count = 0;
	char buffer = '!';
	NODE* Root = CreateNode('1', 5, NULL);

	for (int i = 0; i < 5; i++)
	{
		array[0] = count;
		InsertNode(Root, buffer, 3, array, 1);
		count++;
		buffer++;
	}

	NODE* p = Root;
	int cmd = 1;

	cout << " Press 0 for ending.\nVietsub: Nhap 0 ma tat\n";

	while (cmd != 0)
	{
		cout << "\nInput the order of next node u want to read:";
		cin >> cmd;

		if (cmd > 0)
			p = p->next[cmd - 1];

		else if (cmd == -1)
		{
			cout << "back to the previous node\n\n";
			p = p->prev;
			cout << "the value of previous node is: " << p->data << endl << endl;
			continue;
		}

		if (p == NULL)
		{
			cout << "_____________Reach the end of the linked list______________\n";
			continue;
		}

		cout << "\n_____the value of data is: " <<p->data << endl << endl;
	}

	return 0;
}