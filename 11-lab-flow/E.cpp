#include <vector>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <queue>
#include <algorithm>

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
        size_t edge_id;
    };

#define REV(e) g[(e).to][(e).rev_id]

    solver(vector<vector<edge>> &g, size_t s, size_t t)
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

    size_t n, s, t;
    vector<vector<edge>> g;

#undef REV
};

using edge = solver::edge;

size_t n, m;
vector<vector<edge>> g;

void read() {
    cin >> n >> m;
    g.assign(n, {});
    for (size_t i = 0; i < m; ++i) {
        size_t u, v;
        int64_t cap;
        cin >> u >> v >> cap;
        u--, v--;
        edge e = {v, cap, 0l, g[v].size(), i};
        edge rev = {u, 0, 0l, g[u].size(), i};
        g[u].push_back(e);
        g[v].push_back(rev);
    }
}

void solve() {
    solver s(g, 0, n - 1);
    auto res = s.decompose();
    cout << res.size() << '\n';
    for (auto &path : res) {
        cout << path.first << ' ' << path.second.size() << ' ';
        for (int edge_id : path.second) {
            cout << edge_id + 1 << ' ';
        }
        cout << '\n';
    }
}

int main() {
    read();
    solve();
    return 0;
}