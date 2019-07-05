#include <iostream>

using namespace std;

const int NMAX = 1005;

struct min_max {
    int mn, mx;
};

void readArray(int arr[], int& dimArr) {
    cout << "Dati numarul de elemente al vectorului: ";
    cin >> dimArr;

    cout << "Dati vectorul: ";
    for (int i = 0; i < dimArr; ++ i)
        cin >> arr[i];
}

inline int minInt(const int& a, const int& b) {
    return a < b ? a : b;
}

inline int maxInt(const int& a, const int& b) {
    return a > b ? a : b;
}

min_max getMinMax(int arr[], int dimArr) {
    int i;
    min_max sol;

    // initializare minim si maxim
    if (dimArr % 2) {
        // vectorul are numar impar de elemente
        sol.mn = sol.mx = arr[0];
        i = 1;
    }
    else {
        // vectorul are numar par de elemente
        if (arr[0] > arr[1]) {
            sol.mn = arr[1];
            sol.mx = arr[0];
        }
        else {
            sol.mn = arr[0];
            sol.mx = arr[1];
        }
        i = 2;
    }

    while (i < dimArr) {
        if (arr[i] > arr[i + 1]) {
            sol.mn = minInt(sol.mn, arr[i + 1]);
            sol.mx = maxInt(sol.mx, arr[i]);
        }
        else {
            sol.mn = minInt(sol.mn, arr[i]);
            sol.mx = maxInt(sol.mx, arr[i + 1]);
        }
        i += 2;
    }

    return sol;
}

int main() {
    int arr[NMAX], dimArr;

    readArray(arr, dimArr);

    min_max sol = getMinMax(arr, dimArr);
    cout << "Valoarea minima este " << sol.mn << "\n";
    cout << "Valoarea maxima este " << sol.mx << "\n";

    return 0;
}
