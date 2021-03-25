#include <bits/stdc++.h>

#pragma GCC optimize("O3")

using namespace std;

typedef long long ll;

struct dsu {
    dsu(int n)
    : n(n),
    p(n),
    r(n, 0) {
        for (int i = 0; i < n; ++i) {
            p[i] = i;
        }
    }

    int find(int u) {
        if (p[u] == u) {
            return u;
        }
        return p[u] = find(p[u]);
    }

    void join(int u, int v) {
        u = find(u);
        v = find(v);
        if (u == v) {
            return;
        }
        if (r[u] < r[v]) {
            swap(u, v);
        }
        p[v] = u;
        if (r[u] == r[v]) {
            r[u]++;
        }
    }

 private:
    int n;
    vector<int> p;
    vector<int> r;
};

struct edge {
    int u;
    int v;
    ll w;
};

int n, m;
vector<edge> edges;

void read() {
    cin >> n >> m;
    for (int i = 0; i < m; ++i) {
        int u, v;
        ll w;
        cin >> u >> v >> w;
        edges.push_back({u - 1, v - 1, w});
    }
}

ll mst_w = 0;

void solve() {
    sort(edges.begin(), edges.end(), [](edge e1, edge e2) { return e1.w < e2.w; });
    dsu d(n);
    for (edge e : edges) {
        if (d.find(e.u) != d.find(e.v)) {
            d.join(e.u, e.v);
            mst_w += e.w;
        }
    }
    cout << mst_w;
}

int main() {
    read();
    solve();
}
