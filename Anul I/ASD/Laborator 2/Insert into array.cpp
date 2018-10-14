#include <iostream>

using namespace std;

const int NMAX = 1005;

void read(int arr[], int& dimArr, int arrToInsert[], int& dimArrToInsert, int& elemAfterToInsert) {
    cout << "Dati numarul de elemente ale vectorului initial: ";
    cin >> dimArr;

    cout << "Dati vectorul initial: ";
    for (int i = 0; i < dimArr; ++ i)
        cin >> arr[i];

    cout << "Dati numarul de element al vectorului care trebuie inserat: ";
    cin >> dimArrToInsert;

    cout << "Dati vectorul care trebuie inserat: ";
    for (int i = 0; i < dimArrToInsert; ++ i)
        cin >> arrToInsert[i];

    cout << "Dati elementul dupa care trebuie inserat: ";
    cin >> elemAfterToInsert;
}

int copyArray(int src[], int dimSrc, int dest[], int& dimDest) {
    for (int i = 0; i < dimSrc; ++ i)
        dest[i] = src[i];
    dimDest = dimSrc;
}

void insertArrayIntoArray(int arr[], int& dimArr, int arrToInsert[], int dimArrToInsert, int elemAfterToInsert) {
    int idx = 0;
    // cauta indicele la care se afla prima aparitie a elementului elemAfterToInsert
    while (idx < dimArr && arr[idx] != elemAfterToInsert)
        ++ idx;
    if (idx == dimArr) {
        // elementul nu a fost gasit
        cout << elemAfterToInsert << " nu a fost gasit in vectorul dat!\n";
        return;
    }

    // mutare elemente de dupa elenAfterToInsert cu dimArrToInsert pozitii spre dreapta
    for (int i = dimArr - 1; i > idx; -- i)
        arr[i + dimArrToInsert] = arr[i];

    // inserare vector
    for (int i = 0; i < dimArrToInsert; ++ i)
        arr[idx + i + 1] = arrToInsert[i];

    // actualizare numar elemente vector initial
    dimArr += dimArrToInsert;
}

void printArray(int arr[], int dimArr) {
    for (int i = 0; i < dimArr; ++ i)
        cout << arr[i] << " ";
    cout << "\n";
}

int main() {
    int arr[2 * NMAX], dimArr, arrToInsert[NMAX], dimArrToInsert, elemAfterToInsert;
    int tempArr[NMAX], dimTempArr;

    read(arr, dimArr, arrToInsert, dimArrToInsert, elemAfterToInsert);
    cout << "\n";

    copyArray(arr, dimArr, tempArr, dimTempArr);
    insertArrayIntoArray(arr, dimArr, arrToInsert, dimArrToInsert, elemAfterToInsert);

    cout << "Vectorul initial era: ";
    printArray(tempArr, dimTempArr);
    if (dimArr == dimTempArr)
        cout << "Vectorul nu a fost modificat! ";
    cout << "Noul vector este: ";
    printArray(arr, dimArr);

    return 0;
}
