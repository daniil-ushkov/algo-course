#include <iostream>
#include <cstdint>
#include <vector>
#include <map>
#include <set>
#include <tuple>
#include <algorithm>

using namespace std;

#define OP(node1, node2) (node1.first > node2.first ? node1 : node2)

struct range_tree {
  explicit range_tree(vector<pair<size_t, int32_t> > &vec)
      : data_(4 * vec.size()),
        add_(4 * vec.size()),
        size_(vec.size()) {
    build_(vec, ROOT_);
  }

  pair<size_t, int32_t> get(size_t l, size_t r) {
    segment_ query(-1, l, r);
    return get_(ROOT_, query);
  }

  void update(size_t l, size_t r, int32_t val) {
    segment_ query(-1, l, r);
    update_(ROOT_, query, val);
  }

 private:
  vector<pair<size_t, int32_t>> data_;
  vector<int32_t > add_;
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

  void build_(vector<pair<size_t, int32_t>> &vec, segment_ node) {
    if (node.is_leaf()) {
      data_[node.index] = vec[node.l];
      return;
    }
    build_(vec, node.left());
    build_(vec, node.right());
    data_[node.index] = OP(data_[2 * node.index + 1], data_[2 * node.index + 2]);
  }

  pair<size_t, int32_t> get_(segment_ node, segment_ query) {
    push_(node);
    if (segment_::not_cross(node, query)) {
      return {0, INT32_MAX};
    }
    if (query.contains(node)) {
      return data_[node.index];
    }
    return OP(get_(node.left(), query), get_(node.right(), query));
  }

  void update_(segment_ node, segment_ query, int32_t val) {
    push_(node);
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
    data_[node.index] = OP(data_[node.left().index], data_[node.right().index]);
  }

  void push_(segment_ node) {
    data_[node.index].first += add_[node.index];
    if (!node.is_leaf()) {
      add_[node.left().index] += add_[node.index];
      add_[node.right().index] += add_[node.index];
    }
    add_[node.index] = 0;
  }
};

struct window_edge {
  int32_t x, y1, y2;
  bool first;
};

void run() {
  size_t n;
  cin >> n;

  vector<window_edge> xs;
  set<int32_t> ys;

  for (size_t i = 0; i < n; i++) {
    int32_t x1, y1, x2, y2;
    cin >> x1 >> y1 >> x2 >> y2;
    xs.push_back({x1, y1, y2, true});
    xs.push_back({x2 + 1, y1, y2, false});
    ys.insert(y1);
    ys.insert(y2);
  }

  // Coordinate compression
  map<int32_t, size_t> compressed;
  vector<pair<size_t, int32_t >> vec;
  size_t j = 0;
  for (auto y : ys) {
    compressed[y] = j;
    vec.emplace_back(0, y);
    j++;
  }
  range_tree tree(vec);

  // Scanline
  sort(xs.begin(), xs.end(),
       [](const window_edge &we1, const window_edge &we2) {
         if (we1.x == we2.x) {
           return !we1.first && we2.first;
         }
         return we1.x < we2.x;
       });
  tuple<size_t, int32_t, int32_t> result = {0, INT32_MAX, INT32_MAX};
  for (auto x : xs) {
    tree.update(compressed[x.y1], compressed[x.y2] + 1, x.first ? 1 : -1);
    auto query = tree.get(0, ys.size());
    if (get<0>(result) < query.first) {
      result = {query.first, x.x, query.second};
    }
  }

  cout << get<0>(result) << "\n" << get<1>(result) << " " << get<2>(result) << "\n";
}

int main() {
  run();
}

