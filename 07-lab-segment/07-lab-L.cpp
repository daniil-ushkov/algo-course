#include <iostream>
#include <vector>
#include <algorithm>

#pragma GCC optimize("O3")
#define fastio ios_base::sync_with_stdio(false), cin.tie(0), cout.tie(0)

using namespace std;

template<typename T>
struct two_dim_tree {
  explicit two_dim_tree(vector<T> &src) : data_(4 * src.size()), size_(src.size()) {
    build_(src, 0, 0, src.size());
  }

  size_t get(size_t l, size_t r, T x, T y) {
    return get_(0, 0, size_, l, r, x, y);
  }

 private:
  vector<vector<T>> data_;
  size_t size_;

  static size_t count_(vector<size_t> &v, T x, T y) {
    return upper_bound(v.begin(), v.end(), y) - lower_bound(v.begin(), v.end(), x);
  }

  void build_(vector<T> &src, size_t i, size_t l, size_t r) {
    if (l + 1 == r) {
      data_[i].push_back(src[l]);
      return;
    }
    size_t m = l + (r - l) / 2;
    build_(src, 2 * i + 1, l, m);
    build_(src, 2 * i + 2, m, r);
    merge(data_[2 * i + 1].begin(), data_[2 * i + 1].end(),
          data_[2 * i + 2].begin(), data_[2 * i + 2].end(),
          back_inserter(data_[i]));
  }

  size_t get_(size_t i, size_t l, size_t r, size_t ql, size_t qr, T qx, T qy) {
    if (r <= ql || qr <= l) {
      return 0;
    }
    if (ql <= l && r <= qr) {
      return count_(data_[i], qx, qy);
    }
    size_t m = l + (r - l) / 2;
    return (uint32_t) get_(2 * i + 1, l, m, ql, qr, qx, qy) + get_(2 * i + 2, m, r, ql, qr, qx, qy);
  }
};

size_t q;
uint32_t a, b, cur;
uint32_t next_rand17() {
  cur = cur * a + b;
  return cur >> 15;
}

uint32_t next_rand24() {
  cur = cur * a + b;
  return cur >> 8;
}

void run() {
  cin >> q >> a >> b;

  vector<uint32_t> arr(1 << 17);
  for (size_t i = 0; i < 1 << 17; ++i) {
    arr[i] = next_rand24();
  }

  two_dim_tree<uint32_t> t(arr);
  uint32_t sum = 0;
  while (q--) {
    uint32_t l = next_rand17();
    uint32_t r = next_rand17();
    if (l > r) swap(l, r);

    uint32_t x = next_rand24();
    uint32_t y = next_rand24();
    if (x > y) swap(x, y);

    uint32_t ans = t.get(l, r + 1, x, y);
    sum += ans;
    b += ans;
  }
  cout << sum << "\n";
}

int main() {
  run();
  return 0;
}
