//
// Created by Dany on 11.03.2019.
//

#include <iostream>
#include "matrix.h"

using namespace std;

/**
 * Constructor without parameters for class Matrix
 */
Matrix::Matrix() {
    cntLines_ = cntColumns_ = 0;
    data_ = nullptr;
}

/**
 * Constructor with parameters for class Matrix
 * Initializes the number of lines, the number of columns, allocates memory for the data and
 * initializes the data with a given value
 * @param nrLines - the number of line
 * @param nrColumns - the number of columns
 * @param value - the value tot initialize the data with
 */
Matrix::Matrix(int nrLines, int nrColumns, int value) {
    // initialize dimensions
    cntLines_ = nrLines;
    cntColumns_ = nrColumns;

    // allocate memory
    data_ = new double*[cntLines_];
    for (int i = 0; i < cntLines_; ++i)
        data_[i] = new double[cntColumns_];

    // initialize data
    for (int i = 0; i < cntLines_; ++i)
        for (int j = 0; j < cntColumns_; ++j)
            data_[i][j] = value;
}

/**
 * Copy constructor
 * @param M - Matrix
 */
Matrix::Matrix(const Matrix &M) {
    // initialize dimensions
    cntLines_ = M.cntLines_;
    cntColumns_ = M.cntColumns_;

    // allocate memory
    data_ = new double*[cntLines_];
    for (int i = 0; i < cntLines_; ++i)
        data_[i] = new double[cntColumns_];

    // copy the data
    for (int i = 0; i < cntLines_; ++i)
        for (int j = 0; j < cntColumns_; ++j)
            data_[i][j] = M.data_[i][j];
}

/**
 * Destructor for class Matrix
 */
Matrix::~Matrix() {
    for (int i = 0; i < cntLines_; ++i)
        delete[] data_[i];
    delete[] data_;
}

/**
 * Overload operator >> to make it read a Matrix
 * @param in - input stream
 * @param M - Matrix
 * @return input stream in
 */
istream& operator >> (istream& in, Matrix& M) {
    in >> M.cntLines_ >> M.cntColumns_;

    M.data_ = new double*[M.cntLines_];
    for (int i = 0; i < M.cntLines_; ++i)
        M.data_[i] = new double[M.cntColumns_];

    for (int i = 0; i < M.cntLines_; ++i)
        for (int j = 0; j < M.cntColumns_; ++j)
            in >> M.data_[i][j];

    return in;
}

/**
 * Overload operator << to make it print a Matrix
 * @param out - output stream
 * @param M - const Matrix
 * @return - output stream out
 */
ostream& operator << (ostream& out, const Matrix& M) {
    out << M.cntLines_ << " " << M.cntColumns_ << "\n";
    for (int i = 0; i < M.cntLines_; ++i) {
        for (int j = 0; j < M.cntColumns_; ++j)
            out << M.data_[i][j] << " ";
        out << "\n";
    }

    return out;
}

/**
 * Overload operator = for matrices
 * @param other - Matrix to assign with
 * @return *this
 */
Matrix& Matrix::operator = (const Matrix& other) {
    // check not to do assignment with itself
    if (&other != this) {
        // destroy the old one
        for (int i = 0; i < cntLines_; ++i)
            delete[] data_[i];
        delete data_;

        // make the new one
        cntLines_ = other.cntLines_;
        cntColumns_ = other.cntColumns_;

        // allocate memory
        data_ = new double*[cntLines_];
        for (int i = 0; i < cntLines_; ++i)
            data_[i] = new double[cntColumns_];

        // copy the content
        for (int i = 0; i < cntLines_; ++i)
            for (int j = 0; j < cntColumns_; ++j)
                data_[i][j] = other.data_[i][j];
    }
    return *this;
}

/**
 * Overload operator + to make it do the sum of 2 matrices
 * @param A - the first Matrix
 * @param B - the second Matrix
 * @return Matrix C, the sum of Matrix A and Matrix B
 */
