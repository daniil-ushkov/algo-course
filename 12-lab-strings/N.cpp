#include <bits/stdc++.h>

#pragma GCC optimize("O3")

using namespace std;

using matrix_t = vector<vector<uint32_t>>;
using row_t = vector<uint32_t>;

matrix_t dot(matrix_t &m1, matrix_t &m2) {
    assert(m1[0].size() == m2.size());
    matrix_t res(m1.size(), row_t(m2[0].size(), 0));
    for (size_t i = 0; i < m1.size(); ++i) {
        for (size_t j = 0; j < m2[0].size(); ++j) {
            for (size_t k = 0; k < m1[0].size(); ++k) {
                res[i][j] = (res[i][j] + m1[i][k] * m2[k][j]) % 10000;
            }
        }
    }
    return res;
}

matrix_t pow(matrix_t &m, uint32_t d) {
    if (d == 1) {
        return m;
    }
    if (d % 2 == 0) {
        matrix_t m1 = pow(m, d / 2);
        return dot(m1, m1);
    } else {
        matrix_t m1 = pow(m, d - 1);
        return dot(m, m1);
    }
}

uint64_t pow1(uint64_t x, uint64_t d) {
    assert(x <= 10000);
    if (d == 1) {
        return x % 10000;
    }
    if (d % 2 == 0) {
        uint64_t m1 = pow1(x, d / 2);
        return (m1 * m1) % 10000;
    } else {
        uint64_t m1 = pow1(x, d - 1);
        return (x * m1) % 10000;
    }
}

struct aho_corasick_machine {
    explicit aho_corasick_machine(const vector<string> &patterns)
            : nodes({node()}) {
        build_trie(patterns);
        calc_links();
        init_matrix();
    }

    size_t size() {
        return nodes.size();
    }

    uint32_t calc_paths_num(size_t l) {
        matrix_t res_m = pow(m, l);
        matrix_t c(size(), row_t(1, 0));
        c[ROOT][0] = 1;
        res_m = dot(res_m, c);
        uint32_t result = 0;
        for (size_t i = 0; i < size(); ++i) {
            result = (result + res_m[i][0]) % 10000;
        }
        return result;
    }

    void init_matrix() {
        m.resize(nodes.size(), row_t(nodes.size(), 0));
        for (size_t node1 = 0; node1 < size(); ++node1) {
            for (const auto link : nodes[node1].delta) {
                size_t node2 = link;
                if (!nodes[node2].is_term) {
                    m[node2][node1] += 1;
                }
            }
        }
    }

private:
    struct node {
        size_t &get_delta(char c) {
            return delta[c - 'a'];
        }

        array<std::size_t, 26> delta;
        size_t pi;

        bool is_term;
    };

    void build_trie(const vector<string> &patterns) {
        for (const string &pattern : patterns) {
            add(pattern);
        }
    }

    void add(const string &pattern) {
        size_t i = 0;
        size_t node_num = ROOT;
        while (i < pattern.size() && nodes[node_num].get_delta(pattern[i]) != ROOT) {
            node_num = nodes[node_num].get_delta(pattern[i]);
            ++i;
        }
        while (i < pattern.size()) {
            nodes[node_num].get_delta(pattern[i]) = nodes.size();
            node_num = nodes.size();
            nodes.emplace_back();
            ++i;
        }
        nodes[node_num].is_term = true;
    }

    void calc_links() {
        queue<std::size_t> q;
        vector<bool> visited(size());
        visited[ROOT] = true;
        for (char c = 'a'; c <= 'z'; ++c) {
            size_t next = nodes[ROOT].get_delta(c);
            if (next != ROOT) {
                q.push(next);
                visited[next] = true;
            }
        }
        while (!q.empty()) {
            size_t cur = q.front();
            q.pop();
            calc_delta(cur);
            for (char c = 'a'; c <= 'z'; ++c) {
                size_t next = nodes[cur].get_delta(c);
                if (!visited[next]) {
                    q.push(next);
                    visited[next] = true;
                    calc_pi(cur, next, c);
                }
            }
        }
    }

    void calc_delta(size_t node_num) {
        for (char c = 'a'; c <= 'z'; ++c) {
            if (nodes[node_num].get_delta(c) == ROOT) {
                nodes[node_num].get_delta(c) = nodes[nodes[node_num].pi].get_delta(c);
            }
        }
    }

    void calc_pi(size_t cur, size_t next, char c) {
        nodes[next].pi = nodes[nodes[cur].pi].get_delta(c);
        nodes[next].is_term |= nodes[nodes[next].pi].is_term;
    }

    inline static const size_t ROOT = 0;
    vector<node> nodes;
    matrix_t m;
};

int main() {
    size_t n;
    cin >> n;
    vector<string> patterns;
    for (size_t i = 0; i < n; ++i) {
        string pattern;
        cin >> pattern;
        patterns.push_back(pattern);
    }
    size_t l;
    cin >> l;
    aho_corasick_machine m(patterns);
    cout << (pow1(26, l) + 10000 - m.calc_paths_num(l)) % 10000;
    return 0;
}
