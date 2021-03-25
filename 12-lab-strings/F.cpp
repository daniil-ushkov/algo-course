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
    auto p1 = prefix_function(s);
    int l = (int) s.size() - p1[s.size() - 1];
    string period = s.substr(0, l);
    string s1 = period + '$' + s;
    auto p2 = prefix_function(s1);
    int num = count_if(p2.begin(), p2.end(), [l](int len){ return len == l; });
    if (num * l == (int) s.size()) {
        cout << l;
    } else {
        cout << s.size();
    }
    return 0;
}
