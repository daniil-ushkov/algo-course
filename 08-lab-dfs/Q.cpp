#include <bits/stdc++.h>

using namespace std;

struct edge {
    int to;
    bool verdict;
    int id;
};

int n, m;
vector<vector<edge>> g;

vector<bool> used;
vector<bool> verdict;

bool correct = true;
void dfs(int u, int a) {
    for (edge e : g[u]) {
        if (e.id < a) {
            if (!used[e.to]) {
                used[e.to] = true;
                verdict[e.to] = verdict[u] ? e.verdict : !e.verdict;
                dfs(e.to, a);
            } else {
                correct &= verdict[e.to] == verdict[u] ? e.verdict : !e.verdict;
            }
        }
    }
}

bool check(int a) {
    vector<bool> saved_used(n, false);
    vector<bool> saved_verdict(n, false);
    for (int u = 0; u < n; ++u) {
        if (!saved_used[u]) {
            used = saved_used;
            verdict = saved_verdict;
            correct = true;
            used[u] = true;
            verdict[u] = false;
            dfs(u, a);
            if (correct) {
                saved_used = used;
                saved_verdict = verdict;
                continue;
            }
            used = saved_used;
            verdict = saved_verdict;
            correct = true;
            used[u] = true;
            verdict[u] = true;
            dfs(u, a);
            if (correct) {
                saved_used = used;
                saved_verdict = verdict;
                continue;
            }
            return false;
        }
    }
    return true;
}

int main() {
    cin >> n >> m;
    g.assign(n, {});
    for (int i = 0; i < m; ++i) {
        int x, y;
        string v;
        cin >> x >> y >> v;
        x--;
        y--;
        g[x].push_back({y, v == "T", i});
        g[y].push_back({x, v == "T", i});
    }
    int l = 0, r = m + 1;
    while (r - l != 1) {
        int mid = l + (r - l) / 2;
        if (check(mid)) {
            l = mid;
        } else {
            r = mid;
        }
    }
    cout << l;
    return 0;
}
