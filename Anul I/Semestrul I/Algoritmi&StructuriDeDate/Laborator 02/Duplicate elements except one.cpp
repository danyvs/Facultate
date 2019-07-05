#include <iostream>

using namespace std;

const int NMAX = 1005;

void readArray(int arr[], int& dimArr) {
    cout << "Dati numarul de elemente al vectorului: ";
    cin >> dimArr;

    cout << "Dati elementele vectorului: ";
    for (int i = 0; i < dimArr; ++ i)
        cin >> arr[i];
}

int findSingle(int arr[], int dimArr) {
    int elem = 0;
    for (int i = 0; i < dimArr; ++ i)
        elem ^= arr[i];
    return elem;
}

int main() {
    int arr[NMAX], dimArr;

    readArray(arr, dimArr);

    cout << "\n";
    cout << "Elementul care apare o singura data este: " << findSingle(arr, dimArr);

    return 0;
}
