#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>
#include <queue>

using namespace std;

int cntNodes, cntEdges;
map<int, vector<int>> graph;

void read() {
    ifstream fin("graf.in");
    if (!fin) {
        cout << "Input file could not be opened!\n";
        exit(EXIT_FAILURE);
    }

    fin >> cntNodes >> cntEdges;
    for (int i = 0; i < cntEdges; ++i) {
        int node1, node2;
        fin >> node1 >> node2;
        graph[node1].push_back(node2);
    }

    fin.close();
}

vector<int> calculeInteriorDegrees() {
    vector<int> intDegree(cntNodes + 1, 0);
    for (int i = 1; i <= cntNodes; ++i)
        for (const auto& node : graph[i])
            ++intDegree[node];
    return intDegree;
}

vector<bool> topologicalSort() {
    vector<int> intDegree = calculeInteriorDegrees();
    vector<bool> visited(cntNodes + 1, false);
    queue<int> queueNodes;

    for (int i = 1; i <= cntNodes; ++i)
        if (!intDegree[i])
            queueNodes.push(i);

    while (!queueNodes.empty()) {
        int currentNode = queueNodes.front();
        visited[currentNode] = true;
        queueNodes.pop();
        for (const auto& node : graph[currentNode]) {
            --intDegree[node];
            if (!intDegree[node])
                queueNodes.push(node);
        }
    }

    return visited;
}

void printCycle(int v, int u, vector<int> parent) {
    if (u != v) {
        u = parent[u];
        printCycle(v, u, parent);
        cout << u << " ";
    }

}

void dfs(int currentNode, bool& done, vector<int> parent, vector<bool> visited) {
    visited[currentNode] = true;
    if (!done) {
        for (auto node = graph[currentNode].begin(); node != graph[currentNode].end() && !done; ++node)
            if (!visited[*node]) {
                parent[*node] = currentNode;
                dfs(*node, done, parent, visited);
            }
            else {
                if (parent[currentNode] != *node) {
                    printCycle(*node, currentNode, parent);
                    cout << currentNode << "\n";
                    done = true;
                }
            }
    }

}

void print() {
    vector<bool> visited = topologicalSort();

    bool done = true;
    for (int i = 1; i <= cntNodes && done; ++i)
        done = visited[i];

    ofstream fout("graf.out");
    if (done)
        fout << "REALIZABIL!\n";
    else {
        vector<int> parent;
        vector<bool> seen;
        bool done = false;
        for (int i = 0; i <= cntNodes; ++i) {
            parent.push_back(0);
            seen.push_back(false);
        }
        for (int i = 1; i <= cntNodes && !done; ++i)
            if (!visited[i])
                dfs(i, done, parent, seen);

    }

    fout.close();
}

int main() {
    read();
    print();
    return 0;
}

