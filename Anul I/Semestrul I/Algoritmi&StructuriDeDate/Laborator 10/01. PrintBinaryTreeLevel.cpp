#include "pch.h"
#include <iostream>

using namespace std;

struct Node {
	int info;
	Node *leftSon, *rightSon;
};

Node* newNode(int data) {
	Node* temp = new Node;
	temp->info = data;
	temp->leftSon = temp->rightSon = NULL;
	return temp;
}

void inorder(Node* root, int lvl, int k) {
	if (root) {
		inorder(root->leftSon, lvl + 1, k);
		if (lvl == k)
			cout << root->info << " ";
		inorder(root->rightSon, lvl + 1, k);
	}
}

int main() {
	Node* root;
	int k;
	cin >> k;
	root = newNode(1);
	root->leftSon = newNode(2);
	root->rightSon = newNode(3);
	root->leftSon->leftSon = newNode(4);
	root->leftSon->rightSon = newNode(5);
	root->rightSon->leftSon = newNode(6);
	inorder(root, 0, k);
	return 0;
}
