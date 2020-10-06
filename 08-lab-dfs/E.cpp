#include <bits/stdc++.h>

using namespace std;

int n, m;
vector<vector<int>> graph;
vector<int> color;
vector<int> path;
vector<int> cycle_path;

void dfs(int u) {
    color[u] = 1;
    path.push_back(u);
    for (int v : graph[u]) {
        switch (color[v]) {
            case 0:
                dfs(v);
                break;
            case 1:
                int i = 0;
                while (path[i] != v) {
                    ++i;
                }
                cycle_path = vector<int>(path.begin() + i, path.end());
                break;
        }
    }
    color[u] = 2;
    path.pop_back();
}

int main() {
    cin >> n >> m;
    graph.resize(n);
    color.resize(n);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        graph[--u].push_back(--v);
    }
    for (int u = 0; u < n; ++u) {
        if (color[u] == 0) {
            dfs(u);
        }
    }
    if (cycle_path.empty()) {
        cout << "NO\n";
    } else {
        cout << "YES\n";
        for (int u : cycle_path) {
            cout << u + 1 << " ";
        }
    }
    return 0;
}
