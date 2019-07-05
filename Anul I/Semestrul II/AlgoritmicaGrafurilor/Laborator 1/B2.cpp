#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <queue>

using namespace std;

const int NMAX = 1005;

struct Elem {
    int dist;
    pair<int, int> coord;
};

int cntLines, cntColumns;
Elem matrix[NMAX][NMAX];
queue<pair<int, int>> queueNodes;
vector<pair<int, int>> points;

void read() {
    ifstream fin("graf.in");
    if (!fin) {
        cout << "Fisierul nu a putut fi deschis!\n";
        exit(EXIT_FAILURE);
    }

    fin >> cntLines >> cntColumns;
    for (int i = 1; i <= cntLines; ++i)
        for (int j = 1; j <= cntColumns; ++j) {
            int value;
            fin >> value;
            if (value) {
                queueNodes.push(make_pair(i, j));
                matrix[i][j].dist = 0;
                matrix[i][j].coord = make_pair(i, j);
            }
            else {
                matrix[i][j].dist = cntLines * cntColumns;
            }
        }

    int x, y;
    while (fin >> x >> y) {
        points.emplace_back(make_pair(x, y));
    }
}

void bfs() {
    const int dx[] = {-1, 0, 1, 0};
    const int dy[] = {0, -1, 0, 1};

    while (!queueNodes.empty()) {
        int currentLine = queueNodes.front().first;
        int currentColumn = queueNodes.front().second;
        queueNodes.pop();
        for (int i = 0; i < 4; ++i) {
            int newLine = currentLine + dx[i];
            int newColumn = currentColumn + dy[i];
            if (newLine >= 1 && newLine <= cntLines && newColumn >= 1 && newColumn <= cntColumns &&
            matrix[newLine][newColumn].dist > matrix[currentLine][currentColumn].dist + 1) {
                matrix[newLine][newColumn].dist = matrix[currentLine][currentColumn].dist + 1;
                matrix[newLine][newColumn].coord = matrix[currentLine][currentColumn].coord;
                queueNodes.push(make_pair(newLine, newColumn));
            }
        }
    }
}

void print() {
    ofstream fout("graf.out");

    for (const auto& pos : points) {
        int line = pos.first;
        int column = pos.second;
        fout << matrix[line][column].dist << " " << matrix[line][column].coord.first << " " << matrix[line][column].coord.second << "\n";
    }

   fout.close();
}

 int main() {
    read();
    bfs();
    print();
    return 0;
 }
