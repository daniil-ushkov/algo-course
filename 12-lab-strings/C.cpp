#include <bits/stdc++.h>

using namespace std;

vector<size_t> prefix_function(const string &s) {
    vector<size_t> p(s.size(), 0);
    if (s.empty()) {
        return p;
    }
    p[0] = 0;
    for (size_t i = 1; i < s.size(); ++i) {
        size_t k = p[i - 1];
        while (k != 0 && s[k] != s[i]) {
            k = p[k - 1];
        }
        if (s[k] == s[i]) {
            ++k;
        }
        p[i] = k;
    }
    return p;
}

int main() {
    string s;
    cin >> s;
    for (int val : prefix_function(s)) {
        cout << val << ' ';
    }
    return 0;
}
