#include "pch.h"
#include <iostream>

using namespace std;

struct Node {
	double info;
	Node* next;
};

/* Adaugarea unui nou nod la finalul listei */
void addTail(Node* &head, Node* &tail, double data) {
	if (head == NULL) {
		// lista este vida
		head = tail = new Node;
		head->info = data;
		head->next = NULL;
	}
	else {
		Node* temp = new Node;
		temp->info = data;
		temp->next = NULL;
		tail->next = temp;
		tail = temp;
	}
}

/* Citirea datelor de intrare */
void read(Node* &head, Node* &tail) {
	int n;
	cout << "Dati numarul de elemente al listei initiale: ";
	cin >> n;

	head = tail = NULL; // initializare lista

	if (n <= 0) {
		cout << "Numarul de elemente al listei trebuie sa fie >= 0\n";
		exit(EXIT_SUCCESS);
	}

	cout << "Dati elementele listei: ";
	for (int i = 0; i < n; ++i) {
		double temp;
		cin >> temp;
		addTail(head, tail, temp);
	}
}

/* Adaugarea mediei aritmetice intre oricare doua noduri consecutive */
void addBetweenNodes(Node* head, Node* tail) {
	for (Node* p = head; p->next; p = p->next->next) {
		Node* temp = new Node;
		temp->info = (p->info + p->next->info) / 2;
		/* se face mai intai legatura cu nodul urmator, pentru a nu pierde
		legaturile dintre noduri */
		temp->next = p->next; // legatura cu nodul urmator
		p->next = temp; // legatura cu nodul anterior
	}
}

/* Afisarea listei */
void printList(Node* head, Node* tail) {
	cout << "Lista este: ";
	for (Node* p = head; p; p = p->next)
		cout << p->info << " ";
	cout << "\n";
}

int main() {
	Node *head, *tail;
	read(head, tail);
	addBetweenNodes(head, tail);
	printList(head, tail);
	return 0;
}
