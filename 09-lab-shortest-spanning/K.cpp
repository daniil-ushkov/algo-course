#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

const ll INF = 1e18;

struct edge {
    int from;
    int to;
    ll w;
};

struct dsu {
    dsu(int n)
            : p(n),
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

    vector<int> p;
    vector<int> r;
};

struct node {
    node(edge e) : d(1), e(e), delta(0), l(nullptr), r(nullptr) {}

    static node *merge(node *h1, node *h2) {
        if (h1 == h2) {
            return h1;
        }
        push(h1);
        push(h2);
        if (h1 == nullptr) {
            return h2;
        }
        if (h2 == nullptr) {
            return h1;
        }
        if (h1->e.w > h2->e.w) {
            swap(h1, h2);
        }
        h1->r = merge(h1->r, h2);
        if ((h1->l ? h1->l->d : 0) < (h1->r ? h1->r->d : 0)) {
            swap(h1->l, h1->r);
        }
        h1->d = (h1->r ? h1->r->d : 0) + 1;
        return h1;
    }

    static void add(node *t, ll delta) {
        if (t == nullptr) {
            return;
        }
        t->delta += delta;
    }

    static void push(node *t) {
        if (t == nullptr) {
            return;
        }
        t->e.w += t->delta;
        add(t->l, t->delta);
        add(t->r, t->delta);
        t->delta = 0;
    }

    static edge min(node *h) {
        push(h);
        return h->e;
    }

    static node *extract_min(node *t) {
        push(t);
        if (t == nullptr) {
            return nullptr;
        }
        return merge(t->l, t->r);
    }

    static node *insert(node *t, edge e) {
        push(t);
        return merge(t, new node(e));
    }

    int d;
    edge e;
    ll delta;
    node *l;
    node *r;
};

struct graph {
    graph(int n, vector<node *> &g_rev) : n(n), g_rev(g_rev), d(n) {}

    ll get_arb() {
        ll weight = 0;
        vector<edge> incoming(n);
        for (int v = 1; v < n; ++v) {
            edge e = node::min(g_rev[v]);
            g_rev[v] = node::extract_min(g_rev[v]);
            incoming[v] = e;
            weight += e.w;
            node::add(g_rev[v], -e.w);
        }
        vector<int> used(n, 0);
        used[0] = -1;
        for (int v = 1; v < n; ++v) {
            int u = d.find(v);
            if (used[u]) {
                continue;
            }
            while (true) {
                if (used[u] && used[u] != v) {
                    break;
                }
                if (used[u] && used[u] == v) {
                    vector<int> cycle;
                    cycle.push_back(u);
                    for (int w = d.find(incoming[u].from); w != u; w = d.find(incoming[w].from)) {
                        cycle.push_back(w);
                    }
                    for (int w : cycle) {
                        d.join(w, u);
                    }
                    u = d.find(u);
                    for (int w : cycle) {
                        if (w != u) {
                            g_rev[u] = node::merge(g_rev[w], g_rev[u]);
                        }
                    }
                    edge e = node::min(g_rev[u]);
                    g_rev[u] = node::extract_min(g_rev[u]);
                    incoming[u] = e;
                    weight += e.w;
                    node::add(g_rev[u], -e.w);
                    u = d.find(incoming[u].from);
                    continue;
                }
                used[u] = v;
                u = d.find(incoming[u].from);
            }
        }
        return weight;
    }

    int n;
    vector<node *> g_rev;
    dsu d;
};

void dfs(vector<vector<edge>> &g, vector<bool> &used, int u) {
    used[u] = true;
    for (edge e : g[u]) {
        if (!used[e.to]) {
            dfs(g, used, e.to);
        }
    }
}

bool linked(int n, vector<vector<edge>> &g) {
    vector<bool> used(n, false);
    dfs(g, used, 0);
    return all_of(used.begin(), used.end(), [](bool bit) { return bit; });
}

int main() {
    int n, m;
    cin >> n >> m;
    vector<vector<edge>> g(n);
    vector<node *> g_rev(n, nullptr);
    for (int i = 0; i < m; ++i) {
        int from, to;
        ll w;
        cin >> from >> to >> w;
        --from;
        --to;
        g[from].push_back({from, to, w});
        g_rev[to] = node::insert(g_rev[to], {from, to, w});
    }
    if (!linked(n, g)) {
        cout << "NO\n";
    } else {
        graph gr(n, g_rev);
        cout << "YES\n" << gr.get_arb();
    }
    return 0;
}
