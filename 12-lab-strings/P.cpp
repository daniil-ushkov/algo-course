#include <bits/stdc++.h>

using namespace std;

const uint64_t mod = 1e9 + 7;

uint64_t prefix_function_inv(vector<uint64_t> &pref, uint64_t alphabet_size) {
    vector<uint64_t> cnt(pref.size(), alphabet_size);
    vector<bool> was(pref.size(), false);
    size_t l = pref.size() + 1;
    for (size_t i = pref.size(); i >= 1; --i) {
        if (pref[i - 1] != 0) {
            l = min(l, i - static_cast<size_t>(pref[i - 1]));
        }
        if (l <= i - 1) {
            cnt[i - 1] = 1ull;
            was[i - 1] = true;
        }
    }
    for (size_t i = 1; i < pref.size(); ++i) {
        if (!was[i] && pref[i] == 0) {
            uint64_t k = pref[i - 1];
            while (k != 0) {
                k = pref[static_cast<size_t>(k - 1ull)];
                cnt[i] = ((cnt[i] != 0ull) ? cnt[i] - 1ull : 0ull);
            }
            cnt[i] = ((cnt[i] != 0ull) ? cnt[i] - 1ull : 0ull);
        }
    }
    uint64_t ans = 1;
    for (uint64_t val : cnt) {
        ans = (ans * val) % mod;
    }
    return ans;
}

int main() {
    size_t n;
    size_t c;
    cin >> n >> c;
    vector<uint64_t> pref(n, 0ull);
    for (size_t i = 1; i < n; ++i) {
        cin >> pref[i];
    }
    cout << prefix_function_inv(pref, c);
    return 0;
}