Matrix operator + (Matrix A, Matrix B) {
    // check if the matrices have the same dimensions
    if (A.cntLines_ != B.cntLines_ || A.cntColumns_ != B.cntColumns_) {
        cout << "In order to add 2 matrices, they must have the same number of "
                "lines and columns!\n";
        exit(EXIT_FAILURE);
    }

    // we know for sure that matrices have the same dimensions
    Matrix C(A.cntLines_, A.cntColumns_);

    // add
    for (int i = 0; i < C.cntLines_; ++i)
        for (int j = 0; j < C.cntColumns_; ++j)
            C.data_[i][j] = A.data_[i][j] + B.data_[i][j];

    return C;
}

/**
 * Overload operator * to make it do the multiplication of 2 matrices
 * @param A - the first Matrix
 * @param B - the second Matrix
 * @return the multiplication Matrix
 */
Matrix operator * (Matrix A, Matrix B) {
    // check if the number of columns of A is equal to the number of lines of B
    if (A.cntColumns_ != B.cntLines_) {
        cout << "In order to multiply 2 matrices, the number of columns of the first matrix must be "
                "equal to the number of lines of the second matrix!\n";
        exit(EXIT_FAILURE);
    }

    // we know for sure that we can multiply the matrices
    Matrix C(A.cntLines_, B.cntColumns_, 0);

    // multiplication
    for (int i = 0; i < C.cntLines_; ++i)
        for (int j = 0; j < C.cntColumns_; ++j)
            for (int k = 0; k < A.cntColumns_; ++k)
                C.data_[i][j] += A.data_[i][k] * B.data_[k][j];

    return C;
}

/**
 * Overload operator * to make it do the multiplication between a Matrix and a number
 * @param A - the Matrix
 * @param number - the double to multiply with
 * @return B, the multiplication Matrix
 */
Matrix operator * (Matrix A, double number) {
    Matrix B(A.cntLines_, A.cntColumns_);

    // multiplication
    for (int i = 0; i < B.cntLines_; ++i)
        for (int j = 0; j < B.cntColumns_; ++j)
            B.data_[i][j] = A.data_[i][j] * number;

    return B;
}

/**
 * Overload operator * to make it do the multiplication between a number and a Matrix
 * @param number - the double to multiply with
 * @param A - the Matrix
 * @return number * A (which is equal to A * number)
 */
Matrix operator * (double number, Matrix A) {
    return A * number;
}

/**
 * Overload operator - to make it do the difference of 2 matrices
 * @param A - the first Matrix
 * @param B - the second Matrix
 * @return Matrix C, the difference of Matrix A and Matrix B
 */
Matrix operator - (Matrix A, Matrix B) {
    // check if the matrices have the same dimensions
    if (A.cntLines_ != B.cntLines_ || A.cntColumns_ != B.cntColumns_) {
        cout << "In order to subtract 2 matrices, they must have the same number of "
                "lines and columns!\n";
        exit(EXIT_FAILURE);
    }

    // we know for sure that matrices have the same dimensions
    Matrix C(A.cntLines_, A.cntColumns_);

    // subtract
    for (int i = 0; i < C.cntLines_; ++i)
        for (int j = 0; j < C.cntColumns_; ++j)
            C.data_[i][j] = A.data_[i][j] - B.data_[i][j];

    return C;
}

/**
 * Overload operator [] to make it get a line of the Matrix
 * @param line - int, the line to get
 * @return double*, the line of the matrix
 */
double* Matrix::operator [](int line) {
    return data_[line];
}

/**
 * Calculate the determinant of a Matrix, using a recursive function
 * For each element of first line, remove its line and its column and then multiply the element
 * with the determinant of the corresponding matrix, and finally add them with alternate signs
 * The base case is a Matrix of 1 * 1, which determinant is the only element
 * @param M - Matrix for which we calculate the determinant
 * @param dim - the dimension of the Matrix for which we calculate the determinant
 * @return det, a double which represents the determinant of the matrix
 */
