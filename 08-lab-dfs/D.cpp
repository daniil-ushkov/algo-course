#include <bits/stdc++.h>

using namespace std;

int n, m, s;
vector<vector<int>> graph;
vector<int> sorted;

void top_sort() {
    vector<int> in(n);
    for (int u = 0; u < n; ++u) {
        for (int v : graph[u]) {
            in[v]++;
        }
    }
    queue<int> q;
    for (int u = 0; u < n; ++u) {
        if (in[u] == 0) {
            q.push(u);
        }
    }
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        sorted.push_back(u);
        for (int v : graph[u]) {
            if (--in[v] == 0) {
                q.push(v);
            }
        }
    }
}

int main() {
    cin >> n >> m >> s;
    --s;
    graph.resize(n);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        graph[--u].push_back(--v);
    }
    top_sort();
    vector<bool> victory(n);
    victory[n - 1] = false;
    for (int i = n - 1; i >= 0; --i) {
        int u = sorted[i];
        bool all_victory = true;
        for (int v : graph[u]) {
            all_victory = all_victory && victory[v];
        }
        victory[u] = !all_victory;
    }
    cout << (victory[s] ? "First player wins" : "Second player wins");
    return 0;
}
