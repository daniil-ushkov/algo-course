#include <bits/stdc++.h>
#pragma GCC optimize("O3")

using namespace std;

typedef long long ll;

struct edge {
    int to;
    ll w;
};

const ll INF = 1e18;

int n, m;
vector<vector<edge>> g;

void read() {
    ios_base::sync_with_stdio(false), cin.tie(0), cout.tie(0);
    cin >> n >> m;
    g.assign(n, {});
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        g[u - 1].push_back({v - 1, w});
        g[v - 1].push_back({u - 1, w});
    }
}

vector<ll> d;
void dijkstra() {
    d.assign(n, INF);
    vector<bool> used(n, false);
    set<pair<ll, int>> q;

    d[0] = 0;
    q.insert({d[0], 0});

    while (!q.empty()) {
        auto it = q.begin();
        int u = it->second;
        used[u] = true;
        q.erase(it);

        for (edge e : g[u]) {
            int v = e.to;
            if (!used[v] && d[v] > d[u] + e.w) {
                q.erase({d[v], v});
                d[v] = d[u] + e.w;
                q.insert({d[v], v});
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
    dijkstra();
    write();
    return 0;
}
