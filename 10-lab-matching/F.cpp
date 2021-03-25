#include <bits/stdc++.h>

using namespace std;

struct edge {
    bool rev;
    int to;
};

int n;
vector<vector<int>> l;
vector<int> l_, r;
vector<vector<edge>> g;

void read() {
    cin >> n;
    l.assign(n, {});
    for (int u = 0; u < n; ++u) {
        int k;
        cin >> k;
        for (int i = 0; i < k; ++i) {
            int v;
            cin >> v;
            l[u].push_back(v - 1);
        }
    }
    l_.assign(n, -1);
    r.assign(n, -1);
    g.assign(2 * n, {});
    for (int u = 0; u < n; ++u) {
        int v;
        cin >> v;
        v--;
        l_[u] = v;
        r[v] = u;
        g[n + v].push_back({false, u});
        g[u].push_back({true, n + v});
    }
    for (int u = 0; u < n; ++u) {
        for (int v : l[u]) {
            if (r[v] != u) {
                g[u].push_back({false, n + v});
                g[n + v].push_back({true, u});
            }
        }
    }
}

vector<bool> used;
vector<int> sorted;

void dfs(int u) {
    used[u] = true;
    for (edge e : g[u]) {
        if (!e.rev && !used[e.to]) {
            dfs(e.to);
        }
    }
    sorted.push_back(u);
}

int c = 0;
vector<int> color;

void dfs_rev(int u) {
    color[u] = c;
    for (edge e : g[u]) {
        if (e.rev && color[e.to] == -1) {
            dfs_rev(e.to);
        }
    }
}

void scc() {
    used.assign(2 * n, false);
    for (int u = 0; u < 2 * n; ++u) {
        if (!used[u]) {
            dfs(u);
        }
    }
    reverse(sorted.begin(), sorted.end());
    color.assign(2 * n, -1);
    for (int u : sorted) {
        if (color[u] == -1) {
            dfs_rev(u);
            c++;
        }
    }
}

void write() {
    vector<vector<int>> ans(n);
    for (int u = 0; u < n; ++u) {
        for (edge e : g[u]) {
            if (e.rev) {
                ans[u].push_back(e.to - n);
            } else {
                int v = l_[u];
                int w = r[e.to - n];
                if (color[n + v] == color[w]) {
                    ans[u].push_back(e.to - n);
                }
            }
        }
        sort(ans[u].begin(), ans[u].end());
    }
    for (auto &vec : ans) {
        cout << vec.size() << " ";
        for (int v : vec) {
            cout << v + 1 << " ";
        }
        cout << "\n";
    }
}

int main() {
    read();
    scc();
    write();
    return 0;
}
