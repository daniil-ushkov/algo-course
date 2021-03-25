#include <bits/stdc++.h>

using namespace std;

int n, m;
vector<vector<int>> graph;
vector<int> color;
bool cycle = false;
vector<int> sorted;

void dfs(int u) {
    color[u] = 1;
    for (int v : graph[u]) {
        switch (color[v]) {
            case 0:
                dfs(v);
                break;
            case 1:
                cycle = true;
                break;
        }
    }
    color[u] = 2;
    sorted.push_back(u);
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
    if (cycle) {
        cout << -1;
        return 0;
    }
    for (int i = n - 1; i >= 0; --i) {
        cout << sorted[i] + 1 << " ";
    }
    return 0;
}
