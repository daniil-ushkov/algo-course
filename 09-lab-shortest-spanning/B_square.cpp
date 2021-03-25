#include <bits/stdc++.h>
#pragma GCC optimize("O3")

using namespace std;

typedef long long ll;

struct edge {
    int to;
    ll w;
};

const ll INF = 1e18;

int n, s, f;
vector<vector<edge>> g;

void read() {
    ios_base::sync_with_stdio(false), cin.tie(0), cout.tie(0);
    cin >> n >> s >> f;
    --s;
    --f;
    g.assign(n, {});
    for (int u = 0; u < n; ++u) {
        for (int v = 0; v < n; ++v) {
            int w;
            cin >> w;
            if (w != -1 && u != v) {
                g[u].push_back({v, w});
            }
        }
    }
}

ll ans;

void dijkstra() {
    vector<ll> d(n, INF);
    vector<bool> used(n, false);

    d[s] = 0;

    for (int i = 0; i < n; ++i) {
        int u = -1;
        for (int v = 0; v < n; ++v) {
            if (!used[v] && (u == -1 || d[v] < d[u])) {
                u = v;
            }
        }
        if (d[u] == INF || u == f) {
            break;
        }
        used[u] = true;

        for (edge e : g[u]) {
            d[e.to] = min(d[e.to], d[u] + e.w);
        }
    }
    ans = d[f];
}

int main() {
    read();
    dijkstra();
    cout << (ans == INF ? -1 : ans);
    return 0;
}
