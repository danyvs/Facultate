#include <iostream>

using namespace std;

const int NMAX = 1005;

void readArray(int arr[], int& dimArr) {
    cin >> dimArr;
    for (int i = 0; i < dimArr; ++ i)
        cin >> arr[i];
}

void swapInts(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

void bubbleSort(int arr[], int dimArr) {
    bool sorted = false;
    int idx = 0, tempN = dimArr;
    while (!sorted) {
        sorted = true;
        for (int i = 0; i < dimArr - 1; ++ i)
            if (arr[i] > arr[i + 1]) {
                swapInts(arr[i], arr[i + 1]);
                sorted = false;
                idx = i + 1;
            }
        dimArr = idx;
    }
    dimArr = tempN;
}

void printArray(int arr[], int dimArr) {
    for (int i = 0; i < dimArr; ++ i)
        cout << arr[i] << " ";
    cout << "\n";
}

int main() {
    int arr[NMAX], dimArr;

    readArray(arr, dimArr);
    bubbleSort(arr, dimArr);
    printArray(arr, dimArr);

    return 0;
}
