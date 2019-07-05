#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <cmath>
#include <map>

using namespace std;

struct Edge {
    int node1, node2;
    double weight;
    bool operator < (const Edge& edge) {
        return weight < edge.weight;
    }
};

int cntCentrals, cntBlocks, cntEdges;
vector<pair<int, int>> buildings;
vector<Edge> edges;
vector<int> height;
vector<int> parent;
double sum;
vector<Edge> solution;

double euclideanDistance(const pair<int, int>& A, const pair<int, int>& B) {
    return sqrt((A.first - B.first) * (A.first - B.first) + (A.second - B.second) * (A.second - B.second));
}

void read(const string& inputFilePath) {
    ifstream fin(inputFilePath);
    if (!fin) {
        cout << "Couldn't opened input file!\n";
        exit(EXIT_FAILURE);
    }

    fin >> cntCentrals >> cntBlocks >> cntEdges;

    buildings.emplace_back(make_pair(0, 0));
    for (int i = 0; i < cntCentrals + cntBlocks; ++i) {
        int x, y;
        fin >> x >> y;
        buildings.emplace_back(make_pair(x, y));
    }

    for (int i = 0; i < cntEdges; ++i) {
        Edge edge;
        fin >> edge.node1 >> edge.node2;
        edge.weight = euclideanDistance(buildings[edge.node1], buildings[edge.node2]);
        edges.push_back(edge);
    }

    fin.close();
}

int root(int node) {
    int r = node;
    while (parent[r])
        r = parent[r];
    while (parent[node]) {
        int tmp = parent[node];
        parent[node] = r;
        node = tmp;
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
    for (int i = 0; i <= cntCentrals + cntBlocks + 1; ++i) {
        height.push_back(1);
        parent.push_back(0);
    }

    for (int i = 1; i <= cntCentrals; ++i)
        unite(root(cntCentrals + cntBlocks + 1), root(i));

    sort(edges.begin(), edges.end());

    for (const auto& edge : edges) {
        int root1 = root(edge.node1);
        int root2 = root(edge.node2);
        if (root1 != root2) {
            unite(root1, root2);
            sum += edge.weight;
            solution.push_back(edge);
            if (solution.size() == cntBlocks)
                break;
        }
    }
}

void print(const string& outputFilePath) {
    ofstream fout(outputFilePath);

    fout << sum << "\n";
    for (const auto& edge : solution)
        fout << edge.node1 << " " << edge.node2 << "\n";

    fout.close();
}

int main() {
    read("retea2.in");
    kruskal();
    print("retea2.out");
    return 0;
}
