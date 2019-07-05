#include "pch.h"
#include <iostream>

#define NMAX 1000

using namespace std;

struct QueueNode {
	int info;
	QueueNode* next;
};

struct TrenLinie {
	int tren, linie;
};

void readArray(int arr[], int &n, int &nrLinii) {
	cin >> n >> nrLinii;
	for (int i = 0; i < n; ++i)
		cin >> arr[i];
}

void pushQueue(QueueNode* &front, QueueNode* &tail, int data) {
	QueueNode* node = new QueueNode;
	node->info = data;
	node->next = NULL;
	if (!front)
		front = tail = node;
	else {
		tail->next = node;
		tail = node;
	}
}

inline bool isQueueEmpty(QueueNode* front, QueueNode *tail) {
	return !front;
}

inline int frontQueue(QueueNode* front, QueueNode* tail) {
	if (isQueueEmpty(front, tail)) {
		cout << "Coada este vida!\n";
		return -1;
	}
	return front->info;
}

inline int tailQueue(QueueNode* front, QueueNode* tail) {
	if (isQueueEmpty(front, tail)) {
		cout << "Coada este vida!\n";
		return -1;
	}
	return tail->info;
}

void popQueue(QueueNode* &front, QueueNode* &tail) {
	if (isQueueEmpty(front, tail)) {
		cout << "Coada este vida!\n";
		return;
	}
	front = front->next;
}

bool canTrainsBeSorted(int arr[], int n, int nrLinii, TrenLinie ans[], int &dimAns) {
	QueueNode *front[NMAX], *tail[NMAX];
	for (int i = 0; i < nrLinii; ++i)
		front[i] = tail[i] = NULL;

	dimAns = 0;
	int train = 1;
	for (int it = n - 1; it >= 0; --it) {
		// se determina indicele cozii nevide cu trenul cu numarul maxim, dar mai mic decat trenul curent
		int idx = -1, mx = 0;
		for (int i = 0; i < nrLinii; ++i)
			if (!isQueueEmpty(front[i], tail[i]) && arr[it] > tailQueue(front[i], tail[i]) && tailQueue(front[i], tail[i]) > mx) {
				idx = i;
				mx = tailQueue(front[i], tail[i]);
			}
		if (idx == -1) {
			// trenul curent nu s-a putut introduce intr-o coada deja existenta, deci se cauta o coada vida
			for (int i = 0; i < nrLinii && idx == -1; ++i)
				if (isQueueEmpty(front[i], tail[i]))
					idx = i;
		}

		if (idx == -1) {
			// trenul curent nu a putut fi asezat in nicio coada, deci nu exista solutie
			return false;
		}

		pushQueue(front[idx], tail[idx], arr[it]);
		ans[dimAns].tren = arr[it];
		ans[dimAns].linie = idx;
		++dimAns;

		// se incearca asezarea trenurilor deja exsitente in cozi
		bool done = false;
		while (!done) {
			done = true;
			for (int i = 0; i < nrLinii; ++i)
				while (!isQueueEmpty(front[i], tail[i]) && frontQueue(front[i], tail[i]) == train) {
					ans[dimAns].tren = frontQueue(front[i], tail[i]);
					ans[dimAns].linie = -1;
					++dimAns;
					popQueue(front[i], tail[i]);
					++train;
					done = false;
				}
		}
	}


	bool ok = true;
	for (int i = 0; i < nrLinii && ok; ++i)
		ok = isQueueEmpty(front[i], tail[i]);

	return ok;
}

void printAnswer(int arr[], int n, int nrLinii) {
	int dimAns;
	TrenLinie ans[2 * NMAX];

	bool canBeSorted = canTrainsBeSorted(arr, n, nrLinii, ans, dimAns);
	if (!canBeSorted) {
		cout << "NU exista o strategie pentru a sorta trenurile\n";
	}
	else {
		cout << "Exista o strategie pentru a sorta trenurile\n";
		for (int i = 0; i < dimAns; ++i) {
			cout << ans[i].tren << " -> ";
			if (ans[i].linie >= 0)
				cout << ans[i].linie << "\n";
			else
				cout << "iesire\n";
		}
	}

}

int main() {
	int arr[NMAX], n, nrLinii;
	readArray(arr, n, nrLinii);
	printAnswer(arr, n, nrLinii);
	return 0;
}
