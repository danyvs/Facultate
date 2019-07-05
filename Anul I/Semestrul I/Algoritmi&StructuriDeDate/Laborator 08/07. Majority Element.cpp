#include "pch.h"
#include <iostream>

#define NMAX 1000

using namespace std;

void readArray(int arr[], int &dimArr) {
	cin >> dimArr;
	for (int i = 0; i < dimArr; ++i)
		cin >> arr[i];
}

int findCandidate(int arr[], int dimArr) {
	int candidate = arr[0], cnt = 1;
	for (int i = 1; i < dimArr; ++i)
		if (arr[i] == candidate)
			++cnt;
		else {
			--cnt;
			if (cnt == 0) {
				candidate = arr[i];
				cnt = 1;
			}
		}
	return candidate;
}

bool isMajorityElement(int arr[], int dimArr, int candidate) {
	int cnt = 0;
	for (int i = 0; i < dimArr; ++i)
		if (arr[i] == candidate)
			++cnt;
	return (cnt > dimArr / 2);
}

void printMajorityElement(int arr[], int dimArr) {
	int candidate = findCandidate(arr, dimArr);
	if (isMajorityElement(arr, dimArr, candidate))
		cout << "Elementul majoritar este " << candidate << "\n";
	else
		cout << "Nu exista element majoritar!\n";
}

int main() {
	int arr[NMAX], dimArr;
	readArray(arr, dimArr);
	printMajorityElement(arr, dimArr);
	return 0;
}
