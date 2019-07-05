#include "pch.h"
#include <iostream>

using namespace std;

#define NMAX 1000

struct Node {
	char character;
	int weight;
	Node *left, *right;
};

struct PriorityQueue {
	Node** harr;
	int heapSize, capacity;
};

Node* newNode(char c, int prty) {
	Node* temp = new Node;
	temp->character = c;
	temp->weight = prty;
	temp->left = temp->right = NULL;
	return temp;
}

void read(int& n, Node* arr[NMAX]) {
	cin >> n;
	for (int i = 0; i < n; ++i) {
		char c;
		int p;
		cin >> c >> p;
		arr[i] = newNode(c, p);
	}
}

void initPriorityQueue(PriorityQueue& pq) {
	pq.heapSize = 0;
	pq.capacity = NMAX;
	pq.harr = new Node*[pq.capacity];
}

inline int parent(int i) {
	return (i - 1) / 2;
}

inline int leftSon(int i) {
	return 2 * i + 1;
}

inline int rightSon(int i) {
	return 2 * i + 2;
}

void minHeapify(PriorityQueue pq, int i) {
	int l = leftSon(i);
	int r = rightSon(i);
	int smallest = i;
	if (l < pq.heapSize && pq.harr[l]->weight < pq.harr[smallest]->weight)
		smallest = l;
	if (r < pq.heapSize && pq.harr[r]->weight < pq.harr[smallest]->weight)
		smallest = r;
	if (smallest != i) {
		swap(pq.harr[i], pq.harr[smallest]);
		minHeapify(pq, smallest);
	}
}

Node* extractMin(PriorityQueue& pq) {
	if (pq.heapSize == 1) {
		--pq.heapSize;
		return pq.harr[0];
	}

	Node* root = pq.harr[0];
	pq.harr[0] = pq.harr[pq.heapSize - 1];
	--pq.heapSize;
	minHeapify(pq, 0);

	return root;
}

void decreaseKey(PriorityQueue pq, int i, char c, int prty) {
	pq.harr[i]->character = c;
	pq.harr[i]->weight = prty;
	while (i && pq.harr[parent(i)]->weight > pq.harr[i]->weight) {
		swap(pq.harr[i], pq.harr[parent(i)]);
		i = parent(i);
	}
}

char getMin(PriorityQueue pq) {
	return pq.harr[0]->character;
}

void deleteKey(PriorityQueue& pq, int i) {
	decreaseKey(pq, i, CHAR_MIN, INT_MIN);
	extractMin(pq);
}

void insertKey(PriorityQueue& pq, Node* node) {
	if (pq.heapSize == pq.capacity) {
		cout << "OVERFLOW\n";
		return;
	}

	++pq.heapSize;
	int i = pq.heapSize - 1;
	pq.harr[i] = node;
	while (i && pq.harr[parent(i)]->weight > pq.harr[i]->weight) {
		swap(pq.harr[i], pq.harr[parent(i)]);
		i = parent(i);
	}
}

inline bool isPriorityQueueEmpty(PriorityQueue pq) {
	return !pq.heapSize;
}

inline int sizePriorityQueue(PriorityQueue pq) {
	return pq.heapSize;
}

void pushToPriorityQueue(int n, Node* arr[NMAX], PriorityQueue& pq) {
	for (int i = 0; i < n; ++i)
		insertKey(pq, arr[i]);
}

Node* huffman(int n, Node* arr[NMAX], PriorityQueue pq) {
	while (sizePriorityQueue(pq) > 1) {
		Node* x = extractMin(pq);
		Node* y = extractMin(pq);
		Node* temp = newNode('0', x->weight + y->weight);

		temp->left = x;
		temp->right = y;
		insertKey(pq, temp);
	}
	return extractMin(pq);
}

void asignCode(Node* root, int code[], int lvl) {
	if (root->left) {
		code[lvl] = 0;
		asignCode(root->left, code, lvl + 1);
	}
	if (root->right) {
		code[lvl] = 1;
		asignCode(root->right, code, lvl + 1);
	}
	if (!root->left && !root->right) {
		// este frunza
		cout << root->character<< " : ";
		for (int i = 0; i < lvl; ++i)
			cout << code[i];
		cout << "\n";
	}
}

void inorder(Node* root) {
	if (root) {
		inorder(root->left);
		cout << root->character << " ";
		inorder(root->right);
	}
}

int main() {
	int n;
	Node* arr[NMAX];
	PriorityQueue pq;
	pq.harr = NULL;

	read(n, arr);
	initPriorityQueue(pq);
	pushToPriorityQueue(n, arr, pq);

	Node* root = huffman(n, arr, pq);
	
	int code[NMAX];
	asignCode(root, code, 0);
	
	return 0;
}
