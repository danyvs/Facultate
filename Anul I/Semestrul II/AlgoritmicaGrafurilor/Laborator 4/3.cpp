#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <map>

using namespace std;

struct Edge {
    int node1, node2, weight;
    bool operator < (const Edge& edge) {
        return weight < edge.weight;
    }
};

int cntNodes, clusters;
vector<string> words;
map<string, int> wordToNumber;
vector<int> height;
vector<int> parent;
map<int, vector<int>> graph;
vector<bool> visited;

void read(const string& inputFilePath) {
    ifstream fin(inputFilePath);
    if (!fin) {
        cout << "Input file could not be opened!\n";
        exit(EXIT_FAILURE);
    }

    string word;
    while (fin >> word) {
        words.push_back(word);
        wordToNumber[word] = cntNodes++;
    }

    fin.close();

    cin >> clusters;
}

int levenshteinDistance(string s1, string s2){
    vector<int> ci1(s2.size() + 1);
    vector<int> ci(s2.size() + 1);
    for(int j = 0; j <= s2.size(); ++j)
        ci1[j] = j;

    for(int i = 1; i <= s1.size(); ++i) {
        ci[0] = i;
        for(int j = 1; j <= s2.size(); ++j)
            if (s1[i - 1] == s2[j - 1])
                ci[j] = ci1[j - 1];
            else
                ci[j] = 1 + min(min(ci1[j], ci[j - 1]), ci1[j - 1]);

        for(int j = 0; j <= s2.size(); ++j)
            ci1[j] = ci[j];
    }

    return ci[s2.size()];
}

vector<Edge> constructEdges() {
    vector<Edge> edges;
    for (auto it1 = words.begin(); it1 != words.end(); ++it1)
        for (auto it2 = it1 + 1; it2 != words.end(); ++it2) {
            Edge edge;
            edge.node1 = wordToNumber[*it1];
            edge.node2 = wordToNumber[*it2];
            edge.weight = levenshteinDistance(*it1, *it2);
            edges.push_back(edge);
        }
    return edges;
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
    if (height[root1] > height[root2])
        parent[root2] = root1;
    else {
        parent[root1] = root2;
        if (height[root1] == height[root2])
            ++height[root2];
    }
}

void kruskal() {
    for (int i = 0; i <= cntNodes; ++i) {
        height.push_back(1);
        parent.push_back(0);
    }

    vector<Edge> edges = constructEdges();
    sort(edges.begin(), edges.end());

    int cntSelectedEges = 0;
    for (const auto& edge : edges) {
        int root1 = root(edge.node1);
        int root2 = root(edge.node2);
        if (root1 != root2) {
            unite(root1, root2);
            graph[edge.node1].push_back(edge.node2);
            graph[edge.node2].push_back(edge.node1);
            ++cntSelectedEges;
            if (cntSelectedEges == cntNodes - clusters)
                break;
        }
    }

    if (cntSelectedEges != cntNodes - clusters) {
        cout << "Nu se pot crea " << clusters << " clustere!\n";
        exit(EXIT_FAILURE);
    }
}

void dfs(int currentNode) {
    visited[currentNode] = true;
    cout << words[currentNode] << " ";
    for (const auto& node : graph[currentNode])
        if (!visited[node])
            dfs(node);
}

void printClusters() {
    for (int i = 0; i < cntNodes; ++i)
        visited.push_back(false);
    for (int i = 0; i < cntNodes; ++i)
        if (!visited[i]) {
            dfs(i);
            cout << "\n";
        }
}

int clusterSeparation() {
    int minSeparation = 0x3f3f3f3f;
    for (auto it1 = words.begin(); it1 != words.end(); ++it1)
        for (auto it2 = it1 + 1; it2 != words.end(); ++it2)
            if (root(wordToNumber[*it1]) != root(wordToNumber[*it2]))
                minSeparation = min(minSeparation, levenshteinDistance(*it1, *it2));
    return minSeparation;
}

int main() {
    read("cuvinte.in");
    kruskal();
    printClusters();
    cout << clusterSeparation() << "\n";
    return 0;
}
