#include <iostream>
#include <vector>

using namespace std;

int n;
bool *a;
vector<pair<int, int>> net;

void sortNet(int except) {
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (i != except && j != except) {
                net.push_back({i, j});
            }
        }
    }
}

void createNet() {
    net.clear();
    int first = 0;
    int k = 0;
    for (int i = n - 1; i >= 0; i--) {
        if (a[i]) {
            first = i;
            k++;
        }
    }
    for (int i = first + 1; i < n; i++) {
        if (a[i]) {
            net.push_back({first, i});
        }
    }
    sortNet(first);
    for (int i = first; i > 0; i--) {
        net.push_back({i - 1, i});
    }
    for (int i = first; i < n - k - 1; i++) {
        net.push_back({i, i + 1});
    }
}

int main() {
    while (1) {
        cin >> n;
        if (n == 0) {
            break;
        }
        a = new bool[n];
        for (int i = 0; i < n; i++) {
            cin >> a[i];
        }
        bool sorted = 1;
        for (int i = 1; i < n; i++) {
            sorted = sorted && (a[i - 1] <= a[i]);
        }
        if (sorted) {
            cout << -1 << "\n";
            continue;
        }
        createNet();
        cout << net.size() << "\n";
        for (auto c : net) {
            cout << c.first + 1 << " " << c.second + 1 << "\n";
        }
    }
    return 0;
}
