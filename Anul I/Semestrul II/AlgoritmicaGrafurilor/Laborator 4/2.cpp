#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>
#include <queue>

using namespace std;

const int INF = 0x3f3f3f3f;

struct Edge {
    int node1, node2, weight;
};

int cntNodes, cntEdges;
map<int, vector<pair<int, int>>> graph;

void read(const string& inputFilePath) {
    ifstream fin(inputFilePath);
    if (!fin) {
        cout << "Input file couldn't be opened!\n";
        exit(EXIT_FAILURE);
    }

    fin >> cntNodes >> cntEdges;
    for (int i = 0; i < cntEdges; ++i) {
        int node1, node2, weight;
        fin >> node1 >> node2 >> weight;
        graph[node1].emplace_back(make_pair(weight, node2));
        graph[node2].emplace_back(make_pair(weight, node1));
    }

    fin.close();
}

vector<Edge> prim(int startNode) {
    vector<int> dist(cntNodes + 1, INF);
    vector<int> parent(cntNodes + 1, 0);
    vector<bool> visited(cntNodes + 1, false);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> minHeap;

    minHeap.push(make_pair(0, startNode));
    dist[startNode] = 0;

    vector<Edge> minimumSpanningTree;
    while (!minHeap.empty()) {
        int currentNode = minHeap.top().second;
        int currentDistance = minHeap.top().first;
        minHeap.pop();

        if (!visited[currentNode]) {
            visited[currentNode] = true;

            if (parent[currentNode]) {
                Edge edge;
                edge.node1 = currentNode;
                edge.node2 = parent[currentNode];
                edge.weight = currentDistance;
                minimumSpanningTree.push_back(edge);
            }

            for (const auto& edge : graph[currentNode])
                if (!visited[edge.second] && dist[edge.second] > edge.first) {
                    dist[edge.second] = edge.first;
                    parent[edge.second] = currentNode;
                    minHeap.push(make_pair(dist[edge.second], edge.second));
                }
        }
    }

    return minimumSpanningTree;
}

void print() {
    vector<Edge> MST = prim(1);
    for (const auto& edge : MST)
        cout << edge.node1 << " " << edge.node2 << " " << edge.weight << "\n";
}

int main() {
    read("grafpond.in");
    print();
    return 0;
}
