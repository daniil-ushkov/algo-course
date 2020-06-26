#include <iostream>
#include <vector>

#pragma GCC optimize("O3")
#define fastio ios_base::sync_with_stdio(false), cin.tie(0), cout.tie(0)

using namespace std;

template<typename int_type>
struct diff_vector {
  explicit diff_vector(vector<int_type> &vec) : data_(vec.size()) {
    int_type prev = 0;
    for (size_t i = 0; i < data_.size(); ++i) {
      data_[i] = vec[i] - prev;
      prev = vec[i];
    }
  }

  void add(size_t l, size_t r, int_type val) {
    data_[l] += val;
    data_[r] -= val;
  }

  vector<int_type> convert() {
    vector<int_type> vec(data_);
    int_type prev = 0;
    for (size_t i = 0; i < vec.size(); ++i) {
      vec[i] += prev;
      prev = vec[i];
    }
    return vec;
  }

 private:
  vector<int_type> data_;
};

template<typename int_type>
struct pref_sum {
  explicit pref_sum(vector<int_type> &vec) : data_(vec.size()) {
    int_type prev = 0;
    for (size_t i = 0; i < data_.size(); ++i) {
      data_[i] = vec[i] + prev;
      prev = data_[i];
    }
  }

  int_type get(size_t l, size_t r) {
    if (l != 0) {
      return data_[r - 1] - data_[l - 1];
    }
    return data_[r - 1];
  }

 private:
  vector<int_type> data_;
};

size_t m, q;
uint32_t a, b, cur;
uint32_t next_rand() {
  cur = cur * a + b;
  return cur >> 8;
}

void run() {
  cin >> m >> q >> a >> b;

  vector<int32_t> vec((size_t) 1 << 24);

  diff_vector<int32_t> d(vec);

  uint32_t sum = 0;
  for (size_t i = 0; i < m; i++) {
    uint32_t add = next_rand();
    uint32_t l = next_rand();
    uint32_t r = next_rand();
    if (l > r) {
      swap(l, r);
    }
    d.add(l, r + 1, add);
  }
  vec = d.convert();
  pref_sum<int32_t> ps(vec);
  for (size_t i = 0; i < q; i++) {
    uint32_t l = next_rand();
    uint32_t r = next_rand();
    if (l > r) {
      swap(l, r);
    }
    sum += ps.get(l, r + 1);
  }
  cout << sum;
}

int main() {
  fastio;
  run();
  return 0;
}
