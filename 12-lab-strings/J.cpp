#include "bits/stdc++.h"

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

size_t max_prefix(const string &s) {
    auto p = prefix_function(s);
    size_t m = *std::max_element(p.begin(), p.end());
    return m;
}

int main() {
    string s;
    cin >> s;
    size_t num = 0;
    for (size_t i = 0; i < s.size(); ++i) {
        string sub_s = s.substr(0, i + 1);
        reverse(sub_s.begin(), sub_s.end());
        num += sub_s.size() - max_prefix(sub_s);
        cout << num << '\n';
    }
    return 0;
}
