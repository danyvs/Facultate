#include "pch.h"
#include <iostream>

using namespace std;

void initMinHeap(int * &harr, int capacity, int &heapSize) {
	heapSize = 0;
	harr = new int[capacity];
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

void minHeapify(int *harr, int heapSize, int i) {
	int l = leftSon(i);
	int r = rightSon(i);
	int smallest = i;
	if (l < heapSize && harr[l] < harr[smallest])
		smallest = l;
	if (r < heapSize && harr[r] < harr[smallest])
		smallest = r;
	if (smallest != i) {
		swap(harr[i], harr[smallest]);
		minHeapify(harr, heapSize, smallest);
	}
}

int extractMin(int *harr, int &heapSize) {
	if (heapSize <= 0)
		return -1;
	if (heapSize == 1) {
		--heapSize;
		return harr[0];
	}

	int root = harr[0];
	harr[0] = harr[heapSize - 1];
	--heapSize;
	minHeapify(harr, heapSize, 0);

	return root;
}

void decreaseKey(int *harr, int i, int newVal) {
	harr[i] = newVal;
	while (i && harr[parent(i)] > harr[i]) {
		swap(harr[i], harr[parent(i)]);
		i = parent(i);
	}
}

int getMin(int *harr) {
	return harr[0];
}

void deleteKey(int *harr, int &heapSize, int i) {
	decreaseKey(harr, i, -1);
	extractMin(harr, heapSize);
}

void insertKey(int *harr, int capacity, int &heapSize, int k) {
	if (heapSize == capacity) {
		cout << "OVERFLOW!\n";
		return;
	}

	++heapSize;
	int i = heapSize - 1;
	harr[i] = k;
	while (i && harr[parent(i)] > harr[i]) {
		swap(harr[i], harr[parent(i)]);
		i = parent(i);
	}
}

int main() {
	int *harr = NULL, heapSize, capacity = 11;
	initMinHeap(harr, capacity, heapSize);
	insertKey(harr, capacity, heapSize, 3);
	insertKey(harr, capacity, heapSize, 2);
	deleteKey(harr, heapSize, 1);
	insertKey(harr, capacity, heapSize, 15);
	insertKey(harr, capacity, heapSize, 5);
	insertKey(harr, capacity, heapSize, 4);
	insertKey(harr, capacity, heapSize, 45);
	cout << extractMin(harr, heapSize) << " ";
	cout << getMin(harr) << " ";
	decreaseKey(harr, 2, 1);
	cout << getMin(harr) << "\n";
	return 0;
}
