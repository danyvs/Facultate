#include "pch.h"
#include <iostream>

using namespace std;

struct QueueNode {
	int info;
	QueueNode* next;
};

void pushQueue(QueueNode* &front, QueueNode* &tail, int data) {
	QueueNode* node = new QueueNode;
	node->info = data;
	node->next = NULL;
	if (!front)
		front = tail = node;
	else {
		tail->next = node;
		tail = node;
	}
}

int popQueue(QueueNode* &front, QueueNode* &tail) {
	if (!front) {
		cout << "Coada este vida! ";
		return -1;
	}
	
	int valFront = front->info;
	front = front->next;
	return valFront;
}

int peekQueue(QueueNode* front, QueueNode* tail) {
	if (!front) {
		cout << "Coada este vida! ";
		return -1;
	}
	return front->info;
}

bool isQueueEmpty(QueueNode* front, QueueNode* tail) {
	return !front;
}

int searchQueue(QueueNode* front, QueueNode* tail, int elem) {
	if (!front) {
		cout << "Coada este vida! ";
		return -1;
	}

	QueueNode* it = front;
	int cnt = 1;
	while (it && it->info != elem) {
		++cnt;
		it = it->next;
	}

	if (!it)
		return -1;
	return cnt;
	
}

void printQueue(QueueNode* front, QueueNode* tail) {
	if (!front) {
		cout << "Coada este vida!\n";
		return;
	}

	QueueNode* it = front;
	cout << "out ";
	while (it) {
		cout << it->info << " ";
		it = it->next;
	}
	cout << "in\n";
}

int main() {
	QueueNode *front = NULL, *tail = NULL;

	/* INPUT
	printQueue(front, tail);
	pushQueue(front, tail, 1);
	pushQueue(front, tail, 2);
	pushQueue(front, tail, 3);
	printQueue(front, tail);
	cout << searchQueue(front, tail, 2) << "\n";
	cout << searchQueue(front, tail, 4) << "\n";
	cout << isQueueEmpty(front, tail) << "\n";
	cout << popQueue(front, tail) << "\n";
	printQueue(front, tail);
	cout << peekQueue(front, tail) << "\n";
	printQueue(front, tail);
	popQueue(front, tail);
	popQueue(front, tail);
	printQueue(front, tail);
	cout << popQueue(front, tail) << "\n";
	cout << isQueueEmpty(front, tail) << "\n";
	*/

	return 0;
}
