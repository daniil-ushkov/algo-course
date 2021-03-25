#include <bits/stdc++.h>

using namespace std;

int n, m;
vector<vector<int>> g;

void read() {
    cin >> n >> m;
    g.assign(n, {});
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        g[u - 1].push_back(v - 1);
        g[v - 1].push_back(u - 1);
    }
}

vector<int> d;
void bfs() {
    vector<bool> used(n, false);
    d.assign(n, 0);

    used[0] = true;
    queue<int> q;
    q.push(0);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : g[u]) {
            if (!used[v]) {
                used[v] = true;
                d[v] = d[u] + 1;
                q.push(v);
            }
        }
    }
}

void write() {
    for (int val : d) {
        cout << val << " ";
    }
}

int main() {
    read();
    bfs();
    write();
    return 0;
}
