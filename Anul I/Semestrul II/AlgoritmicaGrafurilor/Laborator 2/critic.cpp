#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>

using namespace std;

int cntNodes, cntEdges;
map<int, vector<int>> graph;
vector<int> level;
vector<int> lowestLevel;
vector<bool> visited;
vector<int> parent;

void read(const string& inputFilePath) {
    ifstream fin(inputFilePath);
    if (!fin) {
        cout << "Couldn't open input file!\n";
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

void dfsBridges(int currentNode) {
    visited[currentNode] = true;
    lowestLevel[currentNode] = level[currentNode];
    for (const auto& node : graph[currentNode])
        if (!visited[node]) {
            // muchie de avansare
            level[node] = level[currentNode] + 1;
            dfsBridges(node);

            lowestLevel[currentNode] = min(lowestLevel[currentNode], lowestLevel[node]);
            if (lowestLevel[node] > level[currentNode]) {
                cout << currentNode << " " << node << "\n";
            }
        }
        else {
            if (level[node] < level[currentNode] - 1) {
                // muchie de intoarece
                lowestLevel[currentNode] = min(lowestLevel[currentNode], level[node]);
            }
        }

}

void bridges() {
    for (int i = 0; i <= cntNodes; ++i) {
        level.push_back(0);
        lowestLevel.push_back(cntNodes);
        visited.push_back(false);
    }
    level[1] = 1;
    dfsBridges(1);

    level.erase(level.begin(), level.end());
    lowestLevel.erase(lowestLevel.begin(), lowestLevel.end());
    visited.erase(visited.begin(), visited.end());
}

void dfsArticulationPoints(int currentNode) {
    visited[currentNode] = true;
    lowestLevel[currentNode] = level[currentNode];
    int children = 0;
    for (const auto& node : graph[currentNode])
        if (!visited[node]) {
            level[node] = level[currentNode] + 1;
            ++children;
            parent[node] = currentNode;
            dfsArticulationPoints(node);

            lowestLevel[currentNode] = min(lowestLevel[currentNode], lowestLevel[node]);
            if ((!parent[currentNode] && children > 1) || (parent[currentNode] && lowestLevel[node] >= level[currentNode])) {
                cout << currentNode << "\n";
            }
        }
        else {
            if (parent[node] != currentNode)
                lowestLevel[currentNode] = min(lowestLevel[currentNode], level[node]);
        }
}

void articulationPoints() {
    for (int i = 0; i <= cntNodes; ++i) {
        level.push_back(0);
        lowestLevel.push_back(cntNodes);
        visited.push_back(false);
        parent.push_back(0);
    }
    level[1] = 1;
    dfsArticulationPoints(1);

}

int main() {
    read("graf.in");
    // bridges();
    articulationPoints();
    return 0;
}


