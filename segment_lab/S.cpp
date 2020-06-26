#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

#pragma GCC optimize("O3")
#define fastio ios_base::sync_with_stdio(false), cin.tie(0), cout.tie(0)

vector<size_t> log;

void init_log(size_t size) {
  log.assign(size, 0);
  size_t deg = 0;
  for (size_t i = 1; i < size; i++) {
    if (i == ((size_t) 1 << (deg + 1)))
      deg++;
    log[i] = deg;
  }
}

template<typename int_type>
struct sparse_table {
  explicit sparse_table(vector<int_type> &vec)
      : deg_(log[vec.size()]), size_(vec.size()), data_(deg_ + 1, vector<int_type>(vec.size())) {
    build_(vec);
  }

  int_type min_range(size_t l, size_t r) {
    size_t deg = log[r - l];
    return min(data_[deg][l], data_[deg][r - ((size_t) 1 << deg)]);
  }

 private:
  size_t deg_;
  size_t size_;
  vector<vector<int_type>> data_;
  void build_(vector<int_type> &vec) {
    data_[0] = vec;
    for (size_t deg = 0; deg <= deg_; ++deg) {
      for (size_t i = 0; i < size_; ++i) {
        if (i + ((size_t) 1 << (deg + 1)) <= size_) {
          data_[deg + 1][i] = min(data_[deg][i], data_[deg][i + ((size_t) 1 << deg)]);
        }
      }
    }
  }
};

void dfs(size_t cur,
         size_t prev,
         size_t d,
         vector<vector<size_t>> &tree,
         vector<pair<size_t, size_t>> &tour_depths,
         vector<size_t> &in,
         vector<size_t> &out) {
  in[cur] = tour_depths.size();
  tour_depths.emplace_back(d, cur);
  for (auto next : tree[cur]) {
    if (next != prev) {
      dfs(next, cur, d + 1, tree, tour_depths, in, out);
      tour_depths.emplace_back(d, cur);
    }
  }
  out[cur] = tour_depths.size();
  tour_depths.emplace_back(d, cur);
}

pair<size_t, size_t> lca(size_t u, size_t v,
                         vector<size_t> &in,
                         vector<size_t> &out,
                         sparse_table<pair<size_t, size_t>>& st) {
  if (in[u] > in[v]) swap(u, v);
  return st.min_range(in[u], out[v]);
}

int main() {
  fastio;
  init_log(1'000'000);
  size_t n, m;
  while (true) {
    cin >> n;
    if (n == 0) break;

    // Tree init
    vector<vector<size_t>> tree(n);
    for (size_t i = 1; i < n; ++i) {
      size_t u, v;
      cin >> u >> v;
      tree[u - 1].push_back(v - 1);
      tree[v - 1].push_back(u - 1);
    }


    // Tour
    vector<pair<size_t, size_t>> tour_depths;
    vector<size_t> in(tree.size());
    vector<size_t> out(tree.size());

    dfs(0, SIZE_MAX, 0, tree, tour_depths, in, out);


    sparse_table<pair<size_t, size_t>> st(tour_depths);

    size_t root = 0;

    cin >> m;
    while (m--) {
      string token;
      cin >> token;
      if (token == "?") {
        size_t u, v;
        cin >> u >> v;
        --u;
        --v;
        cout << max(max(lca(u, root, in, out, st), lca(v, root, in, out, st)),
                    lca(u, v, in, out, st)).second + 1 << "\n";
      }
      if (token == "!") {
        cin >> root;
        --root;
      }
    }
  }
  return 0;
}
