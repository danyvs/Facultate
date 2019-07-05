#include <iostream>

using namespace std;

const int NMAX = 1005;

void readArray(int arr[], int& dimArr) {
    cin >> dimArr;
    for (int i = 0; i < dimArr; ++ i)
        cin >> arr[i];
}

void insertionSort(int arr[], int dimArr) {
    for (int i = 1; i < dimArr; ++ i) {
        int j = i - 1, key = arr[i];
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            -- j;
        }
        arr[j + 1] = key;
    }
}

void printArray(int arr[], int dimArr) {
    for (int i = 0; i < dimArr; ++ i)
        cout << arr[i] << " ";
    cout << "\n";
}

int main() {
    int arr[NMAX], dimArr;

    readArray(arr, dimArr);
    insertionSort(arr, dimArr);
    printArray(arr, dimArr);

    return 0;
}
