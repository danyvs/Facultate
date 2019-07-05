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

/* Citirea datelor de intrare */
void read(Node* &head, Node* &tail, int &number) {
	/* numarul mare se citeste sub forma de sir de caracter din motive de
	eficienta */
	char str[NMAX];
	cout << "Introduceti numarul mare: ";
	cin >> str;

	// initializare lista
	head = tail = NULL;

	int lg = strlen(str);
	for (int i = lg - 1; i >= 0; --i) {
		int digit = str[i] - '0';
		addTail(head, tail, digit);
	}

	cout << "Introduceti cifra: ";
	cin >> number;
}

/* Inmultirea unui numar mare cu un numar mic */
void multiplyBigIntbyInt(Node* head1, Node* tail1, int number, Node* &headProd, Node* &tailProd) {
	// initializare lista vida
	headProd = tailProd = NULL;

	// inmultirea efectiva
	for (Node* it = head1; it; it = it->next)
		addTail(headProd, tailProd, it->info * number);

	// transport
	int carry = 0;
	for (Node* it = headProd; it; it = it->next) {
		it->info += carry;
		carry = it->info / 10;
		it->info %= 10;
	}
	while (carry) {
		addTail(headProd, tailProd, carry % 10);
		carry /= 10;
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

/* Stergerea zerourilor nesemnificative de la inceput */
void deleteZerosHead(Node* &head, Node* tail) {
	while (head->next && head->info == 0) {
		Node* temp = head;
		head = head->next;
		delete temp;
	}
}

/* Afisarea unei liste */
void printList(Node* &head, Node* &tail) {
	cout << "Produsul celor doua numere este: ";
	for (Node* it = head; it; it = it->next)
		cout << it->info;
	cout << "\n";
}

int main() {
	Node *head1, *tail1; // lista in care va fi memorat numarul mare
	int number;
	Node *headProd, *tailProd; // lista in care va fi memorat produsul celor doua numere

	read(head1, tail1, number);
	multiplyBigIntbyInt(head1, tail1, number, headProd, tailProd);
	reverseList(headProd, tailProd);
	deleteZerosHead(headProd, tailProd);
	printList(headProd, tailProd);

	return 0;
}
