#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>

using namespace std;

const int NMAX = 1005;
const int INF = 0x3f3f3f3f;

int cntNodes, cntEdges;
int graph[NMAX][NMAX];
vector<int> parent;

void read(const string& inputFilePath) {
    ifstream fin(inputFilePath);
    if (!fin) {
        cout << "Fisierul de intrare nu a putut fi deschis!\n";
        exit(EXIT_FAILURE);
    }

    fin >> cntNodes >> cntEdges;

    for (int i = 1; i <= cntNodes; ++i)
        for (int j = 1; j <= cntNodes; ++j)
            graph[i][j] = INF;
    for (int i = 1; i <= cntNodes; ++i)
        graph[i][i] = 0;

    for (int i = 0; i < cntEdges; ++i) {
        int node1, node2, weight;
        fin >> node1 >> node2 >> weight;
        graph[node1][node2] = graph[node2][node1] = weight;
    }

    fin.close();
}

void floydWarshall() {
    for (int k = 1; k <= cntNodes; ++k)
        for (int i = 1; i <= cntNodes; ++i)
            for (int j = 1; j <= cntNodes; ++j)
                if (graph[i][j] > graph[i][k] + graph[k][j]) {
                    graph[i][j] = graph[i][k] + graph[k][j];
                    if (i <= j)
                        parent[j] = k;
                }
}

vector<int> calculateEccentricity() {
    vector<int> eccentricity(cntNodes + 1, 0);
    for (int i = 1; i <= cntNodes; ++i)
        for (int j = 1; j <= cntNodes; ++j)
            eccentricity[i] = max(eccentricity[i], graph[i][j]);
    return eccentricity;
}

int radius() {
    vector<int> eccentricity = calculateEccentricity();
    int rad = INF;
    for (int i = 1; i <= cntNodes; ++i)
        rad = min(rad, eccentricity[i]);
    return rad;
}

vector<int> center() {
    vector<int> centerGraph;
    vector<int> eccentricity = calculateEccentricity();
    int rad = radius();
    for (int i = 1; i <= cntNodes; ++i)
        if (eccentricity[i] == rad)
            centerGraph.push_back(i);
    return centerGraph;
}

int diameter() {
    int d = 0;
    for (int i = 1; i <= cntNodes; ++i)
        for (int j = 1; j <= cntNodes; ++j)
            if (graph[i][j] != INF)
                d = max(d, graph[i][j]);
    return d;
}

void printPath() {
    int diam = diameter();
    bool done = false;
    for (int i = 1; i <= cntNodes && !done; ++i)
        for (int j = 1; j <= cntNodes && !done; ++j)
            if (graph[i][j] == diam) {
                cout << j << " ";
                while (parent[j]) {
                    j = parent[j];
                    cout << j << " ";
                }
                done = true;
            }
}

int main() {
    read("grafpond.in");
    for (int i = 0; i <= cntNodes; ++i)
        parent.push_back(0);
    floydWarshall();

    cout << radius() << "\n";
    cout << diameter() << "\n";
    vector<int> centerGraph = center();
    for (const auto& it : centerGraph)
        cout << it << " ";
    cout << "\n";

    printPath();
    return 0;
}
