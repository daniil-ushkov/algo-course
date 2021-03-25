#include <bits/stdc++.h>

using namespace std;

void z_function(const std::string &s, std::vector<size_t> &z) {
    size_t l = 0;
    size_t r = 0;
    for (size_t i = 1; i < s.size(); ++i) {
        if (i < r) {
            z[i] = std::max(z[i], std::min(z[i - l], r - i));
        }
        while (i + z[i] < s.size() && s[z[i]] == s[i + z[i]]) {
            ++z[i];
        }
        if (i + z[i] > r) {
            l = i;
            r = i + z[i];
        }
    }
}

int main() {
    string p;
    string t;
    cin >> p >> t;
    string s = p + '$' + t;
    vector<size_t> z(s.size(), 0);
    z_function(s, z);
    for (int i = 1; i < (int) s.size(); ++i) {
        if (i + z[i] < s.size()) {
            ++z[i];
        }
    }
    z_function(s, z);
    vector<int> res;
    for (int i = 0; i < (int) t.size(); ++i) {
        if (z[p.size() + 1 + i] >= p.size()) {
            res.push_back(i + 1);
        }
    }
    cout << res.size() << '\n';
    for (int pos : res) {
        cout << pos << ' ';
    }
    return 0;
}
