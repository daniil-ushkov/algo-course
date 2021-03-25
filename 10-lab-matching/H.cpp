#include <bits/stdc++.h>

using namespace std;

struct edge {
    int id;
    int from;
    int to;
};

int n, m, m_;
vector<vector<edge>> g;

void build() {
    cin >> n >> m;
    g.assign(n, {});
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        u--;
        v--;
        g[u].push_back({i, u, v});
        g[v].push_back({i, v, u});
    }
    vector<int> odd;
    for (int u = 0; u < n; ++u) {
        if (g[u].size() % 2 == 1) {
            odd.push_back(u);
        }
    }
    m_ = m;
    for (int i = 1; i < (int) odd.size(); i += 2) {
        int u = odd[i - 1];
        int v = odd[i];
        g[u].push_back({m_, u, v});
        g[v].push_back({m_++, v, u});
    }
}

vector<bool> used;
vector<edge> euler_path;

void euler(edge from) {
    int u = from.to;
    while (!g[u].empty()) {
        edge e = g[u].back();
        g[u].pop_back();
        if (!used[e.id]) {
            used[e.id] = true;
            euler(e);
        }
    }
    euler_path.push_back(from);
}

int main() {
    read();
    vector<vector<int>> paths;
    used.assign(m_, false);
    for (int u = 0; u < n; ++u) {
        euler_path.clear();
        euler({-1, -1, u});
        euler_path.pop_back();
        reverse(euler_path.begin(), euler_path.end());
        if (euler_path.empty()) {
            continue;
        }
        int first = paths.size();
        vector<int> path;
        path.push_back(euler_path[0].from);
        for (edge e : euler_path) {
            if (e.id < m) {
                path.push_back(e.to);
            } else {
                paths.push_back(path);
                path = {e.to};
            }
        }
        if (!path.empty()) {
            if (first == (int) paths.size()) {
                paths.push_back(path);
            } else {
                path.insert(path.end(), paths[first].begin() + 1, paths[first].end());
                paths[first] = path;
            }
        }
    }
    cout << paths.size() << '\n';
    for (auto &p : paths) {
        for (int u : p) {
            cout << u + 1 << ' ';
        }
        cout << '\n';
    }
    return 0;
}
