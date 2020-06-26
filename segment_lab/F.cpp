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

uint32_t MOD = 10'000;

struct matrix {
  matrix() = default;

  matrix(uint32_t e11, uint32_t e12, uint32_t e21, uint32_t e22) {
    data_[0][0] = e11;
    data_[0][1] = e12;
    data_[1][0] = e21;
    data_[1][1] = e22;
  }

  matrix dot(matrix m) {
    return matrix((data_[0][0] * m.data_[0][0] + data_[0][1] * m.data_[1][0]) % MOD,
                  (data_[0][0] * m.data_[0][1] + data_[0][1] * m.data_[1][1]) % MOD,
                  (data_[1][0] * m.data_[0][0] + data_[1][1] * m.data_[1][0]) % MOD,
                  (data_[1][0] * m.data_[0][1] + data_[1][1] * m.data_[1][1]) % MOD);
  }
  uint32_t data_[2][2]{};
};

istream &operator>>(istream &in, matrix &m) {
  cin >> m.data_[0][0] >> m.data_[0][1] >> m.data_[1][0] >> m.data_[1][1];
  return in;
}

ostream &operator<<(ostream &out, matrix m) {
  cout << m.data_[0][0] << " " << m.data_[0][1] << "\n"
       << m.data_[1][0] << " " << m.data_[1][1] << "\n";
  return out;
}

struct segment_tree {
  explicit segment_tree(vector<matrix> &vec)
      : data_(4 * vec.size()),
        add_(4 * vec.size()),
        size_(vec.size()) {
    build_(vec, ROOT_);
//    PRINT_VEC(data_)
  }

  matrix get(size_t l, size_t r) {
    segment_ query(-1, l, r);
    return get_(ROOT_, query);
  }

 private:
  vector<matrix> data_;
  vector<matrix> add_;
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

  void build_(vector<matrix> &vec, segment_ node) {
    if (node.is_leaf()) {
      data_[node.index] = vec[node.l];
      return;
    }
    build_(vec, node.left());
    build_(vec, node.right());
    data_[node.index] = data_[2 * node.index + 1].dot(data_[2 * node.index + 2]);
  }

  matrix get_(segment_ node, segment_ query) {
    if (segment_::not_cross(node, query)) {
      return matrix(1, 0, 0, 1);
    }
    if (query.contains(node)) {
      return data_[node.index];
    }
    return get_(node.left(), query).dot(get_(node.right(), query));
  }
};

void run() {
  size_t n, m;
  cin >> MOD >> n >> m;
  vector<matrix> a(n);
  for (size_t i = 0; i < n; ++i) {
    cin >> a[i];
  }
  segment_tree t(a);
  for (size_t i = 0; i < m; ++i) {
    size_t l, r;
    cin >> l >> r;
    cout << t.get(l - 1, r);
    cout << "\n";
  }
}

int main() {
  fastio;
  run();
  return 0;
}
