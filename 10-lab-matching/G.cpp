#include <bits/stdc++.h>

using namespace std;

struct edge {
    int id;
    int from;
    int to;
};

int n;
vector<bool> used;
vector<vector<edge>> g;

vector<int> path;
void euler(int u) {
    while (!g[u].empty()) {
        edge e = g[u].back();
        g[u].pop_back();
        if (!used[e.id]) {
            used[e.id] = true;
            euler(e.to);
        }
    }
    path.push_back(u);
}

int main() {
    cin >> n;
    vector<vector<int>> g1(n);
    int cnt = 0;
    int odd = 0;
    for (int u = 0; u < n; ++u) {
        int deg;
        cin >> deg;
        if (deg % 2 == 1) {
            cnt += deg % 2;
            odd = u;
        }
        for (int i = 0; i < deg; ++i) {
            int v;
            cin >> v;
            g1[u].push_back(v - 1);
        }
        sort(g1[u].begin(), g1[u].end());
    }
    if (cnt > 2) {
        cout << -1;
        return 0;
    }
    g.assign(n, {});
    int id = 0;
    for (int u = n - 1; u >= 0; --u) {
        for (int v : g1[u]) {
            g1[v].pop_back();
            g[u].push_back({id, u, v});
            g[v].push_back({id++, v, u});
        }
    }
    used.assign(id, false);
    euler(odd);
    cout << path.size() - 1 << "\n";
    reverse(path.begin(), path.end());
    for (int u : path) {
        cout << u + 1 << " ";
    }
    return 0;
}
