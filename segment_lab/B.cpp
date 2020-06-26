#include <iostream>
#include <vector>

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
    return min(data_[deg][l], data_[deg][r - (1 << deg)]);
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

int main() {
  fastio;
  size_t n, m, u1, v1;
  uint32_t a1;
  cin >> n >> m >> a1 >> u1 >> v1;

  init_log(n + 1);

  vector<uint32_t> a(n);
  for (size_t i = 0; i < n; ++i) {
    a[i] = a1;
    a1 = (23 * a1 + 21563) % 16714589;
  }

  sparse_table<uint32_t> st(a);

  uint32_t ans;
  for (size_t i = 1; i < m; ++i) {
    if (u1 < v1)
      ans = st.min_range(u1 - 1, v1);
    else
      ans = st.min_range(v1 - 1, u1);
    u1 = ((17 * u1 + 751 + ans + 2 * i) % n) + 1;
    v1 = ((13 * v1 + 593 + ans + 5 * i) % n) + 1;
  }
  if (u1 < v1)
    cout << u1 << " " << v1 << " " << st.min_range(u1 - 1, v1);
  else
    cout << u1 << " " << v1 << " " << st.min_range(v1 - 1, u1);

  return 0;
}
