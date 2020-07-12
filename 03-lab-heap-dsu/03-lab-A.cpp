#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

int main() {
    int n;
    cin >> n;
    int* heap = new int[n + 1];
    for (int i = 1; i <= n; i++) {
        cin >> heap[i];
        if (i != 1 && heap[i / 2] > heap[i]) {
            cout << "NO";
            return 0;
        }
    }
    cout << "YES";
    return 0;
}
