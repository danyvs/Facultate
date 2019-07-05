#include <iostream>
#include <algorithm>
#include <vector>
#include <numeric>
#include <queue>
#include <map>

using namespace std;

vector<int> sequence;
map<int, vector<int>> graph;

void read() {
    int cnt;
    cin >> cnt;
    for (int i = 0; i < cnt; ++i) {
        int temp;
        cin >> temp;
        sequence.push_back(temp);
    }
}

bool checkNecessaryConditions() {
    return accumulate(sequence.begin(), sequence.end(), 0) == 2 * (sequence.size() - 1);
}

struct compPair {
    bool operator()(const pair<int, int>& a, const pair<int, int>& b) {
        return a.first < b.first;
    }
};

void buildTree() {
    priority_queue<pair<int, int>, vector<pair<int, int>>, compPair> maxHeapNonterminals;
    deque<int> terminals;

    for (int i = 0; i < sequence.size(); ++i)
        if (sequence[i] == 1)
            terminals.push_back(i + 1);
        else
            maxHeapNonterminals.push(make_pair(sequence[i], i + 1));

    for (int i = 0; i < sequence.size() - 2; ++i) {
        int degree = maxHeapNonterminals.top().first;
        int node = maxHeapNonterminals.top().second;
        maxHeapNonterminals.pop();

        graph[node].push_back(terminals.front());
        graph[terminals.front()].push_back(node);

        --degree;
        if (degree == 1)
            terminals.push_back(node);
        else
            maxHeapNonterminals.push(make_pair(degree, node));
        terminals.pop_front();
    }

    graph[*terminals.begin()].push_back(*(terminals.begin() + 1));
    graph[*(terminals.begin() + 1)].push_back(*terminals.begin());
}

void print() {
    for (const auto& node1 : graph)
        for (const auto& node2 : graph[node1.first])
            cout << node1.first << " " << node2 << "\n";
}

int main() {
    read();
    if (checkNecessaryConditions()) {
        buildTree();
        print();
    }
    else
        cout << "Cannot build tree!\n";
    return 0;
}
