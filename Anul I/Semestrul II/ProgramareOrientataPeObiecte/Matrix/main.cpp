#include <iostream>
#include "matrix.h"


using namespace std;

int main() {
    Matrix A, B; // constructor without parameters
    cin >> A >> B; // overload operator >>

    Matrix C;

    C = A + B; // overload operator +
    cout << "The sum of matrices A and B is:\n" << C << "\n"; // overload operator <<

    C = A * B; // overload operator *
    cout << "The multiplication of matrices A and B is:\n" << C << "\n";

    double number;
    cin >> number;

    C = A * number; // overload operator *
    cout << "The multiplication of matrix A with a number is:\n" << C << "\n";

    C = number * A; // overload operator *
    cout << "The multiplication of a number with matrix A is:\n" << C << "\n";

    C = A - B; // overload operator -
    cout << "The difference of matrices A and B is:\n" << C << "\n";

    cout << A[0][0] << "\n"; // overload operator []
    A[0][0] = number; // overload operator []
    cout << A[0][0] << "\n";

    // determinant
    cout << "The determinant of matrix A is " << determinant(A, A.getNumberOfLines()) << "\n";

    // number of lines and columns
    cout << "The number of lines is: " << A.getNumberOfLines() << "\n";
    cout << "The number of columns is: " << A.getNumberOfColumns() << "\n";

    C = A ^ number; // overload operator ^
    cout << "The power of matrix A is:\n" << C << "\n";

    cout << "The transpose of matrix A is\n" << A.transpose() << "\n";

    cout << "The inverse of matrix A is\n" << A.inverse() << "\n";

    Matrix D(3, 4, 0); // constructor with parameters
    cout << "Matrix D is:\n" << D << "\n";

    Matrix E(A); // copy constructor
    cout << "Matrix E is:\n" << E << "\n";

    C = A; // overload operator =
    cout << "Matrix C is:\n" << C << "\n";

    return 0;
}