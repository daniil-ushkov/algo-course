#include <bits/stdc++.h>

using namespace std;

int main() {
    int n, l;
    cin >> n >> l;
    vector<int> tour;
    vector<int> last(n);
    for (int i = 0; i < l; ++i) {
        int u;
        cin >> u;
        tour.push_back(--u);
        last[u] = i;
    }
    vector<int> color(n);
    vector<pair<int, int>> edges;
    vector<int> last_edge(n);
    for (int i = 0; i < l; ++i) {
        int v = tour[i];
        if (color[v] == 1) {
            if (last[v] == i) {
                color[v] = 2;
            } else {
                last_edge[v] = tour[i + 1];
            }
            continue;
        }
        for (int w = 0; w < n; ++w) {
            if (color[w] == 1 && last_edge[w] <= v) {
                edges.emplace_back(v, w);
            }
        }
        if (last[v] == i) {
            color[v] = 2;
        } else {
            color[v] = 1;
            last_edge[v] = tour[i + 1];
        }
    }
    cout << edges.size() << "\n";
    for (auto e : edges) {
        cout << e.first + 1 << " " << e.second + 1 << "\n";
    }
    return 0;
}
