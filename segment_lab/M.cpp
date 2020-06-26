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
         size_t d,
         vector<vector<size_t>> &tree,
         vector<pair<size_t, size_t>> &tour_depths,
         vector<size_t> &in,
         vector<size_t> &out) {
  in[cur] = tour_depths.size();
  tour_depths.emplace_back(d, cur);
  for (auto next : tree[cur]) {
    dfs(next, d + 1, tree, tour_depths, in, out);
    tour_depths.emplace_back(d, cur);
  }
  out[cur] = tour_depths.size();
  tour_depths.emplace_back(d, cur);
}

int main() {
  fastio;

  size_t n, m;
  cin >> n >> m;

  // Tree init
  vector<vector<size_t>> tree(n);
  for (size_t i = 1; i < n; ++i) {
    size_t v;
    cin >> v;
    tree[v].push_back(i);
  }

  uint64_t a1, a2, x, y, z;
  cin >> a1 >> a2 >> x >> y >> z;

  // Tour
  vector<pair<size_t, size_t>> tour_depths;
  vector<size_t> in(tree.size());
  vector<size_t> out(tree.size());

  dfs(0, 0, tree, tour_depths, in, out);

  init_log(tour_depths.size() + 1);
  sparse_table<pair<size_t, size_t>> st(tour_depths);

  uint64_t sum = 0;
  uint64_t ans = 0;
  while (m--) {
    size_t v1 = (size_t) (a1 + ans) % n;
    size_t v2 = (size_t) a2;
    if (in[v1] > in[v2]) swap(v1, v2);
    ans = st.min_range(in[v1], out[v2]).second;
    sum += ans;

    a1 = ((x * a1) % n + (y * a2) % n + z) % n;
    a2 = ((x * a2) % n + (y * a1) % n + z) % n;
  }

  cout << sum;

  return 0;
}
