#include <iostream>
#include <cstdint>
#include <vector>

#define PRINT_VEC(vec) {\
for (auto el : vec)\
  cout << el << " ";\
  cout << "\n";\
  }

using namespace std;

#pragma GCC optimize("O3")
#define fastio ios_base::sync_with_stdio(false), cin.tie(0), cout.tie(0)

struct segment_tree {
  explicit segment_tree(vector<uint32_t> &vec)
      : data_(4 * vec.size()),
        add_(4 * vec.size()),
        size_(vec.size()) {
    build_(vec, ROOT_);
//    PRINT_VEC(data_)
  }

  uint32_t get(size_t l, size_t r) {
    segment_ query(-1, l, r);
    return get_(ROOT_, query);
  }

  void update(size_t l, size_t r, uint32_t val) {
    segment_ query(-1, l, r);
    update_(ROOT_, query, val);
  }

 private:
  vector<uint32_t> data_;
  vector<uint32_t> add_;
  size_t size_;

  struct segment_ {
    size_t index;
    size_t l;
    size_t r;

    segment_(size_t index, size_t l, size_t r) : index(index), l(l), r(r) {}

    segment_ left() {
      size_t m = l + (r - l) / 2;
      return segment_(2 * index + 1, l, m);
    }

    segment_ right() {
      size_t m = l + (r - l) / 2;
      return segment_(2 * index + 2, m, r);
    }

    size_t length() {
      return r - l;
    }

    bool is_leaf() {
      return l + 1 == r;
    }

    bool contains(segment_ seg) {
      return l <= seg.l && seg.r <= r;
    }

    static bool not_cross(segment_ s1, segment_ s2) {
      return s1.r <= s2.l || s2.r <= s1.l;
    }
  };

  segment_ ROOT_ = segment_(0, 0, size_);

  void build_(vector<uint32_t> &vec, segment_ node) {
    if (node.is_leaf()) {
      data_[node.index] = vec[node.l];
      return;
    }
    build_(vec, node.left());
    build_(vec, node.right());
    data_[node.index] = data_[2 * node.index + 1] + data_[2 * node.index + 2];
  }

  uint32_t get_(segment_ node, segment_ query) {
    push_(node);
    if (segment_::not_cross(node, query)) {
      return 0;
    }
    if (query.contains(node)) {
      return data_[node.index];
    }
    return get_(node.left(), query) + get_(node.right(), query);
  }

  void update_(segment_ node, segment_ query, uint32_t val) {
    if (segment_::not_cross(node, query)) {
      return;
    }
    if (query.contains(node)) {
      add_[node.index] = val;
      return;
    }
    update_(node.left(), query, val);
    update_(node.right(), query, val);
    push_(node.left());
    push_(node.right());
    data_[node.index] = data_[node.left().index] + data_[node.right().index];
  }

  void push_(segment_ node) {
    data_[node.index] += node.length() * add_[node.index];
    if (!node.is_leaf()) {
      add_[node.left().index] += add_[node.index];
      add_[node.right().index] += add_[node.index];
    }
    add_[node.index] = 0;
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
  vector<uint32_t> vec(1 << 24);
  segment_tree tree(vec);
  uint32_t sum = 0;
  for (size_t i = 0; i < m; i++) {
    uint32_t add = next_rand();
    uint32_t l = next_rand();
    uint32_t r = next_rand();
    if (l > r) {
      swap(l, r);
    }
    tree.update(l, r + 1, add);
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

int main() {
  fastio;
  run();
  return 0;
}
