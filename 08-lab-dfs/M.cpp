#include <bits/stdc++.h>

using namespace std;

int n;
vector<vector<int>> g(26);
vector<bool> used(26);
vector<int> sorted;
vector<bool> first(26);

void top_sort() {
    vector<int> in(26);
    for (int u = 0; u < 26; ++u) {
        for (int v : g[u]) {
            in[v]++;
        }
    }
    queue<int> q;
    for (int u = 0; u < 26; ++u) {
        if (in[u] == 0 && !first[u]) {
            q.push(u);
            break;
        }
    }
    if (q.empty()) {
        return;
    }
    for (int u = 0; u < 26; ++u) {
        if (in[u] == 0 && u != q.front()) {
            q.push(u);
        }
    }
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        sorted.push_back(u);
        for (int v : g[u]) {
            if (--in[v] == 0) {
                q.push(v);
            }
        }
    }
}

void update(string& prev_word, string& word) {
    if (prev_word.size() != word.size()) {
        return;
    }
    int prefix = 0;
    while (prev_word[prefix] == word[prefix]) {
        prefix++;
    }
    g[prev_word[prefix] - 'a'].push_back(word[prefix] - 'a');
}

int main() {
    cin >> n;
    string prev_word;
    cin >> prev_word;
    n--;
    while (n--) {
        string cur_word;
        cin >> cur_word;
        if (prev_word.size() > cur_word.size() || prev_word == cur_word) {
            cout << "No";
            return 0;
        }
        if (prev_word.size() != 1) {
            first[prev_word[0] - 'a'] = true;
        }
        if (cur_word.size() != 1) {
            first[cur_word[0] - 'a'] = true;
        }
        update(prev_word, cur_word);
        prev_word = cur_word;
    }
    top_sort();
    if (sorted.size() < 26u) {
        cout << "No";
        return 0;
    }
    vector<int> ans(26);
    for (int i = 0; i < 26; ++i) {
        ans[sorted[i]] = i;
    }
    cout << "Yes\n";
    for (int u = 0; u < 26; ++u) {
        cout << ans[u] << " ";
    }
    return 0;
}
