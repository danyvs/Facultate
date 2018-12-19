#include "pch.h"
#include <iostream>
#include <queue>

using namespace std;

#define NMAX 100

struct Node {
	int info;
	Node* next;
};

void citireGrafMatrice(int GM[NMAX][NMAX], int& n) {
	int x, y;
	cin >> n;
	for (int i = 1; i <= n; ++i)
		for (int j = 1; j <= n; ++j)
			GM[i][j] = 0;
	while (cin >> x >> y)
		GM[x][y] = GM[y][x] = 1;
}

void adaugareNodLista(Node* &first, int val) {
	Node* temp = new Node;
	temp->info = val;
	temp->next = first;
	first = temp;
}

void citireGrafListe(Node* G[NMAX], int& n) {
	int x, y;
	cin >> n;
	while (cin >> x >> y) {
		adaugareNodLista(G[x], y);
		adaugareNodLista(G[y], x);
	}
}

int grad(Node* G[NMAX], int x) {
	int cnt = 0;
	for (Node* p = G[x]; p; p = p->next)
		++cnt;
	return cnt;
}

void maxGrad(Node* G[NMAX], int n) {
	int dg[NMAX];
	for (int i = 1; i <= n; ++i)
		dg[i] = grad(G, i);

	int dgMax = 0;
	for (int i = 1; i <= n; ++i)
		dgMax = max(dgMax, dg[i]);
	for (int i = 1; i <= n; ++i)
		if (dg[i] == dgMax)
			cout << i << " ";
	cout << "\n";
}

int numarMuchii(Node* G[NMAX], int n) {
	int nrMuchii = 0;
	for (int i = 1; i <= n; ++i)
		nrMuchii += grad(G, i);
	return nrMuchii / 2;
}

void dfs(Node* G[NMAX], int node, bool seen[NMAX]) {
	cout << node << " ";
	seen[node] = true;
	for (Node* p = G[node]; p; p = p->next)
		if (!seen[p->info])
			dfs(G, p->info, seen);
}

void bfs(Node* G[NMAX], int node) {
	bool seen[NMAX] = { false };
	queue<int> q;
	q.push(node);
	seen[node] = true;
	while (!q.empty()) {
		int currNode = q.front();
		cout << currNode << " ";
		q.pop();
		for (Node* p = G[currNode]; p; p = p->next)
			if (!seen[p->info]) {
				q.push(p->info);
				seen[p->info] = true;
			}
	}
}

int main() {
	int n;
	Node* G[NMAX] = { NULL };
	
	// int GM[NMAX][NMAX];
	// citireGrafMatrice(GM, n);

	citireGrafListe(G, n);

	maxGrad(G, n);
	cout << numarMuchii(G, n) << "\n";
	
	bool seen[NMAX] = {false};
	dfs(G, 1, seen);
	cout << "\n";
	bfs(G, 1);
	cout << "\n";
	
	return 0;
}
