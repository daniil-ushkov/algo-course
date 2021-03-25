#include <bits/stdc++.h>

#pragma GCC optimize("O3")

using namespace std;

const int INF = 0x3F3F3F3F;

struct edge {
    int from;
    int to;
    int w;
};

int n;
vector<vector<int>> d;
vector<edge> edges;
void read() {
//    ios_base::sync_with_stdio(false), cin.tie(0), cout.tie(0);
    cin >> n;
    d.assign(n, vector<int>(n));
    for (int u = 0; u < n; ++u) {
        for (int v = 0; v < n; ++v) {
            cin >> d[u][v];
            if (d[u][v] != 100000) {
                edges.push_back({u, v, d[u][v]});
            } else {
                d[u][v] = 0x3F3F3F3F;
            }
        }
    }
}

int floyd() {
    for (int w = 0; w < n; ++w) {
        for (int u = 0; u < n; ++u) {
            for (int v = 0; v < n; ++v) {
                if (d[u][w] != INF && d[w][v] != INF && d[u][v] > d[u][w] + d[w][v]) {
                    d[u][v] = d[u][w] + d[w][v];
                }
            }
        }
    }
    for (int u = 0; u < n; ++u) {
        if (d[u][u] < 0) {
            return u;
        }
    }
    return -1;
}

vector<int> d1;
vector<int> p;
vector<int> cycle;
void find_cycle(int u) {
    for (int i = 0; i < n; ++i) {
        u = p[u];
    }
    int v = u;
    int cur = p[v];
    while(cur != v)
    {
        cycle.push_back(cur);
        cur = p[cur];
    }
    cycle.push_back(v);
    reverse(cycle.begin(), cycle.end());
}

void ford_bellman(int start) {
    d1.assign(n, INF);
    d1[start] = 0;
    p.assign(n, -1);
    for (int i = 1; i < n; ++i) {
        for (edge e : edges) {
            if (d1[e.from] < INF && d1[e.to] > d1[e.from] + e.w) {
                d1[e.to] = d1[e.from] + e.w;
                p[e.to] = e.from;
            }
        }
    }
    int u = -1;
    for (edge e : edges) {
        if (d1[e.from] < INF && d1[e.to] > d1[e.from] + e.w) {
            d1[e.to] = d1[e.from] + e.w;
            p[e.to] = e.from;
            u = e.to;
        }
    }
    if (u != -1) {
        find_cycle(u);
    }
}

int main() {
    read();
    int start = floyd();
    if (start == -1) {
        cout << "NO\n";
        return 0;
    }
    ford_bellman(start);
    cout << "YES\n" << cycle.size() << "\n";
    for (int u : cycle) {
        cout << u + 1 << " ";
    }
    return 0;
}
