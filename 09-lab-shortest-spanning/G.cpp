#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

const double INF = 1e18;

int *x, *y;

double graph(int i, int j) {
    return sqrt((x[i] - x[j]) * (x[i] - x[j]) + (y[i] - y[j]) * (y[i] - y[j]));
}

int main() {
    int n;
    cin >> n;

    x = new int[n + 1];
    y = new int[n + 1];

    for (int i = 1; i <= n; i++) {
        cin >> x[i] >> y[i];
    }

    double w = 0;
    vector<bool> used(n + 1, false);
    vector<double> min_edge(n + 1, INF);
    min_edge[1] = 0;
    for (int i = 1; i <= n; i++) {
        int v = -1;
        for (int j = 1; j <= n; j++) {
            if (!used[j] && (v == -1 || min_edge[j] < min_edge[v]))
                v = j;
        }
        used[v] = true;
        w += min_edge[v];

        for (int j = 1; j <= n; j++) {
            if (min_edge[j] > graph(v, j))
                min_edge[j] = graph(v, j);
        }
    }
    cout.precision(30);
    cout << w;
}
