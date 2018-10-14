#include <iostream>

using namespace std;

const int NMAX = 1005;

void read(int arr[], int& dimArr, int& elem) {
    cout << "Dati numarul de elemente al vectorului initial: ";
    cin >> dimArr;

    cout << "Dati vectorul initial: ";
    for (int i = 0; i < dimArr; ++ i)
        cin >> arr[i];

    cout << "Dati elementul care trebuie eliminat: ";
    cin >> elem;
}

void copyArray(int arr[], int dimArr, int tempArr[], int& dimTempArr) {
    dimTempArr = dimArr;
    for (int i = 0; i < dimArr; ++ i)
        tempArr[i] = arr[i];
}

void deleteElem(int arr[], int& dimArr, int elem) {
    int dif = 0;
    for (int i = 0; i < dimArr; ++ i)
        if (arr[i] == elem)
            ++ dif;
        else
            arr[i - dif] = arr[i];
    dimArr -= dif;
}

void printArray(int arr[], int dimArr) {
    for (int i = 0; i < dimArr; ++ i)
        cout << arr[i] << " ";
    cout << "\n";
}

int main() {
    int arr[NMAX], dimArr, elem;
    int tempArr[NMAX], dimTempArr;

    read(arr, dimArr, elem);

    copyArray(arr, dimArr, tempArr, dimTempArr);
    deleteElem(tempArr, dimTempArr, elem);

    cout << "\n";
    cout << "Vectorul initial era: ";
    printArray(arr, dimArr);

    if (dimTempArr == dimArr)
        cout << "Vectorul initial nu a fost modificat! ";
    cout << "Noul vector este: ";
    printArray(tempArr, dimTempArr);

    return 0;
}
