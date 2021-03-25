#include <bits/stdc++.h>

using namespace std;

std::vector<size_t> z_function(const std::string &s) {
    std::vector<size_t> z(s.size(), 0);
    if (s.empty()) {
        return z;
    }
    z[0] = SIZE_MAX;
    size_t l = 0;
    size_t r = 0;
    for (size_t i = 1; i < s.size(); ++i) {
        z[i] = 0;
        if (i < r) {
            z[i] = std::min(z[i - l], r - i);
        }
        while (i < s.size() && s[z[i]] == s[i + z[i]]) {
            ++z[i];
        }
        if (i + z[i] > r) {
            l = i;
            r = i + z[i];
        }
    }
    return z;
}

int main() {
    string s;
    cin >> s;
    auto z = z_function(s);
    for (int i = 1; i < (int) z.size(); ++i) {
        cout << z[i] << ' ';
    }
    return 0;
}
