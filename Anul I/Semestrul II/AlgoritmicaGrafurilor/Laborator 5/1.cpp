#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <map>
#include <queue>

using namespace std;

int cntNodes, cntEdges;
map<int, vector<int>> graph;
vector<int> duration;
vector<int> parent;

void read(const string& inputFilePath) {
    ifstream fin(inputFilePath);
    if (!fin) {
        cout << "Fisierul de intrare nu a putut fi deschis!\n";
        exit(EXIT_FAILURE);
    }

    fin >> cntNodes;

    duration.push_back(0);
    for (int i = 1; i <= cntNodes; ++i) {
        int t;
        fin >> t;
        duration.push_back(t);
    }

    fin >> cntEdges;
    for (int i = 0; i < cntEdges; ++i) {
        int node1, node2;
        fin >> node1 >> node2;
        graph[node1].push_back(node2);
    }

    fin.close();
}

vector<int> calculateIntDegree() {
    vector<int> intDegree(cntNodes + 1, 0);
    for (int i = 1; i <= cntNodes; ++i)
        for (const auto& node : graph[i])
            ++intDegree[node];
    return intDegree;
}

vector<pair<int, int>> findCriticalPath() {
    vector<int> intDegree = calculateIntDegree();
    vector<pair<int, int>> time(cntNodes + 1, {0, 0});
    queue<int> queueNodes;

    for (int i = 0; i <= cntNodes; ++i)
        parent.push_back(0);

    for (int i = 1; i <= cntNodes; ++i)
        if (!intDegree[i]) {
            queueNodes.push(i);
            time[i].first = 0;
            time[i].second = duration[i];
        }

    while (!queueNodes.empty()) {
        int currentNode = queueNodes.front();
        queueNodes.pop();
        for (const auto& node : graph[currentNode]) {
            --intDegree[node];
            if (time[currentNode].second > time[node].first) {
                time[node].first = time[currentNode].second;
                parent[node] = currentNode;
            }
            if (!intDegree[node]) {
                queueNodes.push(node);
                time[node].second = time[node].first + duration[node];
            }
        }
    }

    return time;
}

void print() {
    vector<pair<int, int>> time = findCriticalPath();

    int maxTimeNode = 0;
    for (int i = 1; i <= cntNodes; ++i)
        if (time[i].second > time[maxTimeNode].second)
            maxTimeNode = i;

    cout << "Timp minim: " << time[maxTimeNode].second << "\n";

    cout << "Activitati critice: ";
    vector<int> criticalActivities;
    criticalActivities.push_back(maxTimeNode);
    while (parent[maxTimeNode]) {
        maxTimeNode = parent[maxTimeNode];
        criticalActivities.push_back(maxTimeNode);
    }
    reverse(criticalActivities.begin(), criticalActivities.end());
    for (const auto& it : criticalActivities)
        cout << it << " ";
    cout << "\n";

    for (int i = 1; i <= cntNodes; ++i)
        cout << i << ": " << time[i].first << " " << time[i].second << "\n";
}


int main() {
    read("activitati.in");
    print();
    return 0;
}
