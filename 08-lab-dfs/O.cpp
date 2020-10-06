#include <bits/stdc++.h>

using namespace std;

int x, m, n;
vector<vector<int>> graph;
vector<vector<int>> graph_rev;
vector<int> neg;
vector<vector<int>> port;

void add_edges(int u, int v) {
    graph[neg[u]].push_back(v);
    graph_rev[v].push_back(neg[u]);

    graph[neg[v]].push_back(u);
    graph_rev[u].push_back(neg[v]);
}

void read() {
    cin >> x;
    n = 2 * x;
    vector<int> wire_color(x);
    for (int i = 0; i < x; ++i) {
        cin >> wire_color[i];
    }
    graph.assign(n, {});
    graph_rev.assign(n, {});
    port.assign(x, {});
    neg.assign(n, 0);
    vector<int> port_color(n);
    for (int i = 0; i < n; ++i) {
        int wire;
        cin >> wire;
        port[wire - 1].push_back(i);
        port_color[i] = wire_color[wire - 1];
    }
    neg.assign(n, 0);
    for (int i = 0; i < x; ++i) {
        neg[port[i][0]] = port[i][1];
        neg[port[i][1]] = port[i][0];
    }
    for (int i = 0; i < n; ++i) {
        if (port_color[i] == port_color[(i + 1) % n]) {
            add_edges(neg[i], neg[(i + 1) % n]);
        }
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
        if (comp[u] > comp[neg[u]]) {
            value[u] = 1;
            value[neg[u]] = 0;
        }
        if (comp[u] < comp[neg[u]]) {
            value[u] = 0;
            value[neg[u]] = 1;
        }
        if (comp[u] == comp[neg[u]]) {
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
    for (int i = 0; i < x; ++i) {
        cout << (value[port[i][0]] ? port[i][0] : port[i][1]) + 1 << " ";
    }
}

int main() {
    read();
    scc();
    two_sat();
    write();
    return 0;
}

