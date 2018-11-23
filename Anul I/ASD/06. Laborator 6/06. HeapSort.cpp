#include "pch.h"
#include <iostream>

#define NMAX 1000

using namespace std;

void readArray(int arr[], int& dimArr) {
	cin >> dimArr;
	for (int i = 0; i < dimArr; ++i)
		cin >> arr[i];
}

void maxHeapify(int arr[], int dimArr, int idx) {
	int leftIdx = 2 * idx + 1;
	int rightIdx = 2 * idx + 2;
	int maxIdx = idx;
	if (leftIdx < dimArr && arr[leftIdx] > arr[maxIdx])
		maxIdx = leftIdx;
	if (rightIdx < dimArr && arr[rightIdx] > arr[maxIdx])
		maxIdx = rightIdx;
	if (maxIdx != idx) {
		swap(arr[idx], arr[maxIdx]);
		maxHeapify(arr, dimArr, maxIdx);
	}
}

void buildMaxHeap(int arr[], int dimArr) {
	for (int i = dimArr / 2 - 1; i >= 0; --i)
		maxHeapify(arr, dimArr, i);
}

void heapsort(int arr[], int dimArr) {
	buildMaxHeap(arr, dimArr);
	for (int i = dimArr - 1; i >= 0; --i) {
		swap(arr[0], arr[i]);
		maxHeapify(arr, i, 0);
	}
}

void printArray(int arr[], int dimArr) {
	for (int i = 0; i < dimArr; ++i)
		cout << arr[i] << " ";
	cout << "\n";
}

int main() {
	int arr[NMAX], dimArr;
	readArray(arr, dimArr);
	heapsort(arr, dimArr);
	printArray(arr, dimArr);
	return 0;
}
