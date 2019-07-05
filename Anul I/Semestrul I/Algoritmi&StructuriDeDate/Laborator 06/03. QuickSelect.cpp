#include "pch.h"
#include <iostream>

#define NMAX 1000

using namespace std;

void readArray(int arr[], int &dimArr, int &kthElem) {
	cin >> dimArr;
	for (int i = 0; i < dimArr; ++i)
		cin >> arr[i];
	cin >> kthElem;
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

int quickSelect(int arr[], int lo, int hi, int kthElem) {
	if (kthElem > 0 && kthElem <= hi - lo + 1) {
		int pi = partition(arr, lo, hi);
		if (pi - lo == kthElem - 1)
			return arr[pi];
		if (pi - lo > kthElem - 1)
			return quickSelect(arr, lo, pi - 1, kthElem);
		return quickSelect(arr, pi + 1, hi, kthElem - pi + lo - 1);
	}
	return -1;
}

int main() {
	int arr[NMAX], dimArr, kthElem;
	readArray(arr, dimArr, kthElem);
	cout << quickSelect(arr, 0, dimArr - 1, kthElem) << "\n";
	return 0;
}
