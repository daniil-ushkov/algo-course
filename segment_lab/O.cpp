#include <iostream>
#include <vector>

using namespace std;

template<typename T>
struct segment_tree {
  segment_tree(vector<T> &vec, T (*bin_op)(T, T))
      : data_(4 * vec.size()),
        add_(4 * vec.size()),
        size_(vec.size()) {
    op = bin_op;
    build_(vec, ROOT_);
  }

  segment_tree(size_t size, T (*bin_op)(T, T))
      : data_(4 * size),
        add_(4 * size),
        size_(size) {
    op = bin_op;
    vector<T> vec(size);
    build_(vec, ROOT_);
  }

  T get(size_t l, size_t r) {
    segment_ query(-1, l, r);
    return get_(ROOT_, query);
  }

  void update(size_t l, size_t r, T val) {
    segment_ query(-1, l, r);
    update_(ROOT_, query, val);
  }

 private:
  vector<T> data_;
  vector<T> add_;
  T (*op)(T, T);
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

  void build_(vector<T> &vec, segment_ node) {
    if (node.is_leaf()) {
      data_[node.index] = vec[node.l];
      return;
    }
    build_(vec, node.left());
    build_(vec, node.right());
    data_[node.index] = op(data_[2 * node.index + 1], data_[2 * node.index + 2]);
  }

  T get_(segment_ node, segment_ query) {
    push_(node);
    if (segment_::not_cross(node, query)) {
      return INT64_MAX;
    }
    if (query.contains(node)) {
      return data_[node.index];
    }
    return op(get_(node.left(), query), get_(node.right(), query));
  }

  void update_(segment_ node, segment_ query, T val) {
    if (segment_::not_cross(node, query)) {
      return;
    }
    if (query.contains(node)) {
      add_[node.index] += val;
      return;
    }
    update_(node.left(), query, val);
    update_(node.right(), query, val);
    push_(node.left());
    push_(node.right());
    data_[node.index] = op(data_[node.left().index], data_[node.right().index]);
  }

  void push_(segment_ node) {
    data_[node.index] += add_[node.index];
    if (!node.is_leaf()) {
      add_[node.left().index] += add_[node.index];
      add_[node.right().index] += add_[node.index];
    }
    add_[node.index] = 0;
  }
};

struct tree {
  explicit tree(vector<vector<size_t>> const &graph)
      : children(graph.size()),
        parent(graph.size()),
        d(graph.size()),
        size_(graph.size()) {
    dfs(graph, 0, SIZE_MAX, 0);
  }

  vector<vector<size_t>> children;
  vector<size_t> parent;
  vector<size_t> d;
  size_t size_;

  bool leaf(size_t v) const {
    return children[v].empty();
  }

  void dfs(vector<vector<size_t>> const &graph, size_t cur, size_t prev, size_t depth) {
    d[cur] = depth;
    for (auto next : graph[cur]) {
      if (next != prev) {
        children[cur].push_back(next);
        parent[next] = cur;
        dfs(graph, next, cur, depth + 1);
      }
    }
  }
};

struct heavy_light {
  explicit heavy_light(tree const &t)
      : t(t),
        s(t.size_, 1), heavy(t.size_),
        st(t.size_, [](int64_t a, int64_t b) { return min(a, b); }), top(t.size_), pos(t.size_) {
    build();
  }

  int64_t calc(size_t u, size_t v) {
    int64_t res = INT64_MAX;
    while (top[u] != top[v]) {
      if (t.d[top[u]] < t.d[top[v]]) swap(u, v);
      res = min(res, st.get(pos[top[u]], pos[u] + 1));
      u = t.parent[top[u]];
    }
    if (pos[u] > pos[v]) swap(u, v);
    res = min(res, st.get(pos[u] + 1, pos[v] + 1));
    return res;
  }

  int64_t get(size_t u) {
    return st.get(pos[u], pos[u] + 1);
  }

  void update(size_t u, size_t v, int64_t val) {
    while (top[u] != top[v]) {
      if (t.d[top[u]] < t.d[top[v]]) swap(u, v);
      st.update(pos[top[u]], pos[u] + 1, val);
      u = t.parent[top[u]];
    }
    if (pos[u] > pos[v]) swap(u, v);
    st.update(pos[u], pos[v] + 1, val);
  }

 private:
  tree const &t;

  vector<size_t> s;
  vector<size_t> heavy;

  segment_tree<int64_t > st;
  vector<size_t> top;
  vector<size_t> pos;

  void calc_heavy(size_t cur) {
    for (auto next : t.children[cur]) {
      calc_heavy(next);
      s[cur] += s[next];
      if (2 * s[next] >= s[cur]) {
        heavy[cur] = next;
      }
    }
  }

  void create_paths(vector<size_t> &paths, size_t cur_top, size_t cur) {
    paths.push_back(cur);
    top[cur] = cur_top;
    pos[cur] = paths.size() - 1;
    if (!t.leaf(cur)) {
      create_paths(paths, cur_top, heavy[cur]);
    }
    for (auto next : t.children[cur]) {
      if (next != heavy[cur]) {
        create_paths(paths, next, next);
      }
    }
  }

  void build() {
    calc_heavy(0);
    vector<size_t> paths;
    create_paths(paths, 0, 0);
  }
};

void run() {
  size_t n;
  cin >> n;
  vector<vector<size_t>> graph(n);
  vector<int64_t > w(n);
  for (size_t i = 1; i < n; ++i) {
    size_t u;
    int64_t x;
    cin >> u >> x;
    w[i] = x;
    graph[i].push_back(u - 1);
    graph[u - 1].push_back(i);
  }
  tree t(graph);
  heavy_light hld(t);

  for (size_t i = 1; i < n; ++i) {
    hld.update(i, i, w[i]);
  }

  size_t q;
  cin >> q;
  while (q--) {
    size_t u, v;
    cin >> u >> v;
    cout << hld.calc(u - 1, v - 1) << "\n";
  }
}

int main() {
  run();
  return 0;
}
