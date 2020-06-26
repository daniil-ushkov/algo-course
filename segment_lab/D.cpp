#include <iostream>
#include <cstdint>
#include <vector>

using namespace std;

const int32_t INF = 2 * 1e9;

template<typename T>
struct segment_tree {
  segment_tree(vector<T> &src, T (*op)(T, T), T neutral_el)
  : neutral_el_(neutral_el) {
    op_ = op;
    roots_.push_back(build(src, 0, src.size()));
  }

  T get(size_t ver, size_t l, size_t r) {
    return roots_[ver]->get(l, r);
  }

  void update_cur_ver(size_t ver, size_t i, T val) {
    roots_[ver]->update_cur_ver(i, val);
  }

  void create_new_ver(size_t ver, size_t i, T val) {
    roots_.push_back(roots_[ver]->create_new_ver(i, val));
  }

  T (*op_)(T, T);
  T neutral_el_;

  struct node {
    segment_tree<T> *tree;
    T value;
    size_t l, r;
    node *left_child;
    node *right_child;

    explicit node(segment_tree<T> *tree, T value, size_t l, size_t r)
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

    void update_cur_ver(size_t i, T val) {
      if (i < l || r <= i) {
        return;
      }
      if (leaf()) {
        value = val;
        return;
      }
      left_child->update_cur_ver(i, val);
      right_child->update_cur_ver(i, val);
      update_node();
    }

    node* create_new_ver(size_t i, T val) {
      if (i < l || r <= i) {
        return this;
      }
      if (leaf()) {
        return new node(tree, val, l, r);
      }
      node* new_node = new node(tree, tree->neutral_el_, l, r);
      new_node->left_child = left_child->create_new_ver(i, val);
      new_node->right_child = right_child->create_new_ver(i, val);
      new_node->update_node();
      return new_node;
    }
  };

  vector<node*> roots_;

  node* build(vector<T> &src, size_t l, size_t r) {
    if (l + 1 == r) {
      return new node(this, src[l], l, r);
    }
    node* new_node = new node(this, neutral_el_, l, r);
    size_t m = l + (r - l) / 2;
    new_node->left_child = build(src, l, m);
    new_node->right_child = build(src, m, r);
    new_node->update_node();
    return new_node;
  }
};

void run() {
  size_t n;
  cin >> n;
  vector<int32_t> a(n);
  for (size_t i = 0; i < n; ++i) {
    cin >> a[i];
  }
  int32_t (*f)(int32_t , int32_t) = [](int32_t a, int32_t b) { return min(a, b);};
  segment_tree<int32_t > t(a, f, INF);
  string token;
  while (cin >> token) {
    if (token == "min") {
      size_t l, r;
      cin >> l >> r;
      cout << t.get(t.roots_.size() - 1, l - 1, r) << "\n";
    }
    if (token == "set") {
      size_t i;
      int32_t val;
      cin >> i >> val;
      t.create_new_ver(t.roots_.size() - 1, i - 1, val);
    }
  }
}

int main() {
  run();
  return 0;
}
