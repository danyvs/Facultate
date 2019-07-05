#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <queue>

using namespace std;

const int INF = 0x3f3f3f3f;

int cntNodes, cntEdges;
map<int, vector<pair<int, int>>> graph;
vector<int> dist;
vector<int> parent;
int startNode, endNode;

void read(const string& inputFilePath) {
    ifstream fin(inputFilePath);

    fin >> cntNodes >> cntEdges;
    for (int i = 0; i < cntEdges; ++i) {
        int node1, node2, weight;
        fin >> node1 >> node2 >> weight;
        graph[node1].push_back(make_pair(-weight, node2));
    }
    fin >> startNode >> endNode;

    fin.close();
}

void dijkstra() {
    for (int i = 0; i <= cntNodes; ++i) {
        dist.push_back(INF);
        parent.push_back(0);
    }

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> minHeap;
    minHeap.push(make_pair(0, startNode));
    dist[startNode] = 0;
    while (!minHeap.empty()) {
        int currentNode = minHeap.top().second;
        int d = minHeap.top().first;
        minHeap.pop();

        for (const auto& edge : graph[currentNode])
            if (dist[edge.second] > edge.first + d) {
                dist[edge.second] = edge.first + d;
                parent[edge.second] = currentNode;
                minHeap.push(make_pair(dist[edge.second], edge.second));
            }
    }
}

void print(int node) {
    if (parent[node]) {
        print(parent[node]);
    }
    cout << node << " ";
}

int main() {
    read("retea.in");
    dijkstra();

    cout << -dist[endNode] << "\n";
    print(endNode);
    return 0;
}
