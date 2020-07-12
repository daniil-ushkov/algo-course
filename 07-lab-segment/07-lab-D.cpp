#include <iostream>
#include <cstdint>
#include <vector>

using namespace std;

const int32_t INF = 2 * 1e9;

struct range_tree {
  explicit range_tree(vector<int32_t> &vec)
      : data_(4 * vec.size()),
        set_(4 * vec.size(), INF),
        size_(vec.size()) {
    build_(vec, ROOT_);
//    PRINT_VEC(data_)
  }

  int32_t get(size_t l, size_t r) {
    segment_ query(-1, l, r);
    return get_(ROOT_, query);
  }

  void update(size_t l, size_t r, int32_t val) {
    segment_ query(-1, l, r);
    update_(ROOT_, query, val);
  }

 private:
  vector<int32_t> data_;
  vector<int32_t> set_;
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

  void build_(vector<int32_t> &vec, segment_ node) {
    if (node.is_leaf()) {
      data_[node.index] = vec[node.l];
      return;
    }
    build_(vec, node.left());
    build_(vec, node.right());
    data_[node.index] = min(data_[2 * node.index + 1], data_[2 * node.index + 2]);
  }

  int32_t get_(segment_ node, segment_ query) {
    push_(node);
    if (segment_::not_cross(node, query)) {
      return INF;
    }
    if (query.contains(node)) {
      return data_[node.index];
    }
    return min(get_(node.left(), query), get_(node.right(), query));
  }

  void update_(segment_ node, segment_ query, int32_t val) {
    if (segment_::not_cross(node, query)) {
      return;
    }
    if (query.contains(node)) {
      set_[node.index] = val;
      return;
    }
    update_(node.left(), query, val);
    update_(node.right(), query, val);
    push_(node.left());
    push_(node.right());
    data_[node.index] = min(data_[node.left().index], data_[node.right().index]);
  }

  void push_(segment_ node) {
    if (set_[node.index] != INF) {
      data_[node.index] = set_[node.index];
      if (!node.is_leaf()) {
        set_[node.left().index] = set_[node.index];
        set_[node.right().index] = set_[node.index];
      }
      set_[node.index] = INF;
    }
  }
};

void run() {
  size_t n;
  cin >> n;
  vector<int32_t> a(n);
  for (size_t i = 0; i < n; ++i) {
    cin >> a[i];
  }
  range_tree t(a);
  string token;
  while (cin >> token) {
    if (token == "min") {
      size_t l, r;
      cin >> l >> r;
      cout << t.get(l - 1, r) << "\n";
    }
    if (token == "set") {
      size_t i;
      int32_t val;
      cin >> i >> val;
      t.update(i - 1, i, val);
    }
  }
}

int main() {
  run();
  return 0;
}
