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

void selectionSort(int arr[], int dimArr) {
    for (int i = 0; i < dimArr - 1; ++ i) {
        int idxMin = i;
        for (int j = i + 1; j < dimArr; ++ j)
            if (arr[j] < arr[idxMin])
                idxMin = j;
        swapInts(arr[i], arr[idxMin]);
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
    selectionSort(arr, dimArr);
    printArray(arr, dimArr);

    return 0;
}
