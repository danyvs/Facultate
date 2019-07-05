#include "pch.h"
#include <iostream>

#define NMAX 100

using namespace std;

void readArray(int &dimArr, int arr[]) {
	cin >> dimArr;
	for (int i = 0; i < dimArr; ++i)
		cin >> arr[i];
}

void copyArray(int dimArr, int dest[], int src[]) {
	for (int i = 0; i < dimArr; ++i)
		dest[i] = src[i];
}

void countSort(int dimArr, int arr[], int exp2) {
	int freq[16] = { 0 };

	// (arr[i] >> exp2) & 15 = (arr[i] / (2 ^ exp2)) % 16
	// freq[(arr[i] >> exp2) & 15] retine de cate ori apare elementul arr[i] % 16
	for (int i = 0; i < dimArr; ++i)
		++freq[(arr[i] >> exp2) & 15];

	// freq[i] retine cate numere sunt mai mici decat arr[i] % 16
	for (int i = 1; i < 16; ++i)
		freq[i] += freq[i - 1];

	int sortedArr[NMAX];
	for (int i = dimArr - 1; i >= 0; --i) {
		sortedArr[freq[(arr[i] >> exp2) & 15] - 1] = arr[i];
		--freq[(arr[i] >> exp2) & 15];
	}

	copyArray(dimArr, arr, sortedArr);
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

void radixSort(int dimArr, int arr[]) {
	int maxVal = getMax(dimArr, arr);
	int exp2 = 0;
	/* pentru a eficientiza algoritmul, numerele vor fi considerate in reprezentarea
	lor in baza 2, luandu-se cate 4 biti; din acest motiv, se va lucra modulo 16 */
	while (maxVal) {
		countSort(dimArr, arr, exp2);
		maxVal >>= 4;
		exp2 += 4;
	}
}

void printArray(int dimArr, int arr[]) {
	for (int i = 0; i < dimArr; ++i)
		cout << arr[i] << " ";
	cout << "\n";
}

int main() {
	int dimArr, arr[NMAX];
	readArray(dimArr, arr);
	radixSort(dimArr, arr);
	printArray(dimArr, arr);
	return 0;
}
