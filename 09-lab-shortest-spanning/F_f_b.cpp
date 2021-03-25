#include <bits/stdc++.h>

#pragma GCC optimize("O3")

using namespace std;

typedef long long ll;

const ll INF = 2 * 1e18;

struct edge {
    int from;
    int to;
    ll w;
};

int n;
vector<edge> edges;

void read() {
    ios_base::sync_with_stdio(false), cin.tie(0), cout.tie(0);
    cin >> n;
    for (int u = 0; u < n; ++u) {
        for (int v = 0; v < n; ++v) {
            ll w;
            cin >> w;
            if (w != (int) 1e9) {
                edges.push_back({u, v, w});
            }
        }
    }
}

vector<ll> d;
vector<int> p;
vector<int> cycle;

void find_cycle(int u) {
    for (int i = 0; i < n; ++i) {
        u = p[u];
    }
    int v = u;
    cycle.push_back(u);
    for (u = p[u]; u != v; u = p[u]) {
        cycle.push_back(u);
    }
    cycle.push_back(u);
}

void ford_bellman() {
    d.assign(n, 0);
    p.assign(n, -1);
    for (int i = 1; i < n; ++i) {
        for (edge e : edges) {
            if (d[e.to] > d[e.from] + e.w) {
                d[e.to] = d[e.from] + e.w;
                p[e.to] = e.from;
            }
        }
    }
    int u = -1;
    for (edge e : edges) {
        if (d[e.to] > d[e.from] + e.w) {
            d[e.to] = d[e.from] + e.w;
            p[e.to] = e.from;
            u = e.to;
        }
    }
    if (u != -1) {
        find_cycle(u);
    }
}

int main() {
    read();
    ford_bellman();
    if (cycle.empty()) {
        cout << "NO\n";
        return 0;
    }
    cout << "YES\n" << cycle.size() << "\n";
    reverse(cycle.begin(), cycle.end());
    for (int u : cycle) {
        cout << u + 1 << " ";
    }
    return 0;
}
