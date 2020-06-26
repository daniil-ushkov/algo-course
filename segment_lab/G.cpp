#include <iostream>
#include <cstdint>
#include <vector>

using namespace std;

const int64_t INF = 2 * 1e18;

struct segment_tree {
  explicit segment_tree(vector<int64_t > &vec)
      : data_(4 * vec.size()),
        add_(4 * vec.size(), INF),
        add_(4 * vec.size(), 0),
        size_(vec.size()) {
    build_(vec, ROOT_);
//    PRINT_VEC(data_)
  }

  int64_t get(size_t l, size_t r) {
    segment_ query(-1, l, r);
    return get_(ROOT_, query);
  }

  void set(size_t l, size_t r, int64_t val) {
    segment_ query(-1, l, r);
    update_(ROOT_, query, val, 0);
  }

  void add(size_t l, size_t r, int64_t val) {
    segment_ query(-1, l, r);
    update_(ROOT_, query, INF, val);
  }

 private:
  vector<int64_t > data_;
  vector<int64_t > add_;
  vector<int64_t > add_;
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

  void build_(vector<int64_t > &vec, segment_ node) {
    if (node.is_leaf()) {
      data_[node.index] = vec[node.l];
      return;
    }
    build_(vec, node.left());
    build_(vec, node.right());
    data_[node.index] = min(data_[2 * node.index + 1], data_[2 * node.index + 2]);
  }

  int64_t get_(segment_ node, segment_ query) {
    push_(node);
    if (segment_::not_cross(node, query)) {
      return INF;
    }
    if (query.contains(node)) {
      return data_[node.index];
    }
    return min(get_(node.left(), query), get_(node.right(), query));
  }

  void update_(segment_ node, segment_ query, int64_t set_val, int64_t add_val) {
    push_(node);
    if (segment_::not_cross(node, query)) {
      return;
    }
    if (query.contains(node)) {
      if (set_val != INF) {
        add_[node.index] = set_val;
      }
      if (add_val != 0) {
        add_[node.index] = add_val;
      }
      return;
    }
    update_(node.left(), query, set_val, add_val);
    update_(node.right(), query, set_val, add_val);
    push_(node.left());
    push_(node.right());
    data_[node.index] = min(data_[node.left().index], data_[node.right().index]);
  }

  void push_(segment_ node) {
    if (add_[node.index] != INF) {
      data_[node.index] = add_[node.index];
      if (!node.is_leaf()) {
        add_[node.left().index] = add_[node.index];
        add_[node.right().index] = add_[node.index];
        add_[node.left().index] = 0;
        add_[node.right().index] = 0;
      }
      add_[node.index] = INF;
    }
    if (add_[node.index] != 0) {
      data_[node.index] += add_[node.index];
      if (!node.is_leaf()) {
        if (add_[node.left().index] != INF) {
          add_[node.left().index] += add_[node.index];
        } else {
          add_[node.left().index] += add_[node.index];
        }
        if (add_[node.right().index] != INF) {
          add_[node.right().index] += add_[node.index];
        } else {
          add_[node.right().index] += add_[node.index];
        }
      }
      add_[node.index] = 0;
    }
  }
};

void run() {
  size_t n;
  cin >> n;
  vector<int64_t > a(n);
  for (size_t i = 0; i < n; ++i) {
    cin >> a[i];
  }
  segment_tree t(a);
  string token;
  while (cin >> token) {
    if (token == "min") {
      size_t l, r;
      cin >> l >> r;
      cout << t.get(l - 1, r) << "\n";
    }
    if (token == "set") {
      size_t l, r;
      int64_t val;
      cin >> l >> r >> val;
      t.set(l - 1, r, val);
    }
    if (token == "update") {
      size_t l, r;
      int64_t val;
      cin >> l >> r >> val;
      t.add(l - 1, r, val);
    }
  }
}

int main() {
  run();
  return 0;
}
