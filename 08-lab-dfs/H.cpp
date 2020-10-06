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
vector<int> bridges;

void dfs(int u, int p) {
    used[u] = true;
    t[u] = min_t[u] = cur_t++;
    for (edge e : graph[u]) {
        if (!used[e.to]) {
            dfs(e.to, u);
            if (min_t[e.to] == t[e.to]) {
                bridges.push_back(e.id);
            }
        }
        if (e.to != p) {
            min_t[u] = min(min_t[u], min_t[e.to]);
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
    for (int u = 0; u < n; ++u) {
        if (!used[u]) {
            dfs(u, -1);
        }
    }
    cout << bridges.size() << "\n";
    sort(bridges.begin(), bridges.end());
    for (int id : bridges) {
        cout << id << " ";
    }
    return 0;
}
