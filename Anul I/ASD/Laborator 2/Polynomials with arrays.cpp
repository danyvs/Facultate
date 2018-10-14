#include <iostream>

using namespace std;

const int NMAX = 1005;

void readPoly(int poly[], int& degree) {
    cout << "Dati gradul polinomului: ";
    cin >> degree;

    cout << "Dati coeficientii polinomului: ";
    for (int i = degree; i >= 0; -- i)
        cin >> poly[i];
}

int valueOfPoly(int poly[], int degree, int value) {
    long long sol = 0;
    for (int i = 0, x = 1; i <= degree; ++ i, x *= value)
        sol += 1LL * poly[i] * x;
    return sol;
}

void printPoly(int poly[], int degree) {
    for (int i = degree; i >= 0; -- i)
        cout << poly[i] << " ";
    cout << "\n";
}

void addPolys(int P[], int degreeP, int Q[], int degreeQ) {
    int R[NMAX], degreeR;

    if (degreeP > degreeQ) {
        for (int i = degreeQ + 1; i <= degreeP; ++ i) {
            // completare cu zerouri
            Q[i] = 0;
        }
        degreeR = degreeP;
    }
    else {
        for (int i = degreeP + 1; i <= degreeQ; ++ i) {
            // completare cu zerouri
            P[i] = 0;
        }
        degreeR = degreeQ;
    }

    // adunare efectiva a coeficientilor
    for (int i = 0; i <= degreeR; ++ i)
        R[i] = P[i] + Q[i];

    cout << "Coeficientii vectorului suma sunt: ";
    printPoly(R, degreeR);
}

void multiplyPolys(int P[], int degreeP, int Q[], int degreeQ) {
    int R[2 * NMAX], degreeR;

    degreeR = degreeP + degreeQ;

    // initializare cu zerouri
    for (int i = 0; i <= degreeR; ++ i)
        R[i] = 0;

    for (int i = 0; i <= degreeP; ++ i)
        for (int j = 0; j <= degreeQ; ++ j)
            R[i + j] += P[i] * Q[j];

    cout << "Coeficientii vectorului produs sunt: ";
    printPoly(R, degreeR);
}

int main() {
    int P[NMAX], degreeP, Q[NMAX], degreeQ, value;

    readPoly(P, degreeP);

    cout << "\nDati valoarea pentru care sa se calculeze polinomul: ";
    cin >> value;
    cout << "Valoarea polinomului in " << value << " este " << valueOfPoly(P, degreeP, value) << "\n\n";

    readPoly(Q, degreeQ);
    cout << "\n";

    addPolys(P, degreeP, Q, degreeQ);
    cout << "\n";

    multiplyPolys(P, degreeP, Q, degreeQ);

    return 0;
}
