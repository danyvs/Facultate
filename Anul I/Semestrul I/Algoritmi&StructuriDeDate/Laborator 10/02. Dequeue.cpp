#include "pch.h"
#include <iostream>

using namespace std;

struct Node {
	int info;
	Node *prev, *next;
};

inline bool isDequeueEmpty(Node* front, Node* tail) {
	return !front;
}

void pushFront(Node* &front, Node* &tail, int data) {
	Node* temp = new Node;
	temp->info = data;
	temp->prev = NULL;
	temp->next = front;
	if (isDequeueEmpty(front, tail))
		front = tail = temp;
	else {
		front->prev = temp;
		front = temp;
	}
}

void pushTail(Node* &front, Node* &tail, int data) {
	Node* temp = new Node;
	temp->info = data;
	temp->prev = tail;
	temp->next = NULL;
	if (isDequeueEmpty(front, tail))
		front = tail = temp;
	else {
		tail->next = temp;
		tail = temp;
	}
}

void popFront(Node* &front, Node* &tail) {
	if (isDequeueEmpty(front, tail)) {
		cout << "Dequeue-ul este vid!\n";
		return;
	}
	
	Node* temp = front;
	front = front->next;
	delete(temp);
}

void popTail(Node* &front, Node* &tail) {
	if (isDequeueEmpty(front, tail)) {
		cout << "Dequeue-ul este vid!\n";
		return;
	}

	Node* temp = tail;
	tail = tail->prev;
	delete(temp);
}

int getFront(Node* front, Node* tail) {
	if (isDequeueEmpty(front, tail)) {
		cout << "Dequeue-ul este vid!\n";
		return -1;
	}
	return front->info;
}

int getTail(Node* front, Node* tail) {
	if (isDequeueEmpty(front, tail)) {
		cout << "Dequeue-ul este vid!\n";
		return -1;
	}
	return tail->info;
}

int main() {
	Node *front = NULL, *tail = NULL;

	return 0;	
}
