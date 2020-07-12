#include <iostream>

#pragma GCC optimize("O3")
#define fastio ios_base::sync_with_stdio(false), cin.tie(0), cout.tie(0)

using namespace std;

struct BIT_RSQ2;

struct BIT {
  explicit BIT(uint32_t* vec, size_t size)
      : data_(new uint32_t[size]{}),
        size_(size) {
    build_(vec);
  }

  explicit BIT(size_t size)
      : data_(new uint32_t[size]{}),
        size_(size) {}

  uint32_t get(size_t l, size_t r) {
    return get_prefix_(r) - get_prefix_(l);
  }

  void add(size_t i, uint32_t val) {
    while (i < size_) {
      data_[i] += val;
      i |= i + 1;
    }
  }

 private:
  uint32_t* data_;
  size_t size_;

  void build_(uint32_t* vec) {
    for (size_t i = 0; i < size_; ++i) {
      add(i, vec[i]);
    }
  }

  friend BIT_RSQ2;
  uint32_t get_prefix_(size_t i) {
    uint32_t result = 0;
    while (i != 0) {
      result += data_[--i];
      i &= (i + 1);
    }
    return result;
  }
};

struct BIT_RSQ2 {
  explicit BIT_RSQ2(size_t size)
      : mul(size),
        add(size),
        size_(size) {}

  uint32_t get(size_t l, size_t r) {
    return get_prefix_(r) - get_prefix_(l);
  }

  void add_range(size_t l, size_t r, uint32_t val) {
    update_prefix_(r, val);
    update_prefix_(l, -val);
  }

 private:
  BIT mul;
  BIT add;
  size_t size_;

  uint32_t get_prefix_(size_t i) {
    return mul.get_prefix_(i) * i + add.get_prefix_(i);
  }

  void update_prefix_(size_t r, uint32_t val) {
    mul.add(0, val);
    mul.add(r, -val);
    add.add(r, val * r);
  }
};

size_t m, q;
uint32_t a, b, cur;
uint32_t next_rand() {
  cur = cur * a + b;
  return cur >> 8;
}

void run() {
  cin >> m >> q >> a >> b;
  BIT_RSQ2 tree(1 << 24);
  uint32_t sum = 0;
  for (size_t i = 0; i < m; i++) {
    uint32_t add = next_rand();
    uint32_t l = next_rand();
    uint32_t r = next_rand();
    if (l > r) {
      swap(l, r);
    }
    tree.add_range(l, r + 1, add);
  }
  for (size_t i = 0; i < q; i++) {
    uint32_t l = next_rand();
    uint32_t r = next_rand();
    if (l > r) {
      swap(l, r);
    }
    sum += tree.get(l, r + 1);
  }
  cout << sum;
}

void test() {
  BIT_RSQ2 t(10);
  string token;
  while (cin >> token) {
    if (token == "get") {
      size_t l, r;
      cin >> l >> r;
      cout << t.get(l, r) << "\n";
    }
    if (token == "update") {
      size_t l, r, val;
      cin >> l >> r >> val;
      t.add_range(l, r, val);
    }
  }
}

int main() {
  fastio;
  run();
  return 0;
}
