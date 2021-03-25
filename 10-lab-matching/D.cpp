#include <bits/stdc++.h>

using namespace std;

int m, n;
vector<vector<int>> l;
vector<int> r;

void build() {
    l.assign(m, {});
    r.assign(n, -1);
    for (int u = 0; u < m; ++u) {
        vector<bool> exists(n, false);
        int v;
        cin >> v;
        while (v != 0) {
            exists[v - 1] = true;
            cin >> v;
        }
        for (int w = 0; w < n; ++w) {
            if (!exists[w]) {
                l[u].push_back(w);
            }
        }
    }
}

vector<bool> used;

bool dfs1(int u) {
    if (used[u]) {
        return false;
    }
    used[u] = true;
    for (int v : l[u]) {
        if (r[v] == -1 || dfs1(r[v])) {
            r[v] = u;
            return true;
        }
    }
    return false;
}

void kuhn() {
    for (int u = 0; u < m; ++u) {
        used.assign(m, false);
        dfs1(u);
    }
}

vector<bool> lused, rused;

void dfs2(int u) {
    if (lused[u]) {
        return;
    }
    lused[u] = true;
    for (int v : l[u]) {
        rused[v] = true;
        if (r[v] != -1) {
            dfs2(r[v]);
        }
    }
}

pair<vector<int>, vector<int>> bipartiteClique() {
    vector<bool> matched(m, false);
    for (int u = 0; u < n; ++u) {
        if (r[u] != -1) {
            matched[r[u]] = true;
        }
    }
    lused.assign(m, false);
    rused.assign(n, false);
    for (int u = 0; u < m; ++u) {
        if (!matched[u]) {
            dfs2(u);
        }
    }
    vector<int> lclique, rclique;
    for (int u = 0; u < m; ++u) {
        if (lused[u]) {
            lclique.push_back(u);
        }
    }
    for (int u = 0; u < n; ++u) {
        if (!rused[u]) {
            rclique.push_back(u);
        }
    }
    return {lclique, rclique};
}

int main() {
    int k;
    cin >> k;
    while (k--) {
        cin >> m >> n;
        read();
        kuhn();
        auto ans = bipartiteClique();
        cout << ans.first.size() + ans.second.size() << "\n";
        cout << ans.first.size() << " " << ans.second.size() << "\n";
        for (int u : ans.first) {
            cout << u + 1 << " ";
        }
        cout << "\n";
        for (int u : ans.second) {
            cout << u + 1 << " ";
        }
        cout << "\n";
    }
    return 0;
}
