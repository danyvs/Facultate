#include "pch.h"
#include <iostream>

using namespace std;

struct Node {
	int info;
	Node *left, *right;
};

Node* newNode(int data) {
	Node* temp = new Node;
	temp->info = data;
	temp->left = temp->right = NULL;
	return temp;
}

void insertNode(Node* &node, int data) {
	if (!node)
		node = newNode(data);
	else {
		if (data < node->info)
			insertNode(node->left, data);
		else
			insertNode(node->right, data);
	}
}

void inorder(Node* root, int val1, int val2) {
	if (root) {
		if (root->info >= val1)
			inorder(root->left, val1, val2);
		if (root->info > val1 && root->info < val2)
			cout << root->info << " ";
		if (root->info <= val2)
			inorder(root->right, val1, val2);
	}
}

int main() {
	Node* root = NULL;
	int val1, val2;
	cin >> val1 >> val2;
	insertNode(root, 2);
	insertNode(root, 5);
	insertNode(root, 4);
	insertNode(root, 7);
	insertNode(root, 3);
	insertNode(root, 1);
	insertNode(root, 6);
	inorder(root, val1, val2);
	return 0;
}
