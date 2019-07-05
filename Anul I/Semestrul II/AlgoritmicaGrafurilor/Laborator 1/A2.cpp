#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>

using namespace std;

int cntNodes, cntEdges;
map<int, vector<int>> graph;

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
        graph[node1].push_back(node2);
        graph[node2].push_back(node1);
    }

    fin.close();
}

void print() {
    for (int node1 = 1; node1 <= cntNodes; ++node1) {
        cout << node1 << " ";
        for (const auto &node2 : graph[node1])
            cout << node2 << " ";
        cout << "\n";
    }
}

int main() {
    read("graf.in");
    print();
    return 0;
}
