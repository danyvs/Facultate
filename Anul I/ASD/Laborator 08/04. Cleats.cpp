#include "pch.h"
#include <iostream>

#define NMAX 1000

using namespace std;

struct StackNode {
	int info;
	StackNode* next;
};

void readArray(int arr[], int &dimArr) {
	cin >> dimArr;
	for (int i = 0; i < dimArr; ++i)
		cin >> arr[i];
}

void pushStack(StackNode* &top, int data) {
	StackNode* node = new StackNode;
	node->info = data;
	node->next = top;
	top = node;
}

void popStack(StackNode* &top) {
	if (!top) {
		cout << "Stiva este vida!\n";
		return;
	}
	top = top->next;
}

inline int topStack(StackNode* top) {
	return top->info;
}

inline bool isStackEmpty(StackNode* top) {
	return !top;
}

bool areCleatsGood(int arr[], int dimArr) {
	StackNode* top = NULL;
	for (int i = 0; i < dimArr; ++i) {
		if (!isStackEmpty(top) && arr[i] == topStack(top))
			popStack(top);
		else
			pushStack(top, arr[i]);
	}
	return isStackEmpty(top);
}

int main() {
	int arr[NMAX], dimArr;
	readArray(arr, dimArr);
	if (areCleatsGood(arr, dimArr))
		cout << "Solutia este corecta!\n";
	else
		cout << "Solutia este gresita!\n";
	return 0;
}
