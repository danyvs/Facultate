#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>
#include <queue>

using namespace std;

int cntNodes, cntEdges;
map<int, vector<int>> graph;
vector<int> controlNodes;
int startNode;
vector<bool> visited;
vector<int> parent;

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

    int cnt;
    fin >> cnt;
    for (int i = 0; i < cnt; ++i) {
        int node;
        fin >> node;
        controlNodes.push_back(node);
    }

    fin >> startNode;

    fin.close();
}

void bfs() {
    queue<int> queueNodes;
    for (int i = 1; i <= cntNodes; ++i)
        visited.push_back(false);

    queueNodes.push(startNode);
    visited[startNode] = true;

    while (!queueNodes.empty()) {
        int currentNode = queueNodes.front();
        queueNodes.pop();

        for (const auto& neighbour : graph[currentNode])
            if (!visited[neighbour]) {
                queueNodes.push(neighbour);
                visited[neighbour] = true;
                parent[neighbour] = currentNode;
            }
    }
}

void print() {
    int distMin = cntNodes;
    int nearsestControlNode = 0;
    for (const auto& node : controlNodes) {
        int dist = 0;
        int currentNode = node;
        while (currentNode != startNode && parent[currentNode]) {
            ++dist;
            currentNode = parent[currentNode];
        }
        if (!parent[currentNode])
            if (dist < distMin) {
                distMin = dist;
                nearsestControlNode = node;
            }
    }

    if (nearsestControlNode) {
        vector<int> path;
        while (nearsestControlNode) {
            path.push_back(nearsestControlNode);
            nearsestControlNode = parent[nearsestControlNode];
        }
        for (auto node = path.end() - 1; node != path.begin() - 1; --node)
            cout << *node << " ";
    }
    else {
        cout << "Nu exista!\n";
    }
}

int main() {
    read();

    for (int i = 1; i <= cntNodes; ++i)
        parent.push_back(0);

    bfs();
    print();

    return 0;
}

