#include "pch.h"
#include <iostream>

using namespace std;

struct Elem {
	int value, priority;
};

void initMaxHeap(Elem* &harr, int capacity, int &heapSize) {
	heapSize = 0;
	harr = new Elem[capacity];
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

void maxHeapify(Elem *harr, int heapSize, int i) {
	int l = leftSon(i);
	int r = rightSon(i);
	int largest = i;
	if (l < heapSize && harr[l].priority > harr[largest].priority)
		largest = l;
	if (r < heapSize && harr[r].priority > harr[largest].priority)
		largest = r;
	if (largest != i) {
		swap(harr[i], harr[largest]);
		maxHeapify(harr, heapSize, largest);
	}
}

Elem extractMax(Elem *harr, int &heapSize) {
	if (heapSize <= 0)
		return harr[0];

	if (heapSize == 1) {
		--heapSize;
		return harr[0];
	}

	Elem root = harr[0];
	harr[0] = harr[heapSize - 1];
	--heapSize;
	maxHeapify(harr, heapSize, 0);

	return root;
}

/* Cresterea valorii elementului de la pozitia i */
void increaseKey(Elem *harr, int i, int val, int prty) {
	harr[i].value = val;
	harr[i].priority = prty;
	while (i && harr[parent(i)].priority < harr[i].priority) {
		swap(harr[i], harr[parent(i)]);
		i = parent(i);
	}
}

int getMax(Elem *harr) {
	return harr[0].value;
}

/* Sterge elementul cu indexul i */
void deleteKey(Elem *harr, int &heapSize, int i) {
	increaseKey(harr, i, INT_MAX, INT_MAX);
	extractMax(harr, heapSize);
}

void insertKey(Elem *harr, int capacity, int &heapSize, int val, int prty) {
	if (heapSize == capacity) {
		cout << "OVERFLOW!\n";
		return;
	}

	++heapSize;
	int i = heapSize - 1;
	harr[i].value = val;
	harr[i].priority = prty;
	while (i && harr[parent(i)].priority < harr[i].priority) {
		swap(harr[i], harr[parent(i)]);
		i = parent(i);
	}
}

int main() {
	Elem *harr = NULL;
	int heapSize, capacity = 100;
  
	return 0;
}
