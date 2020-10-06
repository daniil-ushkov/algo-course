#include <bits/stdc++.h>

using namespace std;

int x, m, n;
vector<vector<int>> graph;
vector<vector<int>> graph_rev;

void add_edges(int u, int v) {
    graph[(u + x) % n].push_back(v);
    graph_rev[v].push_back((u + x) % n);

    graph[(v + x) % n].push_back(u);
    graph_rev[u].push_back((v + x) % n);
}

void read() {
    cin >> x >> m;
    n = 2 * x;
    graph.resize(n);
    graph_rev.resize(n);
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        u = u < 0 ? abs(u) + x : u;
        v = v < 0 ? abs(v) + x : v;
        w = w < 0 ? abs(w) + x : w;
        --u; --v; --w;
        add_edges(u, v);
        add_edges(u, w);
        add_edges(v, w);
    }
}

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

void scc() {
    color.resize(n);
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
}

vector<bool> value;
bool exists = true;

void two_sat() {
    value.assign(n, 0);
    for (int u = 0; u < n; ++u) {
        if (comp[u] > comp[(u + x) % n]) {
            value[u] = 1;
            value[(u + x) % n] = 0;
        }
        if (comp[u] < comp[(u + x) % n]) {
            value[u] = 0;
            value[(u + x) % n] = 1;
        }
        if (comp[u] == comp[(u + x) % n]) {
            exists = false;
            return;
        }
    }
}

void write() {
    if (!exists) {
        cout << "NO\n";
        return;
    }
    cout << "YES\n";
    for (int u = 0; u < x; ++u) {
        cout << (value[u] ? "" : "-") << u + 1 << " ";
    }
}

int main() {
    read();
    scc();
    two_sat();
    write();
    return 0;
}

