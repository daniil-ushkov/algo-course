#include <iostream>
#include <vector>

using namespace std;

#pragma GCC optimize("O3")
#define fastio ios_base::sync_with_stdio(false), cin.tie(0), cout.tie(0)

#define MOD ((int64_t) 1'000'000'007)

vector<size_t> log;

void init_log(size_t size) {
  log.assign(size, 0);
  size_t deg = 0;
  for (size_t i = 1; i < size; i++) {
    if (i == ((size_t) 1 << (deg + 1)))
      deg++;
    log[i] = deg;
  }
}

template<typename T>
struct sparse_table_2D {
  explicit sparse_table_2D(vector<vector<T>> &vec)
      : n_(vec.size()),
        m_(vec[0].size()),
        data_(log[n_] + 1, vector<vector<vector<T>>>(log[m_] + 1, vec)) {
    build_();
  }

  T get(size_t r1, size_t c1, size_t r2, size_t c2) {
    if (r1 > r2) swap(r1, r2);
    if (c1 > c2) swap(c1, c2);
    size_t h = log[r2 - r1];
    size_t w = log[c2 - c1];
    return intersect(
        intersect(data_[h][w][r1][c1], data_[h][w][r2 - (1 << h)][c2 - (1 << w)]),
        intersect(data_[h][w][r1][c2 - (1 << w)], data_[h][w][r2 - (1 << h)][c1]));
  }

 private:
  size_t n_, m_;
  vector<vector<vector<vector<T>>>> data_;
  void build_() {
    for (size_t h = 1; h <= log[n_]; ++h) {
      for (size_t i = 0; i < n_; ++i) {
        for (size_t j = 0; j < m_; ++j) {
          if (i + (1 << (h)) <= n_) {
            data_[h][0][i][j] = intersect(
                data_[h - 1][0][i][j],
                data_[h - 1][0][i + (1 << (h - 1))][j]);
          }
        }
      }
    }
    for (size_t w = 1; w <= log[m_]; ++w) {
      for (size_t i = 0; i < n_; ++i) {
        for (size_t j = 0; j < m_; ++j) {
          if (j + (1 << (w)) <= m_) {
            data_[0][w][i][j] = intersect(
                data_[0][w - 1][i][j],
                data_[0][w - 1][i][j + (1 << (w - 1))]);
          }
        }
      }
    }
    for (size_t h = 0; h < log[n_]; ++h) {
      for (size_t w = 0; w < log[m_]; ++w) {
        for (size_t i = 0; i < n_; ++i) {
          for (size_t j = 0; j < m_; ++j) {
            if (i + (1 << (h + 1)) <= n_ && j + (1 << (w + 1)) <= m_) {
              data_[h + 1][w + 1][i][j] = intersect(
                  intersect(
                      data_[h][w][i][j],
                      data_[h][w][i + (1 << (h))][j]),
                  intersect(
                      data_[h][w][i][j + (1 << (w))],
                      data_[h][w][i + (1 << (h))][j + (1 << (w))]));
            }
          }
        }
      }
    }
  }
};

struct square {
  int64_t x1, y1, x2, y2;

  uint64_t area() {
    if (x1 < x2 && y1 < y2) {
      return (x2 - x1) * (y2 - y1);
    }
    return 0;
  }
};

square intersect(square const &sq1, square const &sq2) {
  return {max(sq1.x1, sq2.x1), max(sq1.y1, sq2.y1), min(sq1.x2, sq2.x2), min(sq1.y2, sq2.y2)};
}

istream &operator>>(istream &in, square &sq) {
  in >> sq.x1 >> sq.y1 >> sq.x2 >> sq.y2;
  if (sq.x1 > sq.x2) swap(sq.x1, sq.x2);
  if (sq.y1 > sq.y2) swap(sq.y1, sq.y2);
  return in;
}

int main() {
  size_t n, m;
  cin >> n >> m;
  init_log(n + m + 1);
  vector<vector<square>> t(n, vector<square>(m));
  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 0; j < m; ++j) {
      cin >> t[i][j];
    }
  }
  sparse_table_2D<square> st(t);

  size_t q, a, b, v0;
  cin >> q >> a >> b >> v0;
  uint64_t ans = 0;
  while (q--) {
    size_t v1, v2, v3, v4;
    v0 = (size_t) (((uint64_t) a * v0) % MOD + b) % MOD;
    v1 = v0 % n;
    v0 = (size_t) (((uint64_t) a * v0) % MOD + b) % MOD;
    v2 = v0 % m;
    v0 = (size_t) (((uint64_t) a * v0) % MOD + b) % MOD;
    v3 = v0 % n;
    v0 = (size_t) (((uint64_t) a * v0) % MOD + b) % MOD;
    v4 = v0 % m;
    if (v1 > v3) swap(v1, v3);
    if (v2 > v4) swap(v2, v4);
    ans = (ans + st.get(v1, v2, v3 + 1, v4 + 1).area()) % MOD;
  }
  cout << ans << "\n";
  return 0;
}
