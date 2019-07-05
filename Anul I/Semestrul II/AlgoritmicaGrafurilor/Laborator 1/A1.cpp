#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int NMAX = 1005;

int graph[NMAX][NMAX], cntNodes, cntEdges;

void read(const string& inputFilePath) {
    ifstream fin(inputFilePath);
    if (!fin) {
        cout << "Fisierul de intrare nu a putut fi deschis!\n";
        exit(EXIT_FAILURE);
    }

    fin >> cntNodes >> cntEdges;
    for (int i = 0; i < cntEdges; ++i) {
        int node1, node2;
        fin >> node1 >> node2;
        graph[node1][node2] = graph[node2][node1] = 1;
    }

    fin.close();
}

void print() {
    for (int i = 1; i <= cntNodes; ++i) {
        for (int j = 1; j <= cntNodes; ++j)
            cout << graph[i][j] << " ";
        cout << "\n";
    }
}

int main() {
    read("graf.in");
    print();
    return 0;
}
