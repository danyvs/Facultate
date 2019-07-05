//
// Created by Dany on 15.05.2019.
//

#ifndef MULTISET_MULTISET_H
#define MULTISET_MULTISET_H

#include <iostream>
#include <cassert>

using namespace std;

template<class T>
class Multiset {
    struct Node {
        explicit Node(const T &value) : value(value), next(nullptr) {}

        T value;
        Node *next;
    };

    Node *head_;

    void copy(const Multiset &other);

    void clear();

public:
    Multiset();

    Multiset(const Multiset &other);

    ~Multiset();

    Multiset &operator=(const Multiset &other);

    void append(const T &elem);

    void removeFirst(const T &value);

    size_t count(const T &value) const;

    bool contains(const T &value) const;

    void removeAll(const T &value);

    size_t size() const;

    template<class Operation>
    void transform(Operation op);

    template<class U>
    friend ostream &operator<<(ostream &out, const Multiset<U> &multiset);


};

/**
 *  Constructor for Multiset
 */
template<class T>
Multiset<T>::Multiset() : head_(nullptr) {

}

/**
 *  Copy constructor
 * @tparam T - parameter type
 * @param other - Multiset
 */
template<class T>
Multiset<T>::Multiset(const Multiset &other) : head_(nullptr) {
    copy(other);
}

/**
 *  Destructor for class Multiset
 * @tparam T - parameter type
 */
template<class T>
Multiset<T>::~Multiset() {
    clear();
}

/**
 *  Overload operator = for Multiset
 * @tparam T - parameter type
 * @param other - Multiset
 * @return *this
 */
template<class T>
Multiset<T> &Multiset<T>::operator=(const Multiset<T> &other) {
    if (&other != this) {
        clear();
        copy(other);
    }
    return *this;
}

/**
 *  Insert a new element into the multiset
 * @tparam T - parameter type
 * @param elem - element to be inserted
 */
template<class T>
void Multiset<T>::append(const T &elem) {
    Node *tempNode = new Node(elem);

    if (!head_) {
        // 0 nodes
        head_ = tempNode;
    } else {
        if (!head_->next) {
            // 1 node
            if (head_->value < tempNode->value) {
                head_->next = tempNode;
            } else {
                tempNode->next = head_;
                head_ = tempNode;
            }
        } else {
            // 2 or more nodes
            if (tempNode->value < head_->value) {
                tempNode->next = head_;
                head_ = tempNode;
            } else {
                Node *previousNode = head_;
                Node *currentNode = head_->next;
                while (currentNode && currentNode->value < tempNode->value) {
                    previousNode = currentNode;
                    currentNode = currentNode->next;
                }
                tempNode->next = previousNode->next;
                previousNode->next = tempNode;
            }
        }
    }

}

/**
 *  Remove the first apparition of the given value
 * @tparam T - parameter type
 * @param value - the value to be removed
 */
template<class T>
void Multiset<T>::removeFirst(const T &value) {
    Node *previousNode = head_;
    Node *currentNode = head_;
    while (currentNode && currentNode->value != value) {
        previousNode = currentNode;
        currentNode = currentNode->next;
    }

    if (!currentNode) {
        // the value was not found
        return;
    }

    if (currentNode == head_) {
        // remove the head
        head_ = head_->next;
    } else {
        if (!currentNode->next) {
            // remove the tail
            previousNode->next = nullptr;
        } else {
            previousNode->next = currentNode->next;
        }
    }
    delete currentNode;
}

/**
 *  Count how many times an element appears
 * @tparam T - parameter type
 * @param value - the value to be looked up
 * @return the number of apparitions of the given element
 */
template<class T>
size_t Multiset<T>::count(const T &value) const {
    size_t cnt = 0;
    for (Node *node = head_; node; node = node->next)
        if (node->value == value)
            ++cnt;
    return cnt;
}

/**
 *  Check if an element in in the multiset
 * @tparam T - parameter type
 * @param value - the value to be looked up
 * @return true or false, depending whether the element is in the multiset or not
 */
template<class T>
bool Multiset<T>::contains(const T &value) const {
    for (Node *node = head_; node; node = node->next)
        if (node->value == value)
            return true;
    return false;
}

/**
 *  Remove all the apparitions of a given value
 * @tparam T - parameter type
 * @param value - the value to be removed
 */
template<class T>
void Multiset<T>::removeAll(const T &value) {
    while (contains(value))
        removeFirst(value);
}

/**
 *  Count the number of distinct elements
 * @tparam T - parameter type
 * @return the number of distinct elements
 */
template<class T>
size_t Multiset<T>::size() const {
    size_t cnt = 0;

    Node *node = head_;
    while (node) {
        ++cnt;
        for (int i = 0, limit = count(node->value); i < limit; ++i)
            node = node->next;
    }

    return cnt;
}

/**
 *  Overload operator << to print a multiset
 * @tparam T - parameter type
 * @param out - output stream
 * @param M - Multiset
 * @return output stream
 */
template<class T>
ostream &operator<<(ostream &out, const Multiset<T> &multiset) {
    for (auto node = multiset.head_; node; node = node->next)
        out << node->value << " ";
    return out;
}

/**
 *  Copy the content of a Multiset into another
 * @tparam T - parameter type
 * @param other - Multiset
 */
template<class T>
void Multiset<T>::copy(const Multiset &other) {
    assert(head_ == nullptr);

    if (!other.head_)
        return;

    head_ = new Node(other.head_->value);

    Node *tail = head_;
    Node *currentNode = other.head_->next;
    while (currentNode) {
        Node *node = new Node(currentNode->value);
        tail->next = node;
        tail = node;

        currentNode = currentNode->next;
    }
}

/**
 *  Clear(erase) the content of the Multiset
 * @tparam T - parameter type
 */
template<class T>
void Multiset<T>::clear() {
    if (!head_)
        return;

    Node *previousNode = head_;
    Node *currentNode = head_->next;
    while (currentNode) {
        delete previousNode;
        previousNode = currentNode;
        currentNode = currentNode->next;
    }
    delete previousNode;

    head_ = nullptr;
}

template<class T>
template<class Operation>
void Multiset<T>::transform(Operation op) {
    for (Node *node = head_; node; node = node->next)
        node->value = op(node->value);
}

class Multiply {
    int num_;
public:
    explicit Multiply(int num) : num_(num) {};
    int operator()(int x) {
        return num_ * x;
    }
};

#endif //MULTISET_MULTISET_H
