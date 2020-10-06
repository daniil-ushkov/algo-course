#include <bits/stdc++.h>

using namespace std;

struct edge {
    int to;
    int w;
};

int n;
vector<vector<edge>> g, g_rev;
vector<bool> used;
vector<int> sorted;

void dfs(int u, int w) {
    used[u] = true;
    for (edge e : g[u]) {
        if (!used[e.to] && e.w <= w) {
            dfs(e.to, w);
        }
    }
    sorted.push_back(u);
}

void dfs_rev(int u, int w) {
    used[u] = true;
    for (edge e : g_rev[u]) {
        if (!used[e.to] && e.w <= w) {
            dfs_rev(e.to, w);
        }
    }
}

bool check(int w) {
    sorted.clear();
    used.assign(n, false);
    dfs(0, w);
    if (sorted.size() != (size_t) n) {
        return false;
    }
    used.assign(n, false);
    dfs_rev(sorted.back(), w);
    return all_of(used.begin(), used.end(), [](bool el) { return el; });
}

int main() {
    cin >> n;
    g.assign(n, {});
    g_rev.assign(n, {});
    int mx = 0;
    for (int u = 0; u < n; ++u) {
        for (int v = 0; v < n; ++v) {
            int w;
            cin >> w;
            mx = max(mx, w);
            if (u != v) {
                g[u].push_back({v, w});
                g_rev[v].push_back({u, w});
            }
        }
    }
    int l = -1;
    int r = mx;
    while (r - l != 1) {
        int m = l + (r - l) / 2;
        if (check(m)) {
            r = m;
        } else {
            l = m;
        }
    }
    cout << r;
    return 0;
}
