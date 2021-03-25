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
            if (w != -1) {
                g[u].push_back({v, w});
            }
        }
    }
}

ll ans;
void dijkstra() {
    vector<ll> d(n, INF);
    vector<bool> used(n, false);
    set<pair<ll, int>> q;

    d[s] = 0;
    q.insert({d[s], s});

    while (!q.empty() && !used[f]) {
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
    ans = d[f];
}

int main() {
    read();
    dijkstra();
    cout << (ans == INF ? -1 : ans);
    return 0;
}
