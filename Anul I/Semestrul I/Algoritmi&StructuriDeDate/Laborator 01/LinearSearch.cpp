#include <iostream>

using namespace std;

const int NMAX = 1005;

void readArray(int arr[], int& dimArr, int& elem) {
    cin >> dimArr;
    for (int i = 0; i < dimArr; ++ i)
        cin >> arr[i];
    cin >> elem;
}

int linearSearch(int arr[], int dimArr, int elem) {
    for (int i = 0; i < dimArr; ++ i)
        if (arr[i] == elem)
            return i;
    return -1;
}

int main() {
    int arr[NMAX], dimArr, elem;

    readArray(arr, dimArr, elem);

    int posElem = linearSearch(arr, dimArr, elem);
    cout << posElem << "\n";

    return 0;
}
