#include "pch.h"
#include <iostream>

using namespace std;

struct Node {
	int info;
	Node* next;
};

/* Adaugarea unui nou nod la finalul listei */
void addTail(Node* &head, Node* &tail, int data) {
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

/* Crearea unei liste */ 
void createList(Node* &head, Node* &tail) {
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
		// lista are cel putin un nod
		cout << "Dati elementele listei: ";
		for (int i = 0; i < n; ++i) {
			int data;
			cin >> data;
			addTail(head, tail, data);
		}
	}
	else
		cout << "Lista este vida!\n";
}

/* Interclasarea a doua liste sortate fara alocare de memorie suplimentara */
void mergeLists(Node* head1, Node* tail1, Node* head2, Node *tail2, Node* &head, Node* &tail) {
	if (!head1 && !head2) {
		// ambele liste sunt vide
		head = tail = NULL;
		return;
	}

	if (!head1 && head2) {
		// prima lista este vida
		head = head2;
		tail = tail2;
		return;
	}

	if (head1 && !head2) {
		// a doua lista este vida
		head = head1;
		tail = tail1;
		return;
	}

	// nicio lista nu este vida
	Node *it1 = head1, *it2 = head2;
	/* primul elemente se trateaza separat pentru a putea determina capul listei in care
	va fi memorata interclasarea celor doua liste initiale */
	if (it1->info < it2->info) {
		head = tail = it1;
		it1 = it1->next;
	}
	else {
		head = tail = it2;
		it2 = it2->next;
	}

	// se interclaseaza efectiv cele doua liste
	while (it1 && it2)
		if (it1->info < it2->info) {
			tail->next = it1;
			tail = it1;
			it1 = it1->next;
		}
		else {
			tail->next = it2;
			tail = it2;
			it2 = it2->next;
		}

	if (it1) {
		/* mai sunt elemente in a prima lista; acestea vor fi conectate
		cu restul listei care memoreaza interclasarea */
		tail->next = it1;
		tail = tail1;
	}

	if (it2) {
		/* mai sunt elemente in a doua lista; acestea vor fi conectate
		cu restul listei care memoreaza interclasarea */
		tail->next = it2;
		tail = tail2;
	}
}

/* Afisarea unei liste */
void printList(Node* head, Node* tail) {
	if (head) {
		cout << "Lista este: ";
		for (Node* it = head; it; it = it->next)
			cout << it->info << " ";
	}
	else
		cout << "Lista este vida!";
	cout << "\n";
}

int main() {
	Node *head1, *tail1; // prima lista
	Node *head2, *tail2; // a doua lista
	Node *head, *tail; // lista interclasare

	createList(head1, tail1);
	createList(head2, tail2);
	mergeLists(head1, tail1, head2, tail2, head, tail);
	printList(head, tail);

	return 0;
}
