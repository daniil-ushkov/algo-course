#include <bits/stdc++.h>

using namespace std;

struct action {
    int type;
    int i;
    int j;
};

struct solver {
    solver(int n, int m, vector<vector<bool>> &&field)
            : n(n),
              m(m),
              k(n + m - 1),
              field(field) {
        build();
        kuhn();
        vertex_cover();
        for (int u : lcover) {
            if (u < m) {
                actions.push_back({1, 0, u});
            } else {
                actions.push_back({1, u - m + 1, m - 1});
            }
        }
        for (int u : rcover) {
            if (u < m) {
                actions.push_back({2, 0, m - 1 - u});
            } else {
                actions.push_back({2, u - m + 1, 0});
            }
        }
    }

    void build() {
        l.assign(k, {});
        r.assign(k, -1);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                if (!field[i][j]) {
                    int u = i + j;
                    int v = i - j + m - 1;
                    l[u].push_back(v);
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
        for (int u = 0; u < k; ++u) {
            used.assign(k, false);
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

    vector<int> lcover, rcover;

    void vertex_cover() {
        vector<bool> matched(k, false);
        for (int u = 0; u < k; ++u) {
            if (r[u] != -1) {
                matched[r[u]] = true;
            }
        }
        lused.assign(k, false);
        rused.assign(k, false);
        for (int u = 0; u < k; ++u) {
            if (!matched[u]) {
                dfs2(u);
            }
        }
        for (int u = 0; u < k; ++u) {
            if (!lused[u]) {
                lcover.push_back(u);
            }
        }
        for (int u = 0; u < k; ++u) {
            if (rused[u]) {
                rcover.push_back(u);
            }
        }
    }

    int n, m, k;
    vector<vector<bool>> field;
    vector<vector<int>> l;
    vector<int> r;
    vector<action> actions;
};

int n, m;
vector<string> board;

void read() {
    cin >> n >> m;
    for (int i = 0; i < n; ++i) {
        string line;
        cin >> line;
        board.push_back(line);
    }
}

vector<vector<bool>> convert(bool invert) {
    vector<vector<bool>> res(n, vector<bool>(m, false));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            res[i][j] = ((i + j) % 2 == 0 ? board[i][j] == 'W' : board[i][j] == 'B');
            if (invert) {
                res[i][j] = !res[i][j];
            }
        }
    }
    return res;
}

void solve() {
    solver s1(n, m, convert(false));
    solver s2(n, m, convert(true));
    if (s1.actions.size() <= s2.actions.size()) {
        cout << s1.actions.size() << '\n';
        for (action a : s1.actions) {
            cout << a.type << " "
            << a.i + 1 << " "
            << a.j + 1 << " "
            << ((a.i + a.j) % 2 == 0 ? 'W' : 'B') << '\n';
        }
    } else {
        cout << s2.actions.size() << '\n';
        for (action a : s2.actions) {
            cout << a.type << " "
            << a.i + 1 << " "
            << a.j + 1 << " "
            << ((a.i + a.j) % 2 == 1 ? 'W' : 'B') << '\n';
        }
    }
}

int main() {
    read();
    solve();
    return 0;
}
