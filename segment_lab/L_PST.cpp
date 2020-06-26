#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>

#pragma GCC optimize("O3")
#define fastio ios_base::sync_with_stdio(false), cin.tie(0), cout.tie(0)

using namespace std;

template<typename T, typename U>
struct segment_tree {
  segment_tree(vector<T> &src, T (*op)(T, T), T neutral_el, T update(T, U))
      : neutral_el_(neutral_el) {
    op_ = op;
    update_ = update;
    roots_.push_back(build(src, 0, src.size()));
  }

  T get(size_t ver, size_t l, size_t r) {
    return roots_[ver]->get(l, r);
  }

  void update_cur_ver(size_t ver, size_t i, U val) {
    roots_[ver]->update_cur_ver(i, val);
  }

  void create_new_ver(size_t ver, size_t i, U val) {
    roots_.push_back(roots_[ver]->create_new_ver(i, val));
  }

  T (*op_)(T, T);
  T neutral_el_;
  T (*update_)(T, U);


  struct node {
    segment_tree<T, U> *tree;
    T value;
    size_t l, r;
    node *left_child;
    node *right_child;

    explicit node(segment_tree<T, U> *tree, T value, size_t l, size_t r)
        : tree(tree), value(value), l(l), r(r), left_child(nullptr), right_child(nullptr) {}

    bool leaf() {
      return left_child == nullptr && right_child == nullptr;
    }

    void update_node() {
      if (!leaf()) {
        value = tree->op_(left_child->value, right_child->value);
      }
    }

    T get(size_t lq, size_t rq) {
      if (r <= lq || rq <= l) {
        return tree->neutral_el_;
      }
      if (lq <= l && r <= rq) {
        return value;
      }
      return tree->op_(left_child->get(lq, rq), right_child->get(lq, rq));
    }

    void update_cur_ver(size_t i, U val) {
      if (i < l || r <= i) {
        return;
      }
      if (leaf()) {
        value = tree->update_(value, val);
        return;
      }
      left_child->update_cur_ver(i, val);
      right_child->update_cur_ver(i, val);
      update_node();
    }

    node *create_new_ver(size_t i, U val) {
      if (i < l || r <= i) {
        return this;
      }
      if (leaf()) {
        return new node(tree, tree->update_(value, val), l, r);
      }
      node *new_node = new node(tree, tree->neutral_el_, l, r);
      new_node->left_child = left_child->create_new_ver(i, val);
      new_node->right_child = right_child->create_new_ver(i, val);
      new_node->update_node();
      return new_node;
    }
  };

  vector<node *> roots_;

  node *build(vector<T> &src, size_t l, size_t r) {
    if (l + 1 == r) {
      return new node(this, src[l], l, r);
    }
    node *new_node = new node(this, neutral_el_, l, r);
    size_t m = l + (r - l) / 2;
    new_node->left_child = build(src, l, m);
    new_node->right_child = build(src, m, r);
    new_node->update_node();
    return new_node;
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
  set<uint32_t> values;
  for (size_t i = 0; i < 1 << 17; ++i) {
    arr[i] = next_rand24();
    values.insert(arr[i]);
  }

  // Compression
  unordered_map<uint32_t, size_t> compressed;
  size_t new_compr = 0;
  for (auto v : values) {
    compressed[v] = new_compr++;
  }

  // Segment tree
  vector<uint32_t> vec(compressed.size());
  segment_tree<uint32_t, uint32_t> tree(
      vec,
      [](uint32_t a, uint32_t b) { return a + b; },
      0,
      [](uint32_t a, uint32_t b) { return a + b; });
  for (auto v : arr) {
    tree.create_new_ver(tree.roots_.size() - 1, compressed[v], 1);
  }

  uint32_t sum = 0;
  while (q--) {
    uint32_t l = next_rand17();
    uint32_t r = next_rand17();
    if (l > r) swap(l, r);

    uint32_t x = next_rand24();
    uint32_t y = next_rand24();
    if (x > y) swap(x, y);

    size_t cx;
    if (values.lower_bound(x) == values.end()) {
      cx = compressed.size();
    } else {
      cx = compressed[*values.lower_bound(x)];
    }
    size_t cy;
    if (values.upper_bound(y) == values.end()) {
      cy = compressed.size();
    } else {
      cy = compressed[*values.upper_bound(y)];
    }
    uint32_t ans = tree.get(r + 1, cx, cy) - tree.get(l, cx, cy);
    sum += ans;
    b += ans;
  }

  cout << sum << "\n";
}

int main() {
  run();
  return 0;
}
