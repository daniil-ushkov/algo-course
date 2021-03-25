#include <bits/stdc++.h>

#pragma GCC optimize("O3")

using namespace std;

typedef long long ll;

const ll INF = 1e18;

struct edge {
    int from;
    int to;
    ll w;
};


struct graph {
    graph(int n, vector<edge> &edges) : n(n), edges(edges) {}

    ll get_arb() {
        ll w = 0;
        vector<ll> min_edge(n, INF);
        for (edge &e : edges) {
            min_edge[e.to] = min(min_edge[e.to], e.w);
        }
        for (edge &e : edges) {
            e.w -= min_edge[e.to];
        }
        vector<edge> incoming(n);
        for (edge &e : edges) {
            if (e.w == 0) {
                incoming[e.to] = e;
            }
        }
        for (int u = 1; u < n; ++u) {
            w += min_edge[u];
        }
        vector<ll> used(n, 0);
        used[0] = -1;
        for (int i = 1; i < n; ++i) {
            if (!used[i]) {
                int u = i;
                while (!used[u]) {
                    used[u] = i;
                    u = incoming[u].from;
                }
                if (used[u] != i) {
                    continue;
                }
                vector<bool> cycle(n, false);
                cycle[u] = true;
                for (int v = incoming[u].from; v != u; v = incoming[v].from) {
                    cycle[v] = true;
                }
                int c = 0;
                vector<int> color(n);
                for (int v = 0; v < n; ++v) {
                    if (!cycle[v]) {
                        color[v] = c++;
                    }
                }
                for (int v = 0; v < n; ++v) {
                    if (cycle[v]) {
                        color[v] = c;
                    }
                }
                vector<edge> new_edges;
                for (edge &e : edges) {
                    if (color[e.from] != color[e.to]) {
                        new_edges.push_back({color[e.from], color[e.to], e.w});
                    }
                }
                graph new_g(c + 1, new_edges);
                return w + new_g.get_arb();
            }
        }
        return w;
    }

    int n;
    vector<edge> edges;
};

void dfs(vector<vector<edge>> &g, vector<bool> &used, int u) {
    used[u] = true;
    for (edge e : g[u]) {
        if (!used[e.to]) {
            dfs(g, used, e.to);
        }
    }
}

bool linked(int n, vector<edge> &edges) {
    vector<vector<edge>> g(n);
    for (edge e : edges) {
        g[e.from].push_back(e);
    }
    vector<bool> used(n, false);
    dfs(g, used, 0);
    return all_of(used.begin(), used.end(), [](bool bit) { return bit; });
}

int main() {
    int n, m;
    cin >> n >> m;
    vector<edge> edges;
    for (int i = 0; i < m; ++i) {
        int from, to;
        ll w;
        cin >> from >> to >> w;
        --from;
        --to;
        if (to != 0) {
            edges.push_back({from, to, w});
        }
    }
    if (!linked(n, edges)) {
        cout << "NO\n";
    } else {
        graph g(n, edges);
        cout << "YES\n" << g.get_arb();
    }
    return 0;
}
