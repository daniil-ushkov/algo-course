#include "bits/stdc++.h"

using namespace std;

struct solver {
    struct edge {
        int to;
        int c;
        int rev_id;
    };

    explicit solver(vector<vector<edge>> &g, int s, int t)
    : n(g.size()),
      s(s),
      t(t),
      g(g) {}

    int flow() {
        vector<int> rev_id(n);
        vector<bool> used(n);
        queue<int> q;
        used[s] = true;
        q.push(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (edge &e : g[u]) {
                if (e.c != 0 && !used[e.to]) {
                    rev_id[e.to] = e.rev_id;
                    used[e.to] = true;
                    q.push(e.to);
                }
            }
        }
        if (!used[t]) {
            return 0;
        }
        int c_min = INT_MAX;

#define REV(e) g[e.to][e.rev_id]

        for (int u = t; u != s; u = g[u][rev_id[u]].to) {
            edge& e = g[u][rev_id[u]];
            c_min = min(c_min, REV(e).c);
        }
        for (int u = t; u != s; u = g[u][rev_id[u]].to) {
            edge& e = g[u][rev_id[u]];
            REV(e).c -= c_min;
            e.c += c_min;
        }

#undef REV

        return c_min + flow();
    }

    int n;
    int s, t;
    vector<vector<edge>> g;
};

using edge = solver::edge;

int n, m;
vector<vector<edge>> g;

void read() {
    cin >> n >> m;
    g.assign(n, {});
    for (int i = 0; i < m; ++i) {
        int u, v, c;
        cin >> u >> v >> c;
        u--, v--;
        edge e = {v, c, static_cast<int>(g[v].size())};
        edge rev = {u, 0, static_cast<int>(g[u].size())};
        g[u].push_back(e);
        g[v].push_back(rev);
    }
}

void solve() {
    solver s(g, 0, n - 1);
    cout << s.flow();
}

int main() {
    read();
    solve();
    return 0;
}