double determinant(Matrix M, int dim) {
    // check if the Matrix has the same dimensions
    if (M.cntLines_ != M.cntColumns_) {
        cout << "Determinant of a Matrix is defined only for square matrices!\n";
        exit(EXIT_FAILURE);
    }

    // base case
    if (dim == 1)
        return M.data_[0][0];

    double det = 0; // the determinant of the matrix
    int sign = 1; // the sign to multiply with


    for (int col = 0; col < M.cntColumns_; ++col) {
        // get the Matrix resulted by eliminating the line and the column of the current element
        Matrix temp(M.cntLines_ - 1, M.cntColumns_ - 1);
        int cntL = 0, cntC = 0;
        for (int i = 1; i < M.cntLines_; ++i)
            for (int j = 0; j < M.cntColumns_; ++j)
                if (j != col) {
                    temp.data_[cntL][cntC++] = M.data_[i][j];
                    if (cntC == M.cntColumns_ - 1) {
                        ++cntL;
                        cntC = 0;
                    }
                }

        det += sign * M.data_[0][col] * determinant(temp, dim - 1);
        sign = -sign; // alternate the sign
    }

    return det;
}

/**
 * Getter for the number of lines of the matrix
 * @return cntLines_, the number of lines
 */
int Matrix::getNumberOfLines() {
    return cntLines_;
}

/**
 * Getter for the number of columns of the matrix
 * @return cntColumns_, the number of columns
 */
int Matrix::getNumberOfColumns() {
    return cntColumns_;
}

/**
 * Determine the nth power of a matrix (exponentiation of a matrix)
 * Recursive function with exponentiation by squaring, which takes only log2(power) time
 * @param A - Matrix to be multiplied
 * @param power - the exponent of the multiplication
 * @return the Matrix (A ^ power)
 */
Matrix operator ^ (Matrix A, int power) {
    // check if the matrix is square
    if (A.cntLines_ != A.cntColumns_) {
        cout << "In order to calculate the power of a matrix, the matrix must be square!\n";
        exit(EXIT_FAILURE);
    }

    if (power == 1)
        return A;
    else {
        if (power % 2)
            return A * (A ^ (power / 2)) * (A ^ (power / 2));
        else
            return (A ^ (power / 2)) * (A ^ (power / 2));
    }
}

/**
 * Determine the transpose of a matrix
 * Change the lines with the columns
 * @return transposeMatrix, the transpose of the matrix given
 */
Matrix Matrix::transpose() {
    Matrix transposeMatrix(cntColumns_, cntLines_);
    for (int i = 0; i < transposeMatrix.cntLines_; ++i)
        for (int j = 0; j < transposeMatrix.cntColumns_; ++j)
            transposeMatrix.data_[i][j] = data_[j][i];
    return transposeMatrix;
}

/**
 * Determine the inverse of a matrix
 * @return inverseMatrix, the inverse of the Matrix
 */
Matrix Matrix::inverse() {
    // check if the matrix is square
    if (cntLines_ != cntColumns_) {
        cout << "In order for a matrix to be invertible, it must be square!\n";
        exit(EXIT_FAILURE);
    }

    // check if the determinant of the matrix is not equal with 0
    double det = determinant(*this, cntLines_);
    if (det == 0) {
        cout << "In order for a matrix to be invertible, it must have the determinant different "
                "from 0!\n";
        exit(EXIT_FAILURE);
    }

    Matrix transposeMatrix = transpose();
    Matrix inverseMatrix(cntLines_, cntColumns_);

    for (int lin = 0; lin < cntLines_; ++lin)
        for (int col = 0; col < cntColumns_; ++col) {
            Matrix temp(cntLines_ - 1, cntColumns_ - 1);
            int cntL = 0, cntC = 0;
            for (int i = 0; i < cntLines_; ++i)
                if (i != lin)
                    for (int j = 0; j < cntColumns_; ++j)
                        if (j != col) {
                            temp.data_[cntL][cntC++] = transposeMatrix.data_[i][j];
                            if (cntC == cntColumns_ - 1) {
                                ++cntL;
                                cntC = 0;
                            }
                        }

            int sign = ((lin + col) % 2) ? -1 : 1;
            inverseMatrix.data_[lin][col] = sign * determinant(temp, temp.cntLines_) / det;
        }

    return inverseMatrix;
}