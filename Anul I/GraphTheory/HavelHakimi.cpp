#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <map>

using namespace std;

vector<pair<int, int>> sequence;
map<int, vector<int>> graph;

void read() {
    int n;
    cin >> n;
    for (int i = 0; i < n; ++i) {
        int nr;
        cin >> nr;
        sequence.emplace_back(make_pair(nr, i + 1));
    }
}

bool checkNecessaryConditions() {
    int sum = 0;
    for (const auto& it : sequence) {
        sum += it.first;
        if (it.first < 0 || it.first >= sequence.size())
            return false;
    }
    return !(sum % 2);
}

struct compPair {
    bool operator()(pair<int, int> a, pair<int, int> b) {
        return a.first < b.first;
    }
};

bool havelHakimi() {
    priority_queue<pair<int, int>, vector<pair<int, int>>, compPair> maxHeap;
    for (const auto& it : sequence)
        maxHeap.push(it);

    while (!maxHeap.empty()) {
        // get the node with the maximum degree
        pair<int, int> maxNode = maxHeap.top();
        maxHeap.pop();

        vector<pair<int, int>> neighbours;
        for (int i = 0; i < maxNode.first; ++i) {
            // get an adjacent node
            pair<int, int> node = maxHeap.top();
            maxHeap.pop();

            // update the degree
            --node.first;

            // check if the new degree is negative
            if (node.first < 0)
                return false;

            neighbours.push_back(node);
        }

        // update the sequence and build the graph
        for (const auto& node : neighbours) {
            // add the new node into the heap
            maxHeap.push(node);

            // add the edge to the graph
            graph[maxNode.second].push_back(node.second);
            graph[node.second].push_back(maxNode.second);
        }
    }
    return true;
}

void printGraph() {
    for (const auto& node1 : graph)
        for (const auto& node2 : graph[node1.first])
            cout << node1.first << " " << node2 << "\n";
}

int main() {
    read();
    if (!checkNecessaryConditions() || !havelHakimi())
        cout << "We cannot build a graph!\n";
    else {
        cout << "We can build a graph!\n";
        printGraph();
    }

    return 0;
}
