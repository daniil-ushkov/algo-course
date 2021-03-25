#include <vector>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <queue>

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
    };

#define REV(e) g[(e).to][(e).rev_id]

    solver(vector<vector<edge>> &g, size_t s, size_t t) : n(g.size()), m(0), s(s), t(t), g(g) {
        for (auto &g_u : g) {
            m += g_u.size();
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

    void flow_min_cost() {
        vector<int64_t> phi;
        ford_bellman_(phi);
        vector<bool> used(n, false);
        while (dfs_flow_min_cost_(s, used, CAP_INF, phi) != 0) {
            dijkstra_(phi);
            used.assign(n, false);
        }
    }

    const vector<vector<edge>> &get_net() {
        return g;
    }

    vector<vector<edge>> get_flow_net() {
        vector<vector<edge>> g_(n);
        for (size_t u = 0; u < n; ++u) {
            for (edge &e : g[u]) {
                if (e.flow > 0) {
                    edge e_ = e;
                    e_.cap = 1;
                    e_.flow = 0;

                    edge rev_e_ = REV(e);

                    rev_e_.flow = 0;

                    e_.rev_id = g_[e_.to].size();
                    rev_e_.rev_id = g_[rev_e_.to].size();

                    g_[rev_e_.to].push_back(e_);
                    g_[e.to].push_back(rev_e_);
                }
            }
        }
        return g_;
    }

    vector<vector<edge>> get_non_flow_net() {
        vector<vector<edge>> g_(n);
        for (size_t u = 0; u < n; ++u) {
            for (edge &e : g[u]) {
                if ((e.flow == 0 || u == s || e.to == t) && e.cap > 0) {
                    edge e_ = e;
                    e_.cap = 1;
                    e_.flow = 0;

                    edge rev_e_ = REV(e);

                    rev_e_.flow = 0;

                    e_.rev_id = g_[e_.to].size();
                    rev_e_.rev_id = g_[rev_e_.to].size();

                    g_[u].push_back(e_);
                    g_[e.to].push_back(rev_e_);
                }
            }
        }
        return g_;
    }

    void clear() {
        for (auto &g_u : g) {
            for (edge &e : g_u) {
                e.flow = 0;
            }
        }
    }

    int64_t get_flow_val() {
        int64_t val = 0;
        for (edge &e : g[s]) {
            if (e.flow > 0) {
                val += e.flow;
            }
        }
        return val;
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

    void ford_bellman_(vector<int64_t> &phi) {
        phi.assign(n, COST_INF);
        phi[s] = 0;
        for (size_t i = 1; i < n; ++i) {
            for (size_t u = 0; u < n; ++u) {
                for (edge &e : g[u]) {
                    if (e.cap > e.flow && phi[u] != COST_INF && phi[e.to] > phi[u] + e.cost) {
                        phi[e.to] = phi[u] + e.cost;
                    }
                }
            }
        }
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

    size_t n, m, s, t;
    vector<vector<edge>> g;

#undef REV
};

using edge = solver::edge;

size_t n;
int64_t k;
vector<vector<edge>> g;

void add_edge(size_t u, size_t v, int64_t cap, int64_t cost) {
    edge e = {v, cap, 0, cost, g[v].size()};
    edge rev = {u, 0, 0, -cost, g[u].size()};
    g[u].push_back(e);
    g[v].push_back(rev);
}

int main() {
    fastio;
    cin >> n >> k;
    g.assign(2 * n + 2, {});
    for (size_t u = 0; u < n; ++u) {
        for (size_t v = n; v < 2 * n; ++v) {
            int64_t cost;
            cin >> cost;
            add_edge(u, v, 1, cost);
        }
    }

    size_t s = 2 * n, t = 2 * n + 1;
    for (size_t u = 0; u < n; ++u) {
        add_edge(s, u, k, 0);
    }
    for (size_t u = n; u < 2 * n; ++u) {
        add_edge(u, t, k, 0);
    }

    solver choose_edges(g, s, t);
    choose_edges.flow_min_cost();

    cout << choose_edges.get_flow_cost() << '\n';

    g = choose_edges.get_flow_net();
    solver matching(g, s, t);
    while (k--) {
        matching.flow_dinic();
        vector<size_t> res(n);
        for (size_t u = 0; u < n; ++u) {
            for (const edge &e : matching.get_net()[u]) {
                if (e.flow == 1) {
                    res[u] = e.to - n + 1;
                }
            }
        }
        for (size_t u : res) {
            cout << u << ' ';
        }
        cout << '\n';
        g = matching.get_non_flow_net();
        matching = solver(g, s, t);
    }
}
