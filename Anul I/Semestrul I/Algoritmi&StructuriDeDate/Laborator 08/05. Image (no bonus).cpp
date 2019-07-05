#include "pch.h"
#include <iostream>

#define NMAX 100

using namespace std;

struct QueueNode {
	int info;
	QueueNode *next;
};

void readMatrix(int matrix[][NMAX], int& dim) {
	cin >> dim;
	for (int i = 0; i < dim; ++i)
		for (int j = 0; j < dim; ++j)
			cin >> matrix[i][j];
}

void pushQueue(QueueNode* &front, QueueNode* &tail, int data) {
	QueueNode *node = new QueueNode;
	node->info = data;
	node->next = NULL;
	if (!front)
		front = tail = node;
	else {
		tail->next = node;
		tail = node;
	}
}

inline bool isQueueEmpty(QueueNode* front, QueueNode* tail) {
	return !front;
}

inline int frontQueue(QueueNode* front, QueueNode* tail) {
	if (isQueueEmpty(front, tail)) {
		cout << "Coada este vida!\n";
		return -1;
	}
	return front->info;
}

void popQueue(QueueNode* &front, QueueNode* &tail) {
	if (isQueueEmpty(front, tail)) {
		cout << "Coada este vida!\n";
		return;
	}
	front = front->next;
}

void colorPixels(int matrix[][NMAX], int dim, int linie, int coloana, int color) {
	QueueNode *frontLin = NULL, *tailLin = NULL, *frontCol = NULL, *tailCol = NULL;
	pushQueue(frontLin, tailLin, linie);
	pushQueue(frontCol, tailCol, coloana);
	matrix[linie][coloana] = color;
	int dx[] = { -1, 0, 1, 0 };
	int dy[] = { 0, -1, 0, 1 };
	while (!isQueueEmpty(frontLin, tailLin)) {
		int linCur = frontQueue(frontLin, tailLin);
		int colCur = frontQueue(frontCol, tailCol);
		popQueue(frontLin, tailLin);
		popQueue(frontCol, tailCol);
		for (int i = 0; i < 4; ++i) {
			int linVec = linCur + dx[i];
			int colVec = colCur + dy[i];
			if (linVec >= 0 && linVec < dim && colVec >= 0 && colVec < dim && matrix[linVec][colVec] == 1) {
				pushQueue(frontLin, tailLin, linVec);
				pushQueue(frontCol, tailCol, colVec);
				matrix[linVec][colVec] = color;
			}
		}
	}
}

void solve(int matrix[][NMAX], int dim) {
	int color = 2;
	for (int i = 0; i < dim; ++i)
		for (int j = 0; j < dim; ++j)
			if (matrix[i][j] == 1) {
				colorPixels(matrix, dim, i, j, color);
				++color;
			}
}

void printMatrix(int matrix[][NMAX], int dim) {
	for (int i = 0; i < dim; ++i) {
		for (int j = 0; j < dim; ++j)
			cout << matrix[i][j] << " ";
		cout << "\n";
	}
}

int main() {
	int matrix[NMAX][NMAX], dim;
	readMatrix(matrix, dim);
	solve(matrix, dim);
	printMatrix(matrix, dim);
	return 0;
}
