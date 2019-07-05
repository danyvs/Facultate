#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <map>

using namespace std;

int cntNodes, cntEdges;
map<int, vector<int>> graph;
vector<bool> visited;
vector<int> parent;

void read(const string& inputFilePath) {
    ifstream fin(inputFilePath);
    if (!fin) {
        cout << "Input file couldn't be opened!\n";
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

void init() {
    for (int i = 1; i <= cntNodes; ++i) {
        visited.push_back(false);
        parent.push_back(0);
    }
}

void printCycle(int v, int u) {
    cout << v << " ";
    while (u != v) {
        cout << u << " ";
        u = parent[u];
    }
    cout << u << "\n";
}

void dfs(int currentNode, bool& done) {
    if (!done) {
        visited[currentNode] = true;
        for (auto node = graph[currentNode].begin(); node != graph[currentNode].end() && !done; ++node)
            if (!visited[*node]) {
                parent[*node] = currentNode;
		        dfs(*node, done);
            }
            else {
                if (parent[currentNode] != *node) {
                    printCycle(*node, currentNode);
                    done = true;
                }
            }
    }
}

int main() {
    read("graf.in");
    init();

    bool done = false;
    for (int i = 1; i <= cntNodes && !done; ++i)
        if (!visited[i])
            dfs(i, done);

    return 0;
}

