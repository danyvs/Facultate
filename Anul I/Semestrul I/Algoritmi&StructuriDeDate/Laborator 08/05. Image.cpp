#include "pch.h"
#include <iostream>

#define NMAX 100

using namespace std;

void readMatrix(int matrix[][NMAX], int &dim) {
	cin >> dim;
	for (int i = 0; i < dim; ++i)
		for (int j = 0; j < dim; ++j)
			cin >> matrix[i][j];
}

void initializeArray(int dim, int arr[], int val) {
	for (int i = 0; i < dim; ++i)
		arr[i] = val;
}

int root(int x, int par[]) {
	while (par[x])
		x = par[x];
	return x;
}

void unite(int x, int y, int ht[], int par[]) {
	if (ht[x] > ht[y])
		par[y] = x;
	else {
		par[x] = y;
		if (ht[x] == ht[y])
			++ht[y];
	}
}

void solve(int matrix[][NMAX], int dim) {
	int ht[NMAX * NMAX], par[NMAX * NMAX];
	initializeArray(dim * dim, ht, 1);
	initializeArray(dim * dim, par, 0);

	for (int i = 0; i < dim; ++i)
		for (int j = 0; j < dim; ++j)
			if (matrix[i][j] == 1) {
				int rootX, rootY;
				rootX = root(i * dim + j, par);
				if (i - 1 >= 0 && matrix[i - 1][j] == 1) {
					rootY = root((i - 1) * dim + j, par);
					if (rootX != rootY)
						unite(rootX, rootY, ht, par);
				}
				rootX = root(i * dim + j, par);
				if (j - 1 >= 0 && matrix[i][j - 1] == 1) {
					rootY = root(i * dim + j - 1, par);
					if (rootX != rootY)
						unite(rootX, rootY, ht, par);
				}
			}


	// colorare
	int col[NMAX * NMAX] = { 0 };
	int color = 2;
	for (int i = 0; i < dim; ++i)
		for (int j = 0; j < dim; ++j)
			if (matrix[i][j] == 1) {
				int r = root(i * dim + j, par);
				if (!col[r])
					col[r] = color++;
				matrix[i][j] = col[r];
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
	cout << "\n";
	printMatrix(matrix, dim);
	return 0;
}
