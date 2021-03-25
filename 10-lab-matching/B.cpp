#include <bits/stdc++.h>

using namespace std;

int n, m;
vector<vector<int>> l;
vector<int> r;
vector<bool> used;

bool dfs(int u) {
    if (used[u]) {
        return false;
    }
    used[u] = true;
    for (int v : l[u]) {
        if (r[v] == -1 || dfs(r[v])) {
            r[v] = u;
            return true;
        }
    }
    return false;
}

int kuhn() {
    int cnt = 0;
    for (int u = 0; u < n; ++u) {
        used.assign(n, false);
        if (dfs(u)) {
            cnt++;
        }
    }
    return cnt;
}

int main() {
    cin >> n >> m;
    l.assign(n, {});
    r.assign(n, -1);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        l[u - 1].push_back(v - 1);
    }
    cout << n - kuhn();
    return 0;
}