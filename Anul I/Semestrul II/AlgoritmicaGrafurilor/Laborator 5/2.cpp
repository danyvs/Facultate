#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <map>
#include <queue>

using namespace std;

const int INF = 0x3f3f3f3f;

int cntNodes, cntEdges;
map<int, vector<pair<int, int>>> graph;
vector<int> dist;
vector<int> parent;
vector<int> controlNodes;
int startNode;

void read(const string &inputFilePath) {
    ifstream fin(inputFilePath);
    if (!fin) {
        cout << "Fisierul de intrare nu a putut fi deschis!\n";
        exit(EXIT_FAILURE);
    }

    fin >> cntNodes >> cntEdges;
    for (int i = 0; i < cntEdges; ++i) {
        int node1, node2, weight;
        fin >> node1 >> node2 >> weight;
        graph[node1].push_back(make_pair(weight, node2));
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

        for (const auto &edge : graph[currentNode])
            if (dist[edge.second] > edge.first + d) {
                dist[edge.second] = edge.first + d;
                parent[edge.second] = currentNode;
                minHeap.push(make_pair(dist[edge.second], edge.second));
            }
    }
}

void print() {
    int distMin = INF;
    int nearsestControlNode = 0;
    for (const auto &node : controlNodes)
        if (dist[node] < distMin) {
            distMin = dist[node];
            nearsestControlNode = node;
        }

    if (nearsestControlNode) {
        vector<int> path;
        while (nearsestControlNode) {
            path.push_back(nearsestControlNode);
            nearsestControlNode = parent[nearsestControlNode];
        }
        for (auto node = path.end() - 1; node != path.begin() - 1; --node)
            cout << *node << " ";
    } else {
        cout << "Nu exista!\n";
    }
}

int main() {
    read("grafpond.in");
    dijkstra();
    print();
    return 0;
}
