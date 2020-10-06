#include <bits/stdc++.h>

using namespace std;

int n, m;
vector<vector<int>> graph;
vector<int> sorted;
vector<int> depth;
int max_depth = 0;

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
    cin >> n >> m;
    graph.resize(n);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        graph[--u].push_back(--v);
    }
    top_sort();
    depth.resize(n, 0);
    for (int i = n - 1; i >= 0; --i) {
        int u = sorted[i];
        for (int v : graph[u]) {
            depth[u] = max(depth[u], depth[v] + 1);
        }
        max_depth = max(max_depth, depth[u]);
    }
    cout << (max_depth == n - 1 ? "YES" : "NO");
    return 0;
}
