#include <iostream>
#include <vector>


using namespace std;

int n;
int* p;
int* r;
int* number;
int* minValue;
int* maxValue;


int find_leader(int x) {
    if (p[x] == x) {
        return x;
    }
    return p[x] = find_leader(p[x]);
}

void merge(int a, int b) {
    a = find_leader(a);
    b = find_leader(b);
    if (a == b) {
        return;
    }
    if (r[a] > r[b]) {
        swap(a, b);
    }
    p[a] = b;
    number[b] += number[a];
    minValue[b] = min(minValue[b], minValue[a]);
    maxValue[b] = max(maxValue[b], maxValue[a]);
    if (r[a] == r[b]) {
        r[b]++;
    }
}

int main() {
    int n;
    cin >> n;
    p = new int[n];
    r = new int[n];
    number = new int[n];
    minValue = new int[n];
    maxValue = new int[n];
    for (int i = 0; i < n; i++) {
        p[i] = i;
        r[i] = 0;
        number[i] = 1;
        minValue[i] = i;
        maxValue[i] = i;
    }
    string command;
    while (cin >> command) {
        if (command == "union") {
            int a, b;
            cin >> a >> b;
            merge(a - 1, b - 1);
        }
        if (command == "get") {
            int i;
            cin >> i;
            cout << minValue[find_leader(i - 1)] + 1 << " "
            << maxValue[find_leader(i - 1)] + 1 << " "
            << number[find_leader(i - 1)] << "\n";
        }
    }
    return 0;
}
