#include <iostream>
#include <vector>

using namespace std;

struct BIT_RSQ2;

struct BIT {
  explicit BIT(vector<uint32_t> &vec)
      : data_(vec.size()),
        size_(vec.size()) {
    build_(vec);
  }

  explicit BIT(size_t size)
      : data_(size),
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
  vector<uint32_t> data_;
  size_t size_;

  void build_(vector<uint32_t> vec) {
    for (size_t i = 0; i < vec.size(); ++i) {
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

int main() {

  return 0;
}