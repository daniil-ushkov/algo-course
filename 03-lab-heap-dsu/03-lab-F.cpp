#include <iostream>
#include <vector>
#include <stack>

using namespace std;

class DSU {
 public:
  DSU(int number) {
    int n = number;
    p = new int[n];
    r = new int[n];
    for (int i = 0; i < n; i++) {
      p[i] = i;
      r[i] = 0;
    }
  }
  int find_leader(int x) {
    if (p[x] == x) {
      return x;
    }
    return p[x] = find_leader(p[x]);
  }
  void merge(int a, int b) {
    a = find_leader(a);
    b = find_leader(b);
    if (a == b) {
      return;
    }
    if (r[a] > r[b]) {
      swap(a, b);
    }
    p[a] = b;
    if (r[a] == r[b]) {
      r[b]++;
    }
  }

 private:
  int n;
  int *p;
  int *r;
};
int main() {
  int n, m, k;
  cin >> n >> m >> k;
  int trash;
  for (int i = 0; i < m; i++) {
    cin >> trash >> trash;
  }
  stack<pair<string, pair<int, int>>> commands;
  string command;
  int x, y;
//  vector<vector<int>> g(n, vector<int>(n, 0));
  for (int i = 0; i < k; i++) {
    cin >> command >> x >> y;
    commands.push({command, {x - 1, y - 1}});
    if (command == "cut") {
//      g[x - 1][y - 1]++;
//      g[y - 1][x - 1]++;
    }
  }
  DSU *dsu = new DSU(n);
  stack<string> answers;
  while (!commands.empty()) {
    pair<string, pair<int, int>> command = commands.top();
    int x = command.second.first;
    int y = command.second.second;
    commands.pop();
    if (command.first == "ask") {
      if (dsu->find_leader(x) == dsu->find_leader(y)) {
        answers.push("YES");
      } else {
        answers.push("NO");
      }
    } else {
//      if (g[x][y] == 1) {
      dsu->merge(x, y);
//      }
//      g[x][y] = max(0, g[x][y] - 1);
//      g[y][x] = max(0, g[y][x] - 1);
    }
  }
  while (!answers.empty()) {
    cout << answers.top() << endl;
    answers.pop();
  }
}
