#include <iostream>

using namespace std;

const int NMAX = 1005;

void readArray(int arr[], int& dimArr, int& elem) {
    cin >> dimArr;
    for (int i = 0; i < dimArr; ++ i)
        cin >> arr[i];
    cin >> elem;
}

int binarySearch(int arr[], int dimArr, int elem) {
    int lo = 0, hi = dimArr - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == elem)
            return mid;
        if (arr[mid] < elem)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return -1;
}

int main() {
    int arr[NMAX], dimArr, elem;

    readArray(arr, dimArr, elem);

    int posElem = binarySearch(arr, dimArr, elem);
    cout << posElem << "\n";

    return 0;
}
