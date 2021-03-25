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

string s;
vector<vector<size_t>> cnt;

void read() {
    cin >> s;
    cnt.assign(s.size(), vector<size_t>(s.size(), 0));
    for (size_t i = 0; i < s.size(); ++i) {
        string pref_s = s.substr(0, i + 1);
        reverse(pref_s.begin(), pref_s.end());
        auto p = prefix_function(pref_s);
        size_t max_ = 0;
        for (size_t j = 0; j < pref_s.size(); ++j) {
            max_ = max(max_, p[j]);
            if (j == 0) {
                cnt[i][i] = 1;
            } else {
                cnt[i - j][i] = cnt[i - j][i - 1] + (j + 1 - max_);
            }
        }
    }
}

void solve() {
    size_t n;
    cin >> n;
    while (n--) {
        size_t l, r;
        cin >> l >> r;
        cout << cnt[l - 1][r - 1] << '\n';
    }
}

int main() {
    read();
    solve();
    return 0;
}
