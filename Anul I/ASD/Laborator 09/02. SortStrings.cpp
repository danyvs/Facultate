#include <iostream>
#include <cstring>

using namespace std;

#define LMAX 20

struct Node {
    char info[LMAX];
    Node *left, *right;
};

Node* newNode(char data[LMAX]) {
    Node* temp = new Node;
    strcpy(temp->info, data);
    temp->left = temp->right = NULL;
    return temp;
}

void insertNode(Node* &root, char data[LMAX]) {
    if (!root)
        root = newNode(data);
    else {
        if (strcmp(root->info, data) > 0)
            insertNode(root->left, data);
        else
            insertNode(root->right, data);
    }
}

void inorder(Node* root) {
    if (root) {
        inorder(root->left);
        cout << root->info << "\n";
        inorder(root->right);
    }
}

int main() {
    int n;
    Node* root = NULL;

    cin >> n;
    for (int i = 0; i < n; ++i) {
        char str[LMAX];
        cin >> str;
        insertNode(root, str);
    }
    inorder(root);
    return 0;
}
