#include "pch.h"
#include <iostream>
#include <cstring>

#define NMAX 1005

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

/* Citirea unui numar mare */
void readNumber(Node* &head, Node* &tail) {
	/* numarul mare se citeste sub forma de sir de caractere din motive de
	eficienta */
	cout << "Introduceti numarul: ";
	char str[NMAX];
	cin >> str;
	int lg = strlen(str);

	head = tail = NULL;
	for (int i = lg - 1; i >= 0; --i)
		addTail(head, tail, (str[i] - '0'));
}

/* Returneaza numarul de noduri al unei liste */
int sizeOfList(Node* head, Node* tail) {
	int cnt = 0;
	for (Node* it = head; it; it = it->next)
		++cnt;
	return cnt;
}

/* Inmultirea a doua numere mari (care nu pot fi memorate pe tipurile de date
predefinite) memorate sub forma de liste liniare simplu inlantuite  */
void multiplyLists(Node* head1, Node* tail1, Node* head2, Node* tail2, Node* &head3, Node* &tail3) {
	head3 = NULL;

	// initializare lista produs cu 0
	int dim = sizeOfList(head1, tail1) + sizeOfList(head2, tail2);
	for (int i = 0; i < dim; ++i)
		addTail(head3, tail3, 0);

	// inmultirea efectiva
	Node* nodeStart = head3; // pozitia de unde pleaca la fiecare iteratie
	for (Node* it1 = head1; it1; it1 = it1->next, nodeStart = nodeStart->next) {
		Node* it = nodeStart;
		for (Node* it2 = head2; it2; it2 = it2->next, it = it->next)
			it->info += it1->info * it2->info;
	}

	int carry = 0;
	for (Node* it = head3; it; it = it->next) {
		it->info += carry;
		carry = it->info / 10;
		it->info %= 10;
	}
}

/* Inversarea unei liste */
void reverseList(Node* &head, Node* &tail) {
	if (!head) {
		// lista este vida
		// NU ar trebui sa se intample niciodata
		return;
	}

	if (!head->next) {
		// lista are un singur element; nu trebuie facut nimic
		return;
	}

	if (!head->next->next) {
		// lista are doua elemente
		tail->next = head;
		head->next = NULL;
		Node* temp = head;
		head = tail;
		tail = temp;
		return;
	}

	// lista are cel putin 3 elemente
	// lista se va parcurge cu 3 pointeri
	Node* prevNode = head;
	Node* currNode = head->next;
	Node* nextNode = head->next->next;

	while (nextNode) {
		currNode->next = prevNode;
		prevNode = currNode;
		currNode = nextNode;
		nextNode = nextNode->next;
	}

	// se actualizeaza coada
	tail = head;
	tail->next = NULL;

	// se actualizeaza capul
	head = currNode;
	head->next = prevNode;
}

/* Stergerea zerourilor nesemnificative de la inceputul numarului */
void deleteZerosHead(Node* &head, Node* &tail) {
	while (head->next && head->info == 0) {
		Node* temp = head;
		head = head->next;
		delete temp;
	}
}

/* Afisarea listei */
void printList(Node* head, Node* tail) {
	cout << "Produsul celor doua numere mari este: ";
	for (Node* it = head; it; it = it->next)
		cout << it->info;
	cout << "\n";
}

int main() {
	Node *head1, *tail1; // lista care memoreaza primul numar
	Node *head2, *tail2; // lista care memoreaza al doilea numar
	Node *headProd, *tailProd; // lista care memoreaza produsul

	readNumber(head1, tail1);
	readNumber(head2, tail2);
	multiplyLists(head1, tail1, head2, tail2, headProd, tailProd);
	reverseList(headProd, tailProd);
	deleteZerosHead(headProd, tailProd);
	printList(headProd, tailProd);

	return 0;
}
