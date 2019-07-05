#include "pch.h"
#include <iostream>

#define NMAX 1000

using namespace std;

void readArray(int arr[], int& dimArr) {
	cin >> dimArr;
	for (int i = 0; i < dimArr; ++i)
		cin >> arr[i];
}

int partition(int arr[], int lo, int hi) {
	int pivot = arr[hi];
	int i = lo - 1;
	for (int j = lo; j < hi; ++j)
		if (arr[j] < pivot)
			swap(arr[++i], arr[j]);
	swap(arr[i + 1], arr[hi]);
	return i + 1;
}

void quickSort(int arr[], int lo, int hi) {
	if (lo < hi) {
		int pi = partition(arr, lo, hi);
		quickSort(arr, lo, pi - 1);
		quickSort(arr, pi + 1, hi);
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
	quickSort(arr, 0, dimArr - 1);
	printArray(arr, dimArr);
	return 0;
}
