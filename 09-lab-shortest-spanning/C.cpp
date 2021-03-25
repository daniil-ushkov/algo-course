#include <bits/stdc++.h>
#pragma GCC optimize("O3")

using namespace std;

typedef long long ll;

const ll INF = 1e18;

int n, m;
vector<vector<ll>> d;

void read() {
    ios_base::sync_with_stdio(false), cin.tie(0), cout.tie(0);
    cin >> n >> m;
    d.assign(n, vector<ll>(n, INF));
    for (int u = 0; u < n; ++u) {
        d[u][u] = 0;
    }
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        d[u - 1][v - 1] = (ll) w;
    }
}

void floyd() {
    for (int w = 0; w < n; ++w) {
        for (int u = 0; u < n; ++u) {
            for (int v = 0; v < n; ++v) {
                d[u][v] = min(d[u][v], d[u][w] + d[w][v]);
            }
        }
    }
}

void write() {
    for (int u = 0; u < n; ++u) {
        for (int v = 0; v < n; ++v) {
            cout << d[u][v] << " ";
        }
        cout << "\n";
    }
}

int main() {
    read();
    floyd();
    write();
    return 0;
}
