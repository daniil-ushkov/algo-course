#include <bits/stdc++.h>

using namespace std;

using hash_t = uint64_t;

const hash_t p = 53;
const hash_t mod = 1073676287;

vector<hash_t> hash_pref;
vector<hash_t> p_deg;

void calc(const string &s) {
    hash_pref.assign(s.size() + 1, 0);
    p_deg.assign(s.size() + 1, 1);
    for (size_t i = 1; i <= s.size(); ++i) {
        hash_pref[i] = ((hash_pref[i - 1] * p) % mod + (s[i - 1] - 'a' + 1)) % mod;
        p_deg[i] = (p_deg[i - 1] * p) % mod;
    }
}

hash_t get_hash(size_t l, size_t r) {
    if (l == 0) {
        return hash_pref[r];
    }
    return (hash_pref[r] + mod - (hash_pref[l] * p_deg[r - l]) % mod) % mod;
}

int main() {
    string s;
    cin >> s;
    calc(s);
    size_t n;
    cin >> n;
    for (size_t i = 0; i < n; ++i) {
        size_t a, b, c, d;
        cin >> a >> b >> c >> d;
        cout << (get_hash(a - 1, b) == get_hash(c - 1, d) ? "Yes\n" : "No\n");
    }
    return 0;
}
