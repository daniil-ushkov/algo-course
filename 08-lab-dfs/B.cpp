#include <bits/stdc++.h>

using namespace std;

int n, m;
vector<vector<int>> graph;
vector<vector<int>> graph_rev;
vector<int> color;
vector<int> sorted;
vector<int> comp;

void dfs(int u) {
    color[u] = 1;
    for (int v : graph[u]) {
        if (color[v] == 0) {
            dfs(v);
        }
    }
    color[u] = 2;
    sorted.push_back(u);
}

void dfs_rev(int u, int c) {
    comp[u] = c;
    for (int v : graph_rev[u]) {
        if (comp[v] == 0) {
            dfs_rev(v, c);
        }
    }
}

int main() {
    cin >> n >> m;
    graph.resize(n);
    graph_rev.resize(n);
    color.resize(n);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        graph[--u].push_back(--v);
        graph_rev[v].push_back(u);
    }
    for (int u = 0; u < n; ++u) {
        if (color[u] == 0) {
            dfs(u);
        }
    }
    reverse(sorted.begin(), sorted.end());
    comp.resize(n);
    int c = 0;
    for (int u : sorted) {
        if (comp[u] == 0) {
            dfs_rev(u, ++c);
        }
    }
    cout << c << "\n";
    for (int u_comp : comp) {
        cout << u_comp << " ";
    }
    return 0;
}
