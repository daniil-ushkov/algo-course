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

const size_t INF = SIZE_MAX;

struct range_tree {
  explicit range_tree(vector<bool> &vec)
      : data_(4 * vec.size()),
        size_(vec.size()) {
    build_(vec, ROOT_);
//    PRINT_VEC(data_)
  }

  size_t get(size_t l, size_t r) {
    segment_ query(-1, l, r);
    return get_(ROOT_, query);
  }

  void update(size_t i, bool val) {
    update_(ROOT_, i, val);
  }

 private:
  vector<size_t> data_;
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

    bool contains(size_t i) {
      return l <= i && i < r;
    }

    static bool not_cross(segment_ s1, segment_ s2) {
      return s1.r <= s2.l || s2.r <= s1.l;
    }
  };

  segment_ ROOT_ = segment_(0, 0, size_);

  void build_(vector<bool> &vec, segment_ node) {
    if (node.is_leaf()) {
      data_[node.index] = vec[node.l] ? INF : node.l;
      return;
    }
    build_(vec, node.left());
    build_(vec, node.right());
    data_[node.index] = min(data_[node.left().index], data_[node.right().index]);
  }

  size_t get_(segment_ node, segment_ query) {
    if (segment_::not_cross(node, query)) {
      return INF;
    }
    if (query.contains(node)) {
      return data_[node.index];
    }
    return min(get_(node.left(), query), get_(node.right(), query));
  }

  void update_(segment_ node, size_t i, bool val) {
    if (!node.contains(i)) {
      return;
    }
    if (node.is_leaf()) {
      data_[node.index] = val ? INF : node.l;
      return;
    }
    update_(node.left(), i, val);
    update_(node.right(), i, val);
    data_[node.index] = min(data_[node.left().index], data_[node.right().index]);
  }
};

void run() {
  size_t n, m;
  cin >> n >> m;
  vector<bool> vec(n);
  range_tree t(vec);
  for (size_t i = 0; i < m; ++i) {
    string token;
    size_t val;
    cin >> token >> val;
    val--;
    if (token == "enter") {
      size_t pos = t.get(val, n);
      if (pos == INF) {
        pos = t.get(0, val);
      }
      t.update(pos, true);
      cout << pos + 1 << "\n";
    }
    if (token == "exit") {
      t.update(val, false);
    }
  }
}

int main() {
  fastio;
  run();
  return 0;
}
