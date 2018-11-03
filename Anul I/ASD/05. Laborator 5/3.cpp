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

/* Citirea unei liste */
void read(Node* &head, Node* &tail) {
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
		for (int i = 0; i < n; ++i) {
			int temp;
			cin >> temp;
			addTail(head, tail, temp);
		}
	}
}

/* Inversarea elementelor unei liste */
void reverseList(Node* &head, Node* &tail) {
	if (!head) {
		// lista este vida
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

/* Afisarea listei inversate */
void printList(Node* head, Node* tail) {
	if (head) {
		cout << "Lista inversata este: ";
		for (Node* it = head; it; it = it->next)
			cout << it->info << " ";
	}
	else
		cout << "Lista este vida!";
	cout << "\n";
}

int main() {
	Node *head, *tail;
	read(head, tail);
	reverseList(head, tail);
	printList(head, tail);
	return 0;
}
