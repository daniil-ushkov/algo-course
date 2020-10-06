#include <bits/stdc++.h>

using namespace std;

struct edge {
    int to;
    int id;
};

int n, m;
vector<vector<edge>> graph;
vector<bool> used;
vector<int> t;
vector<int> min_t;
int cur_t = 0;
vector<bool> deleted;
vector<int> color;

void dfs(int u, int p) {
    used[u] = true;
    t[u] = min_t[u] = cur_t++;
    for (edge e : graph[u]) {
        if (!used[e.to]) {
            dfs(e.to, u);
            if (min_t[e.to] == t[e.to]) {
                deleted[e.id] = true;
            }
        }
        if (e.to != p) {
            min_t[u] = min(min_t[u], min_t[e.to]);
        }
    }
}

void dfs1(int u, int c) {
    color[u] = c;
    for (edge e : graph[u]) {
        if (color[e.to] == 0 && !deleted[e.id]) {
            dfs1(e.to, c);
        }
    }
}

int main() {
    cin >> n >> m;
    graph.resize(n);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        graph[--u].push_back({--v, i + 1});
        graph[v].push_back({u, i + 1});
    }
    used.resize(n);
    t.resize(n);
    min_t.resize(n);
    deleted.resize(m + 1);
    for (int u = 0; u < n; ++u) {
        if (!used[u]) {
            dfs(u, -1);
        }
    }
    color.resize(n);
    int c = 0;
    for (int u = 0; u < n; ++u) {
        if (color[u] == 0) {
            dfs1(u, ++c);
        }
    }
    cout << c << "\n";
    for (int u = 0; u < n; ++u) {
        cout << color[u] << " ";
    }
    return 0;
}
