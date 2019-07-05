#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

using namespace std;

struct Edge {
    int node1, node2, weight;
    bool operator < (const Edge& edge) {
        return weight < edge.weight;
    }
};

int cntNodes;
vector<Edge> edges;
vector<int> height;
vector<int> parent;

void read(const string& inputFilePath) {
    ifstream fin(inputFilePath);
    if (!fin) {
        cout << "Input file could not be opened!\n";
        exit(EXIT_FAILURE);
    }

    int cntEdges;
    fin >> cntNodes >> cntEdges;
    for (int i = 0; i < cntEdges; ++i) {
        Edge edge;
        fin >> edge.node1 >> edge.node2 >> edge.weight;
        edges.push_back(edge);
    }

    fin.close();
}

int root(int node) {
    while (parent[node])
        node = parent[node];
    return node;
}

void unite(int root1, int root2) {
    if (height[root1] > height[root2])
        parent[root2] = root1;
    else {
        parent[root1] = root2;
        if (height[root1] == height[root2])
            ++height[root2];
    }
}

vector<Edge> kruskal() {
    for (int i = 0; i <= cntNodes; ++i) {
        height.push_back(1);
        parent.push_back(0);
    }

    sort(edges.begin(), edges.end());

    vector<Edge> minimumSpanningTree;
    for (const auto& edge : edges) {
        int root1 = root(edge.node1);
        int root2 = root(edge.node2);
        if (root1 != root2) {
            unite(root1, root2);
            minimumSpanningTree.push_back(edge);
            if (minimumSpanningTree.size() == cntNodes - 1)
                break;
        }
    }

    if (minimumSpanningTree.size() != cntNodes - 1)
        minimumSpanningTree.erase(minimumSpanningTree.begin(), minimumSpanningTree.end());
    return minimumSpanningTree;
}

int main() {
    read("grafpond.in");
    vector<Edge> MST = kruskal();
    for (const auto& edge : MST)
        cout << edge.node1 << " " << edge.node2 << " " << edge.weight << "\n";
    return 0;
}
