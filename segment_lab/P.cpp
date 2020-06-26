#include <iostream>
#include <vector>

using namespace std;

struct BIT_RSQ2;

struct BIT {
  explicit BIT(vector<uint64_t> &vec)
      : data_(vec.size()),
        size_(vec.size()) {
    build_(vec);
  }

  explicit BIT(size_t size)
      : data_(size),
        size_(size) {}

  uint64_t get(size_t l, size_t r) {
    return get_prefix_(r) - get_prefix_(l);
  }

  void add(size_t i, uint64_t val) {
    while (i < size_) {
      data_[i] += val;
      i |= i + 1;
    }
  }

 private:
  vector<uint64_t> data_;
  size_t size_;

  void build_(vector<uint64_t> vec) {
    for (size_t i = 0; i < vec.size(); ++i) {
      add(i, vec[i]);
    }
  }

  friend BIT_RSQ2;
  uint64_t get_prefix_(size_t i) {
    uint64_t result = 0;
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

  uint64_t get(size_t l, size_t r) {
    return get_prefix_(r) - get_prefix_(l);
  }

  void add_range(size_t l, size_t r, uint64_t val) {
    update_prefix_(r, val);
    update_prefix_(l, -val);
  }

 private:
  BIT mul;
  BIT add;
  size_t size_;

  uint64_t get_prefix_(size_t i) {
    return mul.get_prefix_(i) * i + add.get_prefix_(i);
  }

  void update_prefix_(size_t r, uint64_t val) {
    mul.add(0, val);
    mul.add(r, -val);
    add.add(r, val * r);
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
        bit(t.size_), top(t.size_), pos(t.size_) {
    build();
  }

  uint64_t calc(size_t u, size_t v) {
    uint64_t res = 0;
    while (top[u] != top[v]) {
      if (t.d[top[u]] > t.d[top[v]]) swap(u, v);
      res += bit.get(pos[top[v]], pos[v] + 1);
      v = t.parent[top[v]];
    }
    if (pos[u] > pos[v]) swap(u, v);
    res += bit.get(pos[u], pos[v] + 1);
    return res;
  }

  uint64_t get(size_t u) {
    return bit.get(pos[u], pos[u] + 1);
  }

  void update(size_t u, size_t v, uint64_t val) {
    while (top[u] != top[v]) {
      if (t.d[top[u]] < t.d[top[v]]) swap(u, v);
      bit.add_range(pos[top[u]], pos[u] + 1, val);
      u = t.parent[top[u]];
    }
    if (pos[u] > pos[v]) swap(u, v);
    bit.add_range(pos[u], pos[v] + 1, val);
  }

 private:
  tree const &t;

  vector<size_t> s;
  vector<size_t> heavy;

  BIT_RSQ2 bit;
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
  for (size_t i = 1; i < n; ++i) {
    size_t u, v;
    cin >> u >> v;
    graph[u - 1].push_back(v - 1);
    graph[v - 1].push_back(u - 1);
  }
  tree t(graph);
  heavy_light hld(t);

  size_t q;
  cin >> q;
  while (q--) {
    string token;
    cin >> token;
    if (token == "+") {
      size_t u, v;
      uint64_t d;
      cin >> u >> v >> d;
      hld.update(u - 1, v - 1, d);
    }
    if (token == "?") {
      size_t u;
      cin >> u;
      cout << hld.get(u - 1) << "\n";
    }
  }
}

int main() {
  run();
  return 0;
}
