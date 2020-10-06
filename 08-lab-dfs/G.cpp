#include <bits/stdc++.h>

using namespace std;

int n, m;
vector<vector<int>> graph;
vector<int> color;

void dfs(int u, int c) {
    color[u] = c;
    for (int v : graph[u]) {
        if (color[v] == 0) {
            dfs(v, c);
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
    color.resize(n);
    int c = 0;
    for (int u = 0; u < n; ++u) {
        if (color[u] == 0) {
            dfs(u, ++c);
        }
    }
    cout << c << "\n";
    for (int u_color : color) {
        cout << u_color << " ";
    }
    return 0;
}
