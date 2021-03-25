#include <bits/stdc++.h>

using namespace std;

#define FASTIO ios_base::sync_with_stdio(false), cin.tie(0), cout.tie(0)
#pragma GCC optimize("O3")

struct aho_corasick_machine {
    inline static constexpr size_t ALPHABET_SIZE = 'z' - 'a' + 1;

    explicit aho_corasick_machine(const vector<string> &patterns)
    : patterns_size(patterns.size()) {
        build_trie(patterns);
        calc_links();
    }

    vector<bool> accept(const std::string &text) {
        vector<bool> exists(patterns_size);
        size_t cur = ROOT;
        for (char c : text) {
            cur = nodes[cur].get_delta(c);
            size_t cur1 = cur;
            while (cur1 != SIZE_MAX) {
                if (nodes[cur1].used) {
                    break;
                }
                if (nodes[cur1].is_term) {
                    for (auto pattern_number : nodes[cur1].pattern_numbers) {
                        exists[pattern_number] = true;
                    }
                }
                nodes[cur1].used = true;
                cur1 = nodes[cur1].closest_term;
            }
        }
        return exists;
    }

    size_t size() {
        return nodes.size();
    }

 private:
    struct node {
        size_t &get_delta(char c) {
            return delta[c - 'a'];
        }

        array<std::size_t, ALPHABET_SIZE> delta;
        size_t pi;

        bool is_term;
        size_t closest_term = SIZE_MAX;

        bool used;

        vector<size_t> pattern_numbers;
    };

    void build_trie(const vector<string> &patterns) {
        for (size_t i = 0; i < patterns.size(); ++i) {
            add(patterns[i], i);
        }
    }

    void add(const string &pattern, size_t pattern_index) {
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
        nodes[node_num].pattern_numbers.push_back(pattern_index);
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
        nodes[next].closest_term = nodes[nodes[next].pi].is_term
                                   ? nodes[next].pi
                                   : nodes[nodes[next].pi].closest_term;
    }

    inline static const size_t ROOT = 0;
    vector<node> nodes = {node()};
    size_t patterns_size;
};

int main() {
    FASTIO;
    size_t n;
    cin >> n;
    vector<string> p;
    for (; n != 0; --n) {
        string s;
        cin >> s;
        p.push_back(s);
    }
    string t;
    cin >> t;
    aho_corasick_machine m(p);
    auto exists = m.accept(t);
    for (bool ans : exists) {
        cout << (ans ? "YES\n" : "NO\n");
    }
    return 0;
}
