#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>

using namespace std;

struct Edge {
    int node1, node2, weight;
    bool operator < (const Edge& edge) {
        return weight < edge.weight;
    }
};

int cntNodes, cntEdges;
vector<Edge> edges;
vector<int> height;
vector<int> parent;
map<int, vector<pair<int, int>>> graph;
map<pair<int, int>, int> edgesCost;
int cost;
vector<bool> visited;
pair<int, int> maxWeightEdgeCycle;

void read(const string& inputFilePath) {
    ifstream fin(inputFilePath);
    if (!fin) {
        cout << "Input file couldn't be opened!\n";
        exit(EXIT_FAILURE);
    }

    fin >> cntNodes >> cntEdges;
    for (int i = 0; i < cntEdges; ++i) {
        Edge edge;
        fin >> edge.node1 >> edge.node2 >> edge.weight;
        edges.push_back(edge);
    }

    fin.close();
}

int root(int node) {
    int r = node;
    while (parent[r])
        r = parent[r];
    while (parent[node]) {
        int temp = parent[node];
        parent[node] = r;
        node = temp;
    }
    return r;
}

void unite(int root1, int root2) {
    if (height[root1] < height[root2])
        parent[root1] = root2;
    else {
        parent[root2] = root1;
        if (height[root1] == height[root2])
            ++height[root1];
    }
}

void kruskal() {
    for (int i = 0; i <= cntNodes; ++i) {
        height.push_back(1);
        parent.push_back(0);
    }

    sort(edges.begin(), edges.end());

    int cntSelectedEdges = 0;
    for (const auto& edge : edges) {
        int root1 = root(edge.node1);
        int root2 = root(edge.node2);
        if (root1 != root2) {
            cost += edge.weight;
            graph[edge.node1].emplace_back(make_pair(edge.weight, edge.node2));
            graph[edge.node2].emplace_back(make_pair(edge.weight, edge.node1));
            edgesCost[make_pair(edge.node1, edge.node2)] = edge.weight;
            edgesCost[make_pair(edge.node2, edge.node1)] = edge.weight;

            unite(root1, root2);
            ++cntSelectedEdges;
            if (cntSelectedEdges == cntNodes - 1)
                break;
        }
    }
}

void printMST() {
    cout << "Minimum Spanning Tree edges:\n";
    for (int node1 = 1; node1 <= cntNodes; ++node1)
        for (const auto& node2 : graph[node1])
            if (node1 < node2.second)
                cout << node1 << " " << node2.second << "\n";
    cout << "Cost: " << cost << "\n";
}

void getMaxWeightCycle(int v, int u) {
    if (edgesCost[make_pair(v, u)] > edgesCost[maxWeightEdgeCycle]) {
        maxWeightEdgeCycle = make_pair(v, u);
    }
    while (v != u) {
        if (edgesCost[make_pair(u, parent[u])] > edgesCost[maxWeightEdgeCycle]) {
            maxWeightEdgeCycle = make_pair(u, parent[u]);
        }
        u = parent[u];
    }
}

void dfs(int currentNode, bool& done) {
    if (!done) {
        visited[currentNode] = true;
        for (auto edge = graph[currentNode].begin(); edge != graph[currentNode].end() && !done; ++edge)
            if (!visited[edge->second]) {
                parent[edge->second] = currentNode;
                dfs(edge->second, done);
            }
            else {
                if (parent[currentNode] != edge->second) {
                    getMaxWeightCycle(edge->second, currentNode);
                    done = true;
                }
            }
    }
}

void addNewEdge() {
    int node1, node2, weight;
    cin >> node1 >> node2 >> weight;
    graph[node1].emplace_back(make_pair(weight, node2));
    graph[node2].emplace_back(make_pair(weight, node1));

    for (int i = 0; i < cntNodes; ++i)
        visited.push_back(false);
    parent.clear();

    bool done = false;
    dfs(1, done);

    cout << "The edge with the max cost in cycle is edge " << maxWeightEdgeCycle.first << " " <<
        maxWeightEdgeCycle.second << " with the cost " << edgesCost[maxWeightEdgeCycle] << "\n";
    cout << "The new cost is " << cost - edgesCost[maxWeightEdgeCycle] + weight << "\n";
}

int main() {
    read("grafpond.in");
    kruskal();
    printMST();
    addNewEdge();
    return 0;
}

