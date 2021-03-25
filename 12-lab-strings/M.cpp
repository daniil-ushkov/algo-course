#include <bits/stdc++.h>

using namespace std;

struct big_integer {
    big_integer() : digits(100, 0) {}

    big_integer(uint64_t val) : big_integer() {
        digits[0] = val;
    }

    big_integer(const big_integer &other) = default;

    big_integer &operator=(const big_integer &other) = default;

    big_integer &operator=(big_integer &&other) = default;

    big_integer operator+(const big_integer &other) {
        big_integer res;
        uint64_t carry = 0;
        for (size_t i = 0; i < digits.size(); ++i) {
            uint64_t tmp = digits[i] + other.digits[i] + carry;
            res.digits[i] = tmp % BASE;
            carry = tmp / BASE;
        }
        return res;
    }

    big_integer &operator+=(const big_integer &other) {
        return *this = *this + other;
    }

    string to_string() const {
        string s;
        size_t i = digits.size();
        while (i >= 1 && digits[i - 1] == 0) {
            --i;
        }
        if (i == 0) {
            return "0";
        }
        bool f = true;
        for (; i >= 1; --i) {
            string dig_str = std::to_string(digits[i - 1]);
            if (!f) {
                for (size_t j = 0; j < 15 - dig_str.size(); ++j) {
                    s += '0';
                }
            } else {
                f = false;
            }
            s += std::to_string(digits[i - 1]);
        }
        return s;
    }

    inline static const uint64_t BASE = 1e15;
    vector<uint64_t> digits;
};

ostream &operator<<(ostream &os, const big_integer &val) {
    return os << val.to_string();
}

struct aho_corasick_machine {
    explicit aho_corasick_machine(const string &alphabet, const vector<string> &patterns)
            : nodes({node(alphabet)}), alphabet(alphabet) {
        nodes[ROOT].f[0] = 1;
        build_trie(patterns);
        calc_links();
    }

    size_t size() {
        return nodes.size();
    }

    big_integer calc_paths_num(size_t m) {
        for (size_t l = 1; l <= 199; ++l) {
            for (size_t cur = 0; cur < size(); ++cur) {
                for (const auto link : nodes[cur].delta) {
                    size_t next = link.second;
                    if (nodes[next].is_term) {
                        nodes[next].g[l] += nodes[cur].f[l - 1] + nodes[cur].g[l - 1];
                    } else {
                        nodes[next].f[l] += nodes[cur].f[l - 1];
                        nodes[next].g[l] += nodes[cur].g[l - 1];
                    }
                }
            }
        }
        big_integer result = 0;
        for (const node &cur : nodes) {
            result += cur.f[m];
        }
        return result;
    }

 private:
    struct node {
        explicit node(const string &alphabet) {
            for (char c : alphabet) {
                delta[c] = ROOT;
            }
        }

        unordered_map<char, size_t> delta;
        size_t pi = 0;

        bool is_term = false;

        vector<big_integer> f = vector<big_integer>(200, 0);
        vector<big_integer> g = vector<big_integer>(200, 0);
    };

    void build_trie(const vector<string> &patterns) {
        for (const string &pattern : patterns) {
            add(pattern);
        }
    }

    void add(const string &pattern) {
        size_t i = 0;
        size_t node_num = ROOT;
        while (i < pattern.size() && nodes[node_num].delta[pattern[i]] != ROOT) {
            node_num = nodes[node_num].delta[pattern[i]];
            ++i;
        }
        while (i < pattern.size()) {
            nodes[node_num].delta[pattern[i]] = nodes.size();
            node_num = nodes.size();
            nodes.emplace_back(alphabet);
            ++i;
        }
        nodes[node_num].is_term = true;
    }

    void calc_links() {
        queue<std::size_t> q;
        vector<bool> visited(size());
        visited[ROOT] = true;
        for (char c : alphabet) {
            size_t next = nodes[ROOT].delta[c];
            if (next != ROOT) {
                q.push(next);
                visited[next] = true;
            }
        }
        while (!q.empty()) {
            size_t cur = q.front();
            q.pop();
            calc_delta(cur);
            for (char c : alphabet) {
                size_t next = nodes[cur].delta[c];
                if (!visited[next]) {
                    q.push(next);
                    visited[next] = true;
                    calc_pi(cur, next, c);
                }
            }
        }
    }

    void calc_delta(size_t node_num) {
        for (char c : alphabet) {
            if (nodes[node_num].delta[c] == ROOT) {
                nodes[node_num].delta[c] = nodes[nodes[node_num].pi].delta[c];
            }
        }
    }

    void calc_pi(size_t cur, size_t next, char c) {
        nodes[next].pi = nodes[nodes[cur].pi].delta[c];
        nodes[next].is_term |= nodes[nodes[next].pi].is_term;
    }

    inline static const size_t ROOT = 0;
    vector<node> nodes;
    string alphabet;
};

int main() {
    size_t n, m, k;
    cin >> n >> m >> k;
    string alphabet;
    cin >> alphabet;
    vector<string> patterns;
    for (size_t i = 0; i < k; ++i) {
        string p;
        cin >> p;
        patterns.push_back(p);
    }
    aho_corasick_machine aho(alphabet, patterns);
    big_integer valid = aho.calc_paths_num(m);
    cout << valid;
    return 0;
}
