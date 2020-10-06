#include <bits/stdc++.h>

using namespace std;

struct edge {
    int u;
    int w;
};

const int INF = 1e9;

int n, m, s, t;
vector<vector<edge>> graph;
vector<int> sorted;

void top_sort() {
    vector<int> in(n);
    for (int u = 0; u < n; ++u) {
        for (edge e : graph[u]) {
            in[e.u]++;
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
        for (edge e : graph[u]) {
            if (--in[e.u] == 0) {
                q.push(e.u);
            }
        }
    }
}

int main() {
    cin >> n >> m >> s >> t;
    --s;
    --t;
    graph.resize(n);
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        graph[--u].push_back({--v, w});
    }
    top_sort();
    vector<int> dist(n, INF);
    dist[t] = 0;
    for (int i = n - 1; i >= 0; --i) {
        int u = sorted[i];
        for (edge e : graph[u]) {
            if (dist[e.u] != INF) {
                dist[u] = min(dist[u], dist[e.u] + e.w);
            }
        }
    }
    if (dist[s] == INF) {
        cout << "Unreachable";
    } else {
        cout << dist[s];
    }
    return 0;
}
