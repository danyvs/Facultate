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

int searchNode(Node* root, int val) {
	if (!root)
		return 0;
	if (val == root->info)
		return 1;
	else {
		if (val < root->info)
			return searchNode(root->left, val);
		else
			return searchNode(root->right, val);
	}
	return 0;
}

void printMaxValue(Node* root) {
	while (root->right)
		root = root->right;
	cout << root->info << "\n";
}

Node* minValueNode(Node* root) {
	while (root->left != NULL)
		root = root->left;
	return root;
}

Node* deleteNode(Node* &root, int val) {
	Node* temp;
	if (!root)
		return root;
	if (val < root->info) {
		root->left = deleteNode(root->left, val);
		return root;
	}
	else {
		if (val > root->info) {
			root->right = deleteNode(root->right, val);
			return root;
		}
	}

	if (root->left == NULL) {
		temp = root->right;
		delete(root);
		return temp;
	}
	else {
		if (root->right == NULL) {
			temp = root->left;
			delete(root);
			return temp;
		}
		else {
			Node* succParent = root->right;
			Node* succ = root->right;
			while (succ->left) {
				succParent = succ;
				succ = succ->left;
			}
			
			succParent->left = succ->right;
			root->info = succ->info;
			delete(succ);
			return root;
		}
	}

}

void inorder(Node* root) {
	if (root) {
		inorder(root->left);
		cout << root->info << " ";
		inorder(root->right);
	}
}

void preorder(Node* root) {
	if (root) {
		cout << root->info << " ";
		preorder(root->left);
		preorder(root->right);
	}
}

void postorder(Node* root) {
	if (root) {
		postorder(root->left);
		postorder(root->right);
		cout << root->info << " ";
	}
}

int main() {
	Node* root = NULL;
	
	return 0;
}
