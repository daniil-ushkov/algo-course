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
vector<int> cut_vertex;

void dfs(int u, int p) {
    used[u] = true;
    t[u] = min_t[u] = cur_t++;
    int cnt = 0;
    for (edge e : graph[u]) {
        if (!used[e.to]) {
            dfs(e.to, u);
            min_t[u] = min(min_t[u], min_t[e.to]);
            if (min_t[e.to] >= t[u]) {
                ++cnt;
            }
        } else if (e.to != p) {
            min_t[u] = min(min_t[u], t[e.to]);
        }
    }
    if (p == -1 && cnt >= 2 || p != -1 && cnt >= 1) {
        cut_vertex.push_back(u);
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
    for (int u = 0; u < n; ++u) {
        if (!used[u]) {
            dfs(u, -1);
        }
    }
    cout << cut_vertex.size() << "\n";
    sort(cut_vertex.begin(), cut_vertex.end());
    for (int u : cut_vertex) {
        cout << u + 1 << " ";
    }
    return 0;
}
