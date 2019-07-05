#include "pch.h"
#include <iostream>

#define NMAX 1000

using namespace std;

struct StackNode {
	char info;
	StackNode* next;
};

StackNode* newNode(char c) {
	StackNode* node = new StackNode;
	node->info = c;
	node->next = NULL;
	return node;
}

void stackPush(StackNode* &top, char c) {
	StackNode* temp = newNode(c);
	temp->next = top;
	top = temp;
}

inline bool isStackEmpty(StackNode* top) {
	return !top;
}

void stackPop(StackNode* &top) {
	top = top->next;
}

bool areBracketsCorrect(char str[]) {
	StackNode *top = NULL;
	for (int i = 0; str[i]; ++i)
		if (str[i] == '(')
			stackPush(top, '(');
		else {
			if (isStackEmpty(top))
				return false;
			else
				stackPop(top);
		}
	return isStackEmpty(top);
}

int main() {
	char str[NMAX];
	cin >> str;
	if (areBracketsCorrect(str))
		cout << "Sirul este corect parantezat!\n";
	else
		cout << "Sirul NU este corect parantezat!\n";
	return 0;
}
