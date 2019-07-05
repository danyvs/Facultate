#include "pch.h"
#include <iostream>

#define NMAX 100
#define VMAX 1005

using namespace std;

void readArray(int &dimArr, int arr[]) {
	cin >> dimArr;
	for (int i = 0; i < dimArr; ++i)
		cin >> arr[i];
}

int max(const int& a, const int& b) {
	return a > b ? a : b;
}

/* Returneaza valoarea maxima dintr-un vector*/
int getMax(int dimArr, int arr[]) {
	int mx = arr[0];
	for (int i = 1; i < dimArr; ++i)
		mx = max(mx, arr[i]);
	return mx;
}

/* Copierea unui vector in alt vector */
void copyArray(int dimArr, int dest[], int src[]) {
	for (int i = 0; i < dimArr; ++i)
		dest[i] = src[i];
}

void countSort(int dimArr, int arr[]) {
	int maxVal = getMax(dimArr, arr);
	int freq[VMAX]; // vector de aparitii

	// initializare vector de aparitii cu 0
	for (int i = 0; i <= maxVal; ++i)
		freq[i] = 0;

	// freq[i] retine de cate ori apare elementul i
	for (int i = 0; i < dimArr; ++i)
		++freq[arr[i]];

	// freq[i] retine cate numere sunt mai mici decat i
	for (int i = 1; i <= maxVal; ++i)
		freq[i] += freq[i - 1];
	
	// se creeaza vectorul sortat
	int sortedArr[NMAX];
	for (int i = dimArr - 1; i >= 0; --i) {
		sortedArr[freq[arr[i]] - 1] = arr[i];
		--freq[arr[i]];
	}

	// vectorul sortat se copiaza in vectorul initial 
	copyArray(dimArr, arr, sortedArr);
}

void printArray(int dimArr, int arr[]) {
	for (int i = 0; i < dimArr; ++i)
		cout << arr[i] << " ";
	cout << "\n";
}

int main() { 
	int dimArr, arr[NMAX];
	readArray(dimArr, arr);
	countSort(dimArr, arr);
	printArray(dimArr, arr);
	return 0;
}
