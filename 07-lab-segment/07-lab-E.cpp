#include <iostream>
#include <vector>

#pragma GCC optimize("O3")
#define fastio ios_base::sync_with_stdio(false), cin.tie(0), cout.tie(0)

using namespace std;

struct BIT {
  explicit BIT(vector<int64_t > &vec)
      : data_(vec.size()),
        size_(vec.size()) {
    build_(vec);
  }

  explicit BIT(size_t size)
      : data_(size),
        size_(size) {}

  int64_t get(size_t l, size_t r) {
    return get_prefix_(r) - get_prefix_(l);
  }

  void add(size_t i, int64_t val) {
    while (i < size_) {
      data_[i] += val;
      i |= i + 1;
    }
  }

 private:
  vector<int64_t > data_;
  size_t size_;

  void build_(vector<int64_t > vec) {
    for (size_t i = 0; i < vec.size(); ++i) {
      add(i, vec[i]);
    }
  }

  int64_t get_prefix_(size_t i) {
    int64_t result = 0;
    while (i != 0) {
      result += data_[--i];
      i &= (i + 1);
    }
    return result;
  }
};

void run() {
  size_t n;
  cin >> n;
  vector<int64_t > a(n);
  for (size_t i = 0; i < n; ++i) {
    cin >> a[i];
  }
  BIT bit(a);
  string token;
  while (cin >> token) {
    if (token == "sum") {
      size_t l, r;
      cin >> l >> r;
      cout << bit.get(l - 1, r) << "\n";
    }
    if (token == "set") {
      size_t i;
      int64_t val;
      cin >> i >> val;
      bit.add(i - 1, val - bit.get(i - 1, i));
    }
  }
}

int main() {
  fastio;
  run();
  return 0;
}
