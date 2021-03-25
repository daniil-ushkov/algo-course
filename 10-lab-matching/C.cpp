#include <bits/stdc++.h>

using namespace std;

int m, n;
vector<vector<int>> l;
vector<int> r;
vector<bool> lused, rused;

void dfs(int u) {
    if (lused[u]) {
        return;
    }
    lused[u] = true;
    for (int v : l[u]) {
        rused[v] = true;
        if (r[v] != -1) {
            dfs(r[v]);
        }
    }
}

int main() {
    cin >> m >> n;
    l.assign(m, {});
    r.assign(n, -1);
    for (int u = 0; u < m; ++u) {
        int cnt;
        cin >> cnt;
        for (int i = 0; i < cnt; ++i) {
            int v;
            cin >> v;
            l[u].push_back(v - 1);
        }
    }
    vector<bool> matched(m, false);
    for (int u = 0; u < m; ++u) {
        int v;
        cin >> v;
        if (v != 0) {
            matched[u] = true;
            r[v - 1] = u;
        }
    }
    lused.assign(m, false);
    rused.assign(n, false);
    for (int u = 0; u < m; ++u) {
        if (!matched[u]) {
            dfs(u);
        }
    }
    vector<int> lcover, rcover;
    for (int u = 0; u < m; ++u) {
        if (!lused[u]) {
            lcover.push_back(u);
        }
    }
    for (int u = 0; u < n; ++u) {
        if (rused[u]) {
            rcover.push_back(u);
        }
    }
    cout << lcover.size() + rcover.size() << "\n" << lcover.size() << " ";
    for (int u : lcover) {
        cout << u + 1 << " ";
    }
    cout << "\n" << rcover.size() << " ";
    for (int u : rcover) {
        cout << u + 1 << " ";
    }
    return 0;
}
