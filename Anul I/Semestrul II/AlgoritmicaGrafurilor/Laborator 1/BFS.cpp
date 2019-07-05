#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>
#include <queue>

using namespace std;

int cntNodes, cntEdges;
map<int, vector<int>> graph;
vector<bool> visited;

void read() {
    ifstream fin("graf.in");
    if (!fin) {
        cout << "Fisierul nu a putut fi deschis!\n";
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

void bfs(int startNode) {
    queue<int> queueNodes;
    for (int i = 1; i <= cntNodes; ++i)
        visited.push_back(false);

    queueNodes.push(startNode);
    visited[startNode] = true;

    while (!queueNodes.empty()) {
        int currentNode = queueNodes.front();
        queueNodes.pop();
        for (auto neighbour : graph[currentNode])
            if (!visited[neighbour]) {
                queueNodes.push(neighbour);
                visited[neighbour] = true;
            }
    }
}

int main() {
    read();
    bfs(1);

    for (int i = 1; i <= cntNodes; ++i)
        if (visited[i])
            cout << i << " ";

    return 0;
}
