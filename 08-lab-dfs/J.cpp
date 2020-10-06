#include <bits/stdc++.h>

using namespace std;

int n, m;
vector<vector<int>> graph;
vector<int> color;
bool bipartite = true;

void dfs(int u, int c) {
    color[u] = c;
    for (int v : graph[u]) {
        if (color[v] == c) {
            bipartite = false;
        }
        if (color[v] == -1) {
            dfs(v, 1 - c);
        }
    }
}

int main() {
    cin >> n >> m;
    graph.resize(n);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        graph[--u].push_back(--v);
        graph[v].push_back(u);
    }
    color.resize(n, -1);
    for (int u = 0; u < n; ++u) {
        if (color[u] == -1) {
            dfs(u, 0);
        }
    }
    cout << (bipartite ? "YES" : "NO");
    return 0;
}
