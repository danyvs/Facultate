#include "pch.h"
#include <iostream>

#define NMAX 1000
#define VALMAX 2000000000

using namespace std;

void readArray(int arr[], int& dimArr) {
	cin >> dimArr;
	for (int i = 0; i < dimArr; ++i)
		cin >> arr[i];
}

void merge(int arr[], int lo, int mid, int hi) {
	int L[NMAX], dimL = mid - lo + 1; // jumatatea din stanga
	for (int i = 0; i < dimL; ++i)
		L[i] = arr[lo + i];
	L[dimL] = VALMAX; // garda

	int R[NMAX], dimR = hi - mid; // jumatatea din dreapta
	for (int i = 0; i < dimR; ++i)
		R[i] = arr[mid + 1 + i];
	R[dimR] = VALMAX; // garda

	// interclasare
	for (int it = lo, i = 0, j = 0; it <= hi; ++it)
		arr[it] = (L[i] < R[j] ? L[i++] : R[j++]);
}

/*
void merge1(int arr[], int lo, int mid, int hi) {
	int idx1 = lo, idx2 = mid + 1;
	int tempArr[NMAX], dimTempArr = 0;
	while (idx1 <= mid && idx2 <= hi)
		tempArr[dimTempArr++] = (arr[idx1] < arr[idx2]) ? arr[idx1++] : arr[idx2++];
	while (idx1 <= mid)
		tempArr[dimTempArr++] = arr[idx1++];
	while (idx2 <= hi)
		tempArr[dimTempArr++] = arr[idx2++];

	for (int i = 0; i < dimTempArr; ++i)
		arr[lo + i] = tempArr[i];
}
*/

void mergeSort(int arr[], int lo, int hi) {
	if (lo < hi) {
		int mid = lo + (hi - lo) / 2;
		mergeSort(arr, lo, mid);
		mergeSort(arr, mid + 1, hi);
		merge(arr, lo, mid, hi);
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
	mergeSort(arr, 0, dimArr - 1);
	printArray(arr, dimArr);
	return 0;
}
