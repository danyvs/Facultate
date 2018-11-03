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

/* Citirea unui numar mare si memorarea sa intr-o lista */
void readBigNumber(Node* &head, Node* &tail) {
	// numarul se citeste sub forma de sir din motive de eficienta
	char str[NMAX];
	cout << "Dati numarul: ";
	cin >> str;

	int lg = strlen(str);
	head = tail = NULL;
	for (int i = lg - 1; i >= 0; --i) {
		int digit = str[i] - '0';
		addTail(head, tail, digit);
	}
}

/* Adunarea a doua numere mari memorate ca liste */
void addBigNumbers(Node* head1, Node* tail1, Node* head2, Node* tail2, Node* &headSum, Node* &tailSum) {
	// initializare lista suma
	headSum = tailSum = NULL;

	// adunarea cifra cu cifra, incepand de la cifra cea mai nesemnificativa
	Node *it1 = head1, *it2 = head2;
	int sum, transport = 0;
	while (it1 && it2) {
		// calcularea valorii din nodul curent
		sum = it1->info + it2->info + transport;
		transport = sum / 10;
		sum %= 10;
		
		// adaugarea efectiva a nodului
		addTail(headSum, tailSum, sum);

		it1 = it1->next;
		it2 = it2->next;
	}

	while (it1) {
		// primul numar este mai mare decat al doilea
		sum = it1->info + transport;
		transport = sum / 10;
		sum %= 10;

		addTail(headSum, tailSum, sum);
		it1 = it1->next;
	}

	while (it2) {
		// al doilea numar este mai mare decat primul
		sum = it2->info + transport;
		transport = sum / 10;
		sum %= 10;

		addTail(headSum, tailSum, sum);
		it2 = it2->next;
	}

	// adaugarea posibilului transport
	while (transport) {
		addTail(headSum, tailSum, transport % 10);
		transport /= 10;
	}
}

/* Inversarea elementelor unei liste */
void reverseList(Node* &head, Node* &tail) {
	if (!head) {
		// lista este vida
		// asa ceva nu ar trebui sa se intample niciodata
		return;
	}

	if (!head->next) {
		// lista are un singur element, deci nu trebuie facut nimic
		return;
	}

	if (!head->next->next) {
		// lista are exact doua elemente
		tail->next = head;
		head->next = NULL;
		head = tail;
		return;
	}

	// lista are cel putin 3 noduri
	Node* prevNode = head;
	Node* currNode = head->next;
	Node* nextNode = head->next->next;

	tail = head; // coada devine cap

	while (nextNode) {
		currNode->next = prevNode;
		prevNode = currNode;
		currNode = nextNode;
		nextNode = nextNode->next;
	}

	currNode->next = prevNode;
	head = currNode;
	tail->next = NULL;
}

/* Afisarea listei */
void printList(Node* head, Node* tail) {
	cout << "Suma celor doua numere este: ";
	for (Node* it = head; it; it = it->next)
		cout << it->info;
	cout << "\n";
}

int main() {
	Node *head1, *tail1; // prima lista; memoreaza primul numar
	Node *head2, *tail2; // a doua lista; memoreaza al doilea numar
	Node *headSum, *tailSum; // lista care memoreaza suma

	readBigNumber(head1, tail1);
	readBigNumber(head2, tail2);
	addBigNumbers(head1, tail1, head2, tail2, headSum, tailSum);
	reverseList(headSum, tailSum);
	printList(headSum, tailSum);

	return 0;
}
