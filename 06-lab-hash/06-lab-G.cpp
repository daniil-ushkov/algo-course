#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#pragma GCC optimize("O3")

using namespace std;
struct tree {
  typedef size_t vert;
  vector<vert> **links;
  vert center;
  size_t size;

  explicit tree(size_t size) {
    this->size = size;
    links = new vector<vert> *[size];
    for (size_t v = 0; v < size; v++) {
      links[v] = new vector<vert>;
    }
    tree_class = new uint32_t[size];
    center = 4294967295;
    subtree_size = new size_t[size];
  }

  void link(vert u, vert v) {
    links[u]->push_back(v);
    links[v]->push_back(u);
  }

  void set_center() {
    size_t s = rand() % size;
    get_sizes(s, 4294967295);
    if (size % 2 == 1) {
      center = find_center(s);
    }
  }

  size_t *subtree_size;
  void get_sizes(vert u, vert p) {
    subtree_size[u] = 1;
    for (vert v : *links[u]) {
      if (v != p) {
        get_sizes(v, u);
        subtree_size[u] += subtree_size[v];
      }
    }
  }

  vert find_center(size_t root) {
    size_t half_size = size / 2;
    if (links[root]->size() == 2
        && subtree_size[(*links[root])[0]] == half_size) {
      return root;
    }

    vert curr = root;
    vert p = 4294967295;
    while (true) {
      bool stop = true;
      for (vert u : *links[curr]) {
        if (u != p && subtree_size[u] >= half_size + 1) {
          p = curr;
          curr = u;
          stop = false;
          break;
        }
      }
      if (stop) {
        return 4294967295;
      }
      if (subtree_size[curr] == half_size + 1) {
        break;
      }
    }
    if (links[curr]->size() == 2) {
      return curr;
    } else {
      return 4294967295;
    }
  }

  uint32_t A = 29;
  uint32_t P = 1e9 + 7;
  uint32_t *tree_class;
  uint32_t to_num(vector<uint32_t> &v) {
    sort(v.begin(), v.end());
    uint32_t res = 0;
    for (uint32_t cl : v) {
      res = (A * res + cl) % P;
    }
    return (res + 1) % P;
  }
  void calc_classes(vert u, vert p) {
    vector<uint32_t> children;
    for (vert v : *links[u]) {
      if (v != p) {
        calc_classes(v, u);
        children.push_back(tree_class[v]);
      }
    }
    tree_class[u] = to_num(children);
  }
};

int main() {
  srand(time(NULL));
  size_t n;
  cin >> n;
  if (n == 0) {
    cout << "NO";
    return 0;
  }
  tree t(n);
  tree::vert u, v;
  for (size_t i = 0; i < n - 1; i++) {
    cin >> u >> v;
    t.link(u - 1, v - 1);
  }
  if (n == 1) {
    cout << "NO";
    return 0;
  }
  t.set_center();
  tree::vert c = t.center;
  if (c == 4294967295) {
    cout << "NO";
    return 0;
  }
  u = (*t.links[c])[0];
  v = (*t.links[c])[1];
  t.calc_classes(u, c);
  t.calc_classes(v, c);
  if (t.tree_class[u] == t.tree_class[v]) {
    cout << "YES";
  } else {
    cout << "NO";
  }
  return 0;
}
