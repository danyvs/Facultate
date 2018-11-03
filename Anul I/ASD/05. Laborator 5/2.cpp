#include "pch.h"
#include <iostream>

using namespace std;

struct Node {
	int info;
	Node* next;
};

/* Adaugarea unui nou nod la inceputul listei */
void addHead(Node* &head, Node* &tail, int data) {
	/* nu se trateaza cazul cand lista este vida, deaorece lista initiala nu
	este vida si, pe tot parcursul executiei programului, nu se face nicio stergere
	in lista */
	Node* temp = new Node;
	temp->info = data;
	temp->next = head;
	head = temp;
}

/* Adaugarea unui nou nod la finalul listei */
void addTail(Node* &head, Node* &tail, int data) {
	/* nu se trateaza cazul cand lista este vida, deaorece lista initiala nu
	este vida si, pe tot parcursul executiei programului, nu se face nicio stergere
	in lista */
	Node* temp = new Node;
	temp->info = data;
	temp->next = NULL;
	tail->next = temp;
	tail = temp;
}

/* Returneaza un pointer catre valoarea dupa care trebuie inserata valoarea curenta*/
Node* positionToInsert(Node* head, Node* tail, int data) {
	Node* it = head;
	while (it->next && data < it->next->info)
		it = it->next;
	return it;
}

/* Afisarea listei */
void printList(Node* head, Node* tail) {
	cout << "Lista este: ";
	for (Node* it = head; it; it = it->next)
		cout << it->info << " ";
	cout << "\n";
}

/* Crearea unei liste sortate */
void createSortedList(Node* &head, Node* &tail) {
	int n;
	cout << "Dati numarul de elemente al listei: ";
	cin >> n;

	// initializarea listei
	head = tail = NULL; 

	if (n < 0) {
		cout << "Numarul de elemente al listei trebuie sa fie >= 0\n";
		exit(EXIT_SUCCESS);
	}

	if (n > 0) {
		cout << "Dati elementele listei: ";

		/* primul nod se trateaza separat, deoarece nu poate fi comparat cu
		niciun alt nod */
		int data;
		cin >> data;
		head = tail = new Node;
		head->info = data;
		head->next = NULL;
		printList(head, tail);

		for (int i = 1; i < n; ++i) {
			cin >> data;
			if (data <= head->info) {
				// valoarea curenta este mai mica (sau egala) decat cea din primul nod al listei
				addHead(head, tail, data);
			}
			else {
				if (data > tail->info) {
					// informatia curenta este mai mare decat cea din primul nod al listei
					addTail(head, tail, data);
				}
				else {
					// informatia curenta trebuie inserata intre doua noduri deja existente
					Node* pos = positionToInsert(head, tail, data);
					Node* temp = new Node;
					temp->info = data;
					temp->next = pos->next;
					pos->next = temp;
				}
			}
			printList(head, tail);
		}
	}
	else
		cout << "Lista este vida!\n";
}

int main() {
	Node *head, *tail;
	createSortedList(head, tail);
	return 0;
}
