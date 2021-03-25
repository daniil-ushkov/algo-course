#include <bits/stdc++.h>

#pragma GCC optimize("O3")

using namespace std;

typedef long long ll;

const ll INF = 1e9;

struct edge {
    int from;
    int to;
    int w;
};

int n, m;
vector<edge> edges;

void read() {
    ios_base::sync_with_stdio(false), cin.tie(0), cout.tie(0);
    cin >> n >> m;
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        edges.push_back({u - 1, v - 1, w});
    }
}

vector<ll> d;
void bellman_ford() {
    d.assign(n, INF);
    d[0] = 0;
    for (int k = 1; k < n; ++k) {
        for (edge e : edges) {
            if (d[e.from] < INF) {
                d[e.to] = min(d[e.to], d[e.from] + e.w);
            }
        }
    }
}

void write() {
    for (int u = 0; u < n; ++u) {
        cout << (d[u] == INF ? -1 : d[u]) << " ";
    }
}

int main() {
    read();
    bellman_ford();
    write();
    return 0;
}
