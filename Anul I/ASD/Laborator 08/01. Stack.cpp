#include "pch.h"
#include <iostream>

using namespace std;

struct StackNode {
	int info;
	StackNode *next;
};

StackNode* newNode(int data) {
	StackNode *stackNode = new StackNode;
	stackNode->info = data;
	stackNode->next = NULL;
	return stackNode;
}

/* Adaugarea unui nou numar in varful stivei */
void pushStack(StackNode* &head, int data) {
	StackNode *stackNode = newNode(data);
	stackNode->next = head;
	head = stackNode;
}

/* Eliminarea elementului din varful stivei si returnarea lui */
int popStack(StackNode* &head) {
	if (!head) {
		// stiva este vida
		return -1;
	}
	else {
		int stackTop = head->info;
		head = head->next;
		return stackTop;
	}
}

/* Returneaza elementul din varful stivei, daca exista */
int peekStack(StackNode* head) {
	if (!head) {
		// stiva este vida
		return -1;
	}
	else
		return head->info;
}

/* Returneaza adevarat daca stiva este vida */
bool isStackEmpty(StackNode *head) {
	return !head;
}

/* Cauta un element in stiva si returneaza distanta fata de varf */
int searchElemInStack(StackNode *head, int elem) {
	int dist = 0;
	StackNode *it = head;
	while (it) {
		if (it->info == elem)
			return dist;
		++dist;
		it = it->next;
	}
	return -1;
}

/* Afisarea stivei */
void printStack(StackNode* head) {
	if (!head) {
		// stiva este vida
		cout << "Stiva vida!\n";
	}
	else {
		StackNode *tempHead = head;
		cout << "varf ";
		while (tempHead) {
			cout << tempHead->info << " ";
			tempHead = tempHead->next;
		}
		cout << "baza\n";
	}
}

int main() {
	StackNode *head = NULL;

	/* INPUT
	printStack(head);
	pushStack(head, 1);
	pushStack(head, 2);
	pushStack(head, 3);
	printStack(head);
	cout << searchElemInStack(head, 2) << "\n";
	cout << searchElemInStack(head, 4) << "\n";
	cout << isStackEmpty(head) << "\n";
	cout << popStack(head) << "\n";
	printStack(head);
	cout << peekStack(head) << "\n";
	printStack(head);
	popStack(head);
	popStack(head);
	printStack(head);
	cout << popStack(head) << "\n";
	cout << isStackEmpty(head) << "\n";
	*/

	return 0;
}
