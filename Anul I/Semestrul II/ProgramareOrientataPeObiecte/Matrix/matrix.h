//
// Created by Dany on 11.03.2019.
//

#ifndef PROJECT1_OOP_MATRIX_H
#define PROJECT1_OOP_MATRIX_H

#include <iostream>

using namespace std;

class Matrix {
    int cntLines_, cntColumns_;
    double** data_;
public:
    Matrix();
    Matrix(int nrLines, int nrColumns, int value = 0);
    Matrix(const Matrix& M);
    ~Matrix();
    friend istream& operator >> (istream& in, Matrix& M);
    friend ostream& operator << (ostream& out, const Matrix& M);
    Matrix& operator = (const Matrix& M);
    friend Matrix operator + (Matrix A, Matrix B);
    friend Matrix operator * (Matrix A, Matrix B);
    friend Matrix operator * (Matrix A, double number);
    friend Matrix operator * (double number, Matrix A);
    friend Matrix operator - (Matrix A, Matrix B);
    double* operator [] (int line);
    friend double determinant(Matrix M, int dim);
    int getNumberOfLines();
    int getNumberOfColumns();

    Matrix transpose();
    friend Matrix operator ^ (Matrix A, int power);
    Matrix inverse();
};

#endif //PROJECT1_OOP_MATRIX_H
