#include "bits/stdc++.h"

using namespace std;

#define fastio ios_base::sync_with_stdio(false), cin.tie(0), cout.tie(0)
#pragma GCC optimize("O3")

using namespace std;

struct solver {
    inline static const size_t DIST_INF = SIZE_MAX / 4;
    inline static const int64_t CAP_INF = INT64_MAX / 4;
    inline static const int64_t COST_INF = INT64_MAX / 4;

    struct edge {
        size_t to;
        int64_t cap;
        int64_t flow;
        int64_t cost;
        size_t rev_id;
        size_t id;
    };

#define REV(e) g[(e).to][(e).rev_id]

    solver(vector<vector<edge>> &g, size_t s, size_t t) : n(g.size()), m(0), s(s), t(t), g(g) {
        for (auto &g_u : g) {
            m += g_u.size();
        }
    }

    void flow_min_cost() {
        vector<int64_t> phi;
        ford_bellman_(phi);
        vector<bool> used(n, false);
        while (dfs_flow_min_cost_(s, used, CAP_INF, phi) != 0) {
            dijkstra_(phi);
            used.assign(n, false);
        }
    }

    void ford_bellman_(vector<int64_t> &phi) {
        phi.assign(n, COST_INF);
        vector<size_t> in(n, 0);
        vector<size_t> sorted;
        for (size_t u = 0; u < n; ++u) {
            for (edge &e : g[u]) {
                if (e.cap > 0) {
                    in[e.to]++;
                }
            }
        }
        queue<size_t> q;
        q.push(s);
        while (!q.empty()) {
            size_t u = q.front();
            q.pop();
            sorted.push_back(u);
            for (edge &e : g[u]) {
                if (e.cap > 0) {
                    in[e.to]--;
                    if (in[e.to] == 0) {
                        q.push(e.to);
                    }
                }
            }
        }
        phi[s] = 0;
        for (size_t i = 0; i < n; ++i) {
            size_t u = sorted[i];
            for (edge &e : g[u]) {
                if (e.cap > 0) {
                    phi[e.to] = min(phi[e.to], phi[u] + e.cost);
                }
            }
        }
    }

    int64_t dfs_flow_min_cost_(size_t u, vector<bool> &used, int64_t c_min, vector<int64_t> &phi) {
        used[u] = true;
        if (u == t) {
            return c_min;
        }
        for (edge &e : g[u]) {
            if (!used[e.to] && e.cap > e.flow && e.cost + phi[u] - phi[e.to] == 0) {
                int64_t val = dfs_flow_min_cost_(e.to, used, min(c_min, e.cap - e.flow), phi);
                if (val != 0) {
                    e.flow += val;
                    REV(e).flow -= val;
                    return val;
                }
            }
        }
        return 0;
    }

    void dijkstra_(vector<int64_t> &phi) {
        vector<int64_t> d(n, COST_INF);
        vector<bool> used(n, false);
        d[s] = 0;
        for (size_t i = 0; i < n; ++i) {
            size_t u = SIZE_MAX;
            for (size_t v = 0; v < n; ++v) {
                if (!used[v] && (u == SIZE_MAX || d[v] < d[u])) {
                    u = v;
                }
            }
            if (d[u] == COST_INF) {
                break;
            }
            used[u] = true;
            for (edge &e : g[u]) {
                if (!used[e.to] && e.cap > e.flow && d[e.to] > d[u] + e.cost + phi[u] - phi[e.to]) {
                    d[e.to] = d[u] + e.cost + phi[u] - phi[e.to];
                }
            }
        }
        for (size_t u = 0; u < n; ++u) {
            phi[u] += d[u];
        }
    }

    int64_t flow_dinic() {
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
        flow_dinic();
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

    int64_t get_flow_cost() {
        int64_t val = 0;
        for (auto &g_u : g) {
            for (edge &e : g_u) {
                if (e.flow > 0) {
                    val += e.flow * e.cost;
                }
            }
        }
        return val;
    }

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
            edge &e = g[u][useful_id[u]];
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

    const vector<vector<edge>> &get_result() {
        return g;
    }

 private:
    size_t n, m, s, t;
    vector<vector<edge>> g;

#undef REV
};

using edge = solver::edge;

size_t n, k;
size_t s;
size_t t;
size_t s_;
size_t t_;
vector<vector<edge>> g;

void add_edge(size_t u, size_t v, int64_t cap, int64_t cost) {
    edge e = {v, cap, 0, cost, g[v].size(), 0};
    edge rev = {u, 0, 0, -cost, g[u].size(), 0};
    g[u].push_back(e);
    g[v].push_back(rev);
}

void read() {
    cin >> n >> k;
    vector<tuple<size_t, size_t, int64_t>> tasks;
    for (size_t i = 0; i < n; ++i) {
        size_t si, ti;
        int64_t cost;
        cin >> si >> ti >> cost;
        tasks.emplace_back(si, si + ti - 1, cost);
    }
    s = 2 * n;
    t = 2 * n + 1;
    s_ = 2 * n + 2;
    t_ = 2 * n + 3;
    g.assign(2 * n + 4, {});
    for (size_t u = 0; u < n; ++u) {
        add_edge(u, u + n, 1, -get<2>(tasks[u]));
    }
    for (size_t u = 0; u < n; ++u) {
        for (size_t v = 0; v < n; ++v) {
            if (get<1>(tasks[u]) < get<0>(tasks[v])) {
                add_edge(u + n, v, 1, 0);
            }
        }
    }
    add_edge(s_, s, k, 0);
    add_edge(t, t_, k, 0);
    for (size_t u = 0; u < n; ++u) {
        add_edge(s, u, 1, 0);
    }
    for (size_t u = n; u < 2 * n; ++u) {
        add_edge(u, t, 1, 0);
    }
}

void solve() {
    solver slvr(g, s_, t_);
    slvr.flow_min_cost();
    for (size_t u = 0; u < n; ++u) {
        bool flag = false;
        for (const edge &e : slvr.get_result()[u]) {
            flag |= (e.flow == 1);
        }
        cout << (flag ? 1 : 0) << ' ';
    }
}

int main() {
    fastio;
    read();
    solve();
    return 0;
}