#include <bits/stdc++.h>

using namespace std;

const size_t ALPHABET_SIZE = 'z' - 'a' + 1;

#define TO_INDEX(c) ((c) - 'a')

struct node {
    node()
            : delta_(), root_(this), pi_(this),
              closest_term_(nullptr), term_(false),
              pattern_numbers(), visited_(false) {
        delta_.fill(this);
    }

    node(node *root)
            : delta_(), root_(root), pi_(root),
              closest_term_(nullptr), term_(false),
              pattern_numbers(), visited_(false) {
        delta_.fill(root);
    }

    static node *build_trie(const std::vector<std::string> &patterns) {
        node *root = new node();
        for (size_t i = 0; i < patterns.size(); ++i) {
            root->add(patterns[i], i);
        }
        return root;
    }

    static node *build_aho_corasick_trie(const std::vector<std::string> &patterns) {
        node *root = build_trie(patterns);
        bfs(root);
        return root;
    }

    static void bfs(node *root) {
        std::queue<node *> q;
        root->visited_ = true;
        for (char c = 'a'; c <= 'z'; ++c) {
            if (root->has_next(c)) {
                node *next = root->delta(c);
                q.push(next);
                next->visited_ = true;
            }
        }
        while (!q.empty()) {
            node *cur = q.front();
            q.pop();
            cur->calc_delta();
            for (char c = 'a'; c <= 'z'; ++c) {
                node *next = cur->delta(c);
                if (next != nullptr && !next->visited_) {
                    q.push(next);
                    next->visited_ = true;
                    cur->calc_pi(next, c);
                }
            }
        }
    }

    void calc_delta() {
        for (char c = 'a'; c <= 'z'; ++c) {
            if (!has_next(c)) {
                delta_[TO_INDEX(c)] = pi()->delta(c);
            }
        }
    }

    void calc_pi(node *next, char c) {
        next->pi_ = pi()->delta(c);
        next->closest_term_ = pi()->delta(c)->term_
                              ? pi()->delta(c)
                              : pi()->delta(c)->closest_term_;
    }

    void add(const std::string &s, size_t index) {
        size_t i = 0;
        node *cur = this;
        while (i < s.size() && cur->has_next(s[i])) {
            cur = cur->delta(s[i]);
            ++i;
        }
        while (i < s.size()) {
            cur = cur->add(s[i]);
            ++i;
        }
        cur->term_ = true;
        cur->pattern_numbers.push_back(index);
    }

    node *add(char c) {
        return delta_[TO_INDEX(c)] = new node(root_);
    }

    bool has_next(char c) {
        return delta_[TO_INDEX(c)] != root_;
    }

    node *delta(char c) {
        return delta_[TO_INDEX(c)];
    }

    node *pi() {
        return pi_;
    }

    static void accept(const std::vector<std::string> &patterns,
                       std::vector<bool> &exists, const std::string &text) {
        node *root = build_aho_corasick_trie(patterns);
        node *cur = root;
        for (char c : text) {
            cur = cur->delta(c);
            node *cur1 = cur;
            while (cur1 != nullptr) {
                if (cur1->term_) {
                    for (auto pattern_number : cur1->pattern_numbers)
                        exists[pattern_number] = true;
                }
                cur1 = cur1->closest_term_;
            }
        }
    }

 private:
    std::array<node *, ALPHABET_SIZE> delta_;
    node *root_;
    node *pi_;
    node *closest_term_;
    bool term_;
    vector<size_t> pattern_numbers;
    bool visited_;
};


int main() {
    string t;
    cin >> t;
    size_t n;
    cin >> n;
    vector<string> p;
    for (; n != 0; --n) {
        string s;
        cin >> s;
        p.push_back(s);
    }
    vector<bool> exists(p.size(), false);
    node::accept(p, exists, t);
    for (bool ans : exists) {
        cout << (ans ? "Yes\n" : "No\n");
    }
    return 0;
}
