#include <vector>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <queue>
#include <algorithm>

#pragma GCC optimize("O3")

using namespace std;

struct flow_solver {
    inline static const size_t DIST_INF = 1000ul;
    inline static const int64_t CAP_INF = 2000000000l;

    struct edge {
        size_t to;
        int64_t cap;
        int64_t flow;
        size_t rev_id;
        size_t edge_id;
    };

#define REV(e) g[(e).to][(e).rev_id]

    flow_solver() = default;

    flow_solver(vector<vector<edge>> &g, size_t s, size_t t)
    : n(g.size()),
      s(s),
      t(t),
      g(g) {}

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

    vector<size_t> min_cut() {
        flow();
        vector<size_t> dist;
        bfs_layer_net_(s, dist);
        vector<size_t> res;
        for (size_t u = 0; u < n; ++u) {
            if (dist[u] != DIST_INF) {
                res.push_back(u);
            }
        }
        return res;
    }

    vector<pair<size_t, vector<size_t>>> decompose() {
        flow();
        vector<pair<size_t, vector<size_t>>> res;
        int64_t val;
        vector<size_t> useful_id(n, 0ul);
        vector<bool> used(n, false);
        vector<size_t> path;
        while ((val = dfs_decompose_(s, useful_id, used, path, CAP_INF)) != 0) {
            reverse(path.begin(), path.end());
            res.emplace_back(val, path);
            used.assign(n, false);
            path.clear();
        }
        return res;
    }

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
                if (e.cap > e.flow && dist[e.to] == DIST_INF) {
                    dist[e.to] = dist[v] + 1;
                    q.push(e.to);
                }
            }
        }
        return dist[t] != DIST_INF;
    }

    int64_t dfs_flow_(size_t u, vector<size_t> &useful_id, vector<size_t> &dist, int64_t c_min) {
        if (dist[u] == dist[t]) {
            return u == t ? c_min : 0l;
        }
        for (; useful_id[u] < g[u].size(); ++useful_id[u]) {
            edge& e = g[u][useful_id[u]];
            if (e.cap > e.flow && dist[u] + 1 == dist[e.to]) {
                int64_t val = dfs_flow_(e.to, useful_id, dist, min(c_min, e.cap - e.flow));
                if (val != 0l) {
                    e.flow += val;
                    REV(e).flow -= val;
                    return val;
                }
            }
        }
        return 0l;
    }

    int64_t dfs_decompose_(size_t u, vector<size_t> &useful_id, vector<bool> &used,
                           vector<size_t> &path, int64_t flow_min) {
        if (u == t) {
            return flow_min;
        }
        used[u] = true;
        for (; useful_id[u] < g[u].size(); ++useful_id[u]) {
            edge& e = g[u][useful_id[u]];
            if (e.flow > 0 && !used[e.to]) {
                int64_t val = dfs_decompose_(e.to, useful_id, used, path, min(flow_min, e.flow));
                if (val != 0) {
                    e.flow -= val;
                    REV(e).flow += val;
                    path.push_back(e.edge_id);
                    return val;
                }
            }
        }
        return 0l;
    }

    size_t n{}, s{}, t{};
    vector<vector<edge>> g;

    friend class circulation_solver;

#undef REV
};

struct circulation_solver {
    struct edge {
        size_t from;
        size_t to;
        int64_t cap_l;
        int64_t cap_r;
    };

    circulation_solver(size_t n, size_t m, vector<edge> &edges) : n(n), m(m), sum_cap_l(0) {
        size_t s = 0;
        size_t t = n + 1;

        using flow_edge = flow_solver::edge;
        vector<vector<flow_edge>> g(n + 2);
        for (size_t id = 0; id < m; ++id) {
            edge &e = edges[id];

            flow_edge e1 = {e.to + 1, e.cap_l, 0l, g[e.to + 1].size(), id};
            flow_edge rev_e1 = {s, 0l, 0l, g[s].size(), SIZE_MAX};
            g[s].push_back(e1);
            g[e.to + 1].push_back(rev_e1);

            flow_edge e2 = {e.to + 1, e.cap_r - e.cap_l, 0l, g[e.to + 1].size(), id};
            flow_edge rev_e2 = {e.from + 1, 0l, 0l, g[e.from + 1].size(), SIZE_MAX};
            g[e.from + 1].push_back(e2);
            g[e.to + 1].push_back(rev_e2);

            flow_edge e3 = {t, e.cap_l, 0l, g[t].size(), SIZE_MAX};
            flow_edge rev_e3 = {e.from + 1, 0l, 0l, g[e.from + 1].size(), SIZE_MAX};
            g[e.from + 1].push_back(e3);
            g[t].push_back(rev_e3);

            sum_cap_l += e.cap_l;
        }
        solver = flow_solver(g, s, t);
        flow_val = solver.flow();
    }

    bool has_circulation() const {
        return sum_cap_l == flow_val;
    }

    vector<int64_t> circulation() {
        if (!has_circulation()) {
            return {};
        }
        vector<int64_t> edges_flow(m);
        using edge = flow_solver::edge;
        for (size_t u = 0; u <= n; ++u) {
            for (edge &e : solver.g[u]) {
                if (e.edge_id != SIZE_MAX) {
                    edges_flow[e.edge_id] += e.flow;
                }
            }
        }
        return edges_flow;
    }

 private:
    size_t n;
    size_t m;
    int64_t sum_cap_l;
    flow_solver solver;
    int64_t flow_val;
};

using edge = circulation_solver::edge;

size_t n, m;
vector<edge> edges;

void read() {
    cin >> n >> m;
    for (size_t i = 0; i < m; ++i) {
        size_t u, v;
        int64_t l, r;
        cin >> u >> v >> l >> r;
        u--, v--;
        edges.push_back({u, v, l, r});
    }
}

void solve() {
    circulation_solver solver(n, m, edges);
    if (solver.has_circulation()) {
        cout << "YES\n";
        auto res = solver.circulation();
        for (int64_t flow : res) {
            cout << flow << '\n';
        }
    } else {
        cout << "NO\n";
    }
}

int main() {
    read();
    solve();
    return 0;
}
