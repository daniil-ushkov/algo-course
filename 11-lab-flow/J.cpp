#include <vector>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <queue>

#pragma GCC optimize("O3")

using namespace std;

struct solver {
    const size_t DIST_INF = 1000ul;
    const int64_t CAP_INF = 2000000000l;

    struct edge {
        size_t to;
        int64_t cap;
        int64_t flow;
        size_t rev_id;

        size_t cost;
    };

#define REV(e) g[(e).to][(e).rev_id]

    solver(vector<vector<edge>> &g, size_t s, size_t t)
    : min_cost(0), n(g.size()), s(s), t(t), g(g) {}

    int64_t flow() {
        int64_t val = 0l;
        vector<size_t> dist;
        while (bfs_layer_net_(s, dist)) {
            int64_t blocking_flow;
            vector<size_t> useful_id(n, 0ul);
            while ((blocking_flow = dfs_flow_(s, useful_id, dist, CAP_INF)) != 0) {
                val += blocking_flow;
            }
        }
        return val;
    }

    void clear() {
        for (auto &u : g) {
            for (edge &e : u) {
                e.flow = 0l;
            }
        }
    }

    size_t min_cost;

 private:
    bool bfs_layer_net_(size_t u, vector<size_t> &dist) {
        dist.assign(n, DIST_INF);
        queue<size_t> q;
        dist[u] = 0ul;
        q.push(u);
        while (!q.empty()) {
            size_t v = q.front();
            q.pop();
            for (edge &e : g[v]) {
                if (e.cap > e.flow && e.cost >= min_cost && dist[e.to] == DIST_INF) {
                    dist[e.to] = dist[v] + 1;
                    q.push(e.to);
                }
            }
        }
        return dist[t] != DIST_INF;
    }

    int64_t dfs_flow_(size_t u, vector<size_t> &useful_id,
                      vector<size_t> &dist, int64_t c_min) {
        if (dist[u] == dist[t]) {
            return u == t ? c_min : 0l;
        }
        for (; useful_id[u] < g[u].size(); ++useful_id[u]) {
            edge& e = g[u][useful_id[u]];
            if (e.cap > e.flow && e.cost >= min_cost && dist[u] + 1 == dist[e.to]) {
                int64_t val = dfs_flow_(e.to, useful_id, dist,
                                        min(c_min, e.cap - e.flow));
                if (val != 0l) {
                    e.flow += val;
                    REV(e).flow -= val;
                    return val;
                }
            }
        }
        return 0l;
    }

    size_t n, s, t;
    vector<vector<edge>> g;

#undef REV
};

using edge = solver::edge;

size_t n;
size_t s, t;
size_t max_cost = 0;
vector<vector<edge>> g;

void read() {
    cin >> n;
    g.assign(2 * n + 2, {});
    for (size_t u = 0; u < n; ++u) {
        for (size_t v = n; v < 2 * n; ++v) {
            size_t cost;
            cin >> cost;
            max_cost = max(max_cost, cost);
            edge e = {v, 1l, 0l, g[v].size(), cost};
            edge rev_e = {u, 0l, 0l, g[u].size(), cost};
            g[u].push_back(e);
            g[v].push_back(rev_e);
        }
    }
    s = 2 * n;
    t = 2 * n + 1;
    for (size_t u = 0; u < n; ++u) {
        edge e = {u, 1l, 0l, g[u].size(), SIZE_MAX};
        edge rev_e = {s, 0l, 0l, g[s].size(), SIZE_MAX};
        g[s].push_back(e);
        g[u].push_back(rev_e);
    }
    for (size_t u = n; u < 2 * n; ++u) {
        edge e = {t, 1l, 0l, g[t].size(), SIZE_MAX};
        edge rev_e = {u, 0l, 0l, g[u].size(), SIZE_MAX};
        g[u].push_back(e);
        g[t].push_back(rev_e);
    }
}

void solve() {
    solver solver(g, s, t);
    size_t l = 0;
    size_t r = max_cost + 1;
    while (r - l != 1) {
        size_t m = l + (r - l) / 2;
        solver.min_cost = m;
        if (solver.flow() == n) {
            l = m;
        } else {
            r = m;
        }
        solver.clear();
    }
    cout << l;
}

int main() {
    read();
    solve();
    return 0;
}