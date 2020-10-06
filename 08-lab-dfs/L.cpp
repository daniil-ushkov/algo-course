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

vector<bool> used1;
vector<int> color;

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
    if ((p == -1 && cnt >= 2) || (p != -1 && cnt >= 1)) {
        cut_vertex[u] = true;
    }
}

int cnt_c = 0;
void dfs1(int u, int c) {
    used1[u] = true;
    for (edge e : graph[u]) {
        if (!used1[e.to]) {
            int c1 = min_t[e.to] >= t[u] ? ++cnt_c : c;
            color[e.id] = c1;
            dfs1(e.to, c1);
        } else if (t[e.to] < t[u]) {
            color[e.id] = c;
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
    cut_vertex.resize(n);
    for (int u = 0; u < n; ++u) {
        if (!used[u]) {
            dfs(u, -1);
        }
    }
    used1.resize(n);
    color.resize(m + 1);
    for (int u = 0; u < n; ++u) {
        if (!used1[u]) {
            dfs1(u, 1);
        }
    }
    cout << cnt_c << "\n";
    for (int id = 1; id <= m; ++id) {
        cout << color[id] << " ";
    }
    return 0;
}
