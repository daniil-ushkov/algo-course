#include <bits/stdc++.h>

#pragma GCC optimize("O3")

using namespace std;

typedef long long ll;

const ll INF = 9223372036854775807 / 2;

struct edge {
    int from;
    int to;
    ll w;
};

int n, m, s;
vector<vector<edge>> g;
vector<edge> edges;

void read() {
    ios_base::sync_with_stdio(false), cin.tie(0), cout.tie(0);
    cin >> n >> m >> s;
    --s;
    g.assign(n, {});
    for (int i = 0; i < m; ++i) {
        int u, v;
        ll w;
        cin >> u >> v >> w;
        edges.push_back({u - 1, v - 1, w});
        g[u - 1].push_back({u - 1, v - 1, w});
    }
}

vector<bool> mark;
void dfs(int u) {
    mark[u] = true;
    for (edge e : g[u]) {
        if (!mark[e.to]) {
            dfs(e.to);
        }
    }
}

vector<ll> d;
vector<ll> d1;
void bellman_ford() {
    d.assign(n, INF);
    d[s] = 0;
    for (int k = 1; k < n; ++k) {
        for (edge e : edges) {
            if (d[e.from] != INF) {
                d[e.to] = max(-INF, min(d[e.to], d[e.from] + e.w));
            }
        }
    }
    d1 = d;
    for (int k = 0; k < (int) edges.size(); ++k) {
        for (edge e : edges) {
            if (d1[e.from] != INF) {
                d1[e.to] = max(-INF, min(d1[e.to], d1[e.from] + e.w));
            }
        }
    }
    mark.assign(n, false);
    for (int u = 0; u < n; ++u) {
        if (d[u] == -INF || d1[u] < d[u]) {
            dfs(u);
        }
    }
}

void write() {
    for (int u = 0; u < n; ++u) {
        if (mark[u]) {
            cout << "-\n";
        } else {
            if (d[u] == INF) {
                cout << "*\n";
            } else {
                cout << d[u] << "\n";
            }
        }
    }
}

int main() {
    read();
    bellman_ford();
    write();
    return 0;
}
