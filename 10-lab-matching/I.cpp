#include <bits/stdc++.h>

using namespace std;

struct edge {
    int id;
    int to;
};

int d, k, n = 1, id;
vector<vector<edge>> g;

void build() {
    for (int i = 0; i < k - 1; ++i) {
        n *= d;
    }
    g.assign(n, {});
    for (int i = 0; i < n; ++i) {
        int common = i % (n / d);
        for (int r = 0; r < d; ++r) {
            g[i].push_back({id++, d * common + r});
        }
    }
}

vector<bool> used;
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
    cin >> d >> k;
    if (k == 1) {
        for (int i = 0; i < d; ++i) {
            cout << i;
        }
        return 0;
    }
    build();
    used.assign(id, false);
    euler(0);
    reverse(path.begin(), path.end());
    int first = path[0] / d;
    string str;
    for (int i = 0; i < k - 1; ++i) {
        str.push_back((char) ((int) '0' + (first % d)));
        first /= d;
    }
    reverse(str.begin(), str.end());
    cout << str;
    for (int i = 1; i < (int) path.size(); ++i) {
        cout << path[i] % d;
    }
    return 0;
}
