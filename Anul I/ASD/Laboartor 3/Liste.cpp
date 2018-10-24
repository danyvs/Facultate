#include <iostream>

using namespace std;

struct node {
    int info;
    node* next;
};

void addHead(node* &head, node* &tail, int data) {
    // adaugarea unui nou element la inceputul listei
    if (head == NULL) {
        // in acest moment, lista este vida
        head = tail = new node;
        head->info = data;
        head->next = NULL;
    }
    else {
        node* temp = new node;
        temp->info = data;
        temp->next = head;
        head = temp;
    }
}

void addTail(node* &head, node* &tail, int data) {
    // adaugarea unui nou element la finalul listei
    if (head == NULL) {
        // in acest moment, lista este vida
        head = tail = new node;
        head->info = data;
        head->next = NULL;
    }
    else {
        node* temp = new node;
        temp->info = data;
        temp->next = NULL;
        tail->next = temp;
        tail = temp;
    }
}

void printList(node* head, node* tail) {
    // afisarea listei
    if (head == NULL)
        cout << "Lista vida!";
    else
        for (node* p = head; p; p = p->next)
            cout << p->info << " ";
    cout << "\n";
}

int searchValue(node* head, node* tail, int data) {
    // cautarea unui element dupa valoare
    int idx = 1;
    for (node* p = head; p; p = p->next, ++ idx)
        if (p->info == data)
            return idx;
    return -1;
}

int searchPosition(node* head, node* tail, int pos) {
    // cautarea unui element dupa pozitie
    node* p = head;
    int idx = 1;

    while (p && idx < pos) {
        p = p->next;
        ++ idx;
    }

    return p ? p->info : -1;
}

void insertAfterValue(node* head, node* &tail, int value) {
    node* p = head;
    while (p && p->info != value)
        p = p->next;
    if (p) {
        // valoarea a fost gasita
        node* q = new node;
        q->info = value;
        q->next = p->next;
        p->next = q;
        if (p == tail)
            tail = q;
    }
}

void insertPosition(node* &head, node* &tail, int pos) {
    if (pos == 1) {
        node* p = new node;
        p->info = pos;
        p->next = head->next;
        head->next = p;
        head = p;
    }
    else {
        int idx = 1;
        node* p = head;
        while (p && idx < pos) {
            p = p->next;
            ++ idx;
        }
        if (p) {
            node* q = new node;
            q->info = pos;
            q->next = p->next;
            p->next = q;
            if (p == tail)
                tail = q;
        }
    }
}

void deleteValue(node* &head, node* &tail, int value) {
    node* p = head, *q = NULL;
    while (p && p->info != value) {
        q = p;
        p = p->next;
    }

    if (p == NULL)
        return;

    if (head == tail && head->info == value) {
        head = tail = NULL;
        return;
    }

    if (head-> info == value) {
        node* p = head;
        head = head->next;
        delete p;
        return;
    }

    if (p == tail) {
        q->next = NULL;
        tail = q;
        delete p;
    }

    q->next = p->next;
    delete p;
}

void deletePosition(node* &head, node* &tail, int pos) {
    int idx = 1;
    node* p = head, *q = NULL;
    while (p && idx < pos) {
        q = p;
        p = p->next;
        ++ idx;
    }

    if (!p)
        return;

    if (head == tail && pos == 1) {
        head = tail = NULL;
        return;
    }

    if (p == head) {
        head = head->next;
        delete p;
        return;
    }

    if (p == tail) {
        q->next = NULL;
        tail = q;
        delete p;
    }

    q->next = p->next;
    delete p;
}

int main() {
    node *head = NULL, *tail = NULL;
    printList(head, tail);
    addHead(head, tail, 1);
    addHead(head, tail, 2);
    addHead(head, tail, 3);
    printList(head, tail);
    addTail(head, tail, 4);
    printList(head, tail);
    cout << searchValue(head, tail, 5) << "\n";
    cout << searchValue(head, tail, 1) << "\n";
    cout << searchPosition(head, tail, 5) << "\n";
    cout << searchPosition(head, tail, 3) << "\n";
    deletePosition(head, tail, 1);
    printList(head, tail);
    deletePosition(head, tail, 1);
    printList(head, tail);
    deleteValue(head, tail, 4);
    printList(head, tail);
    addTail(head, tail, 5);
    printList(head, tail);
    deleteValue(head, tail, 1);
    deletePosition(head, tail, 1);
    printList(head, tail);
    addTail(head, tail, 8);
    printList(head, tail);
    return 0;
}
