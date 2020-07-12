#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

ifstream fin("sequences.in");
ofstream fout("sequences.out");

typedef vector<vector<int>> matrix;
typedef vector<int> line;

const long long mod = 999999937;

matrix mul(matrix a, matrix b) {
  int n = a.size();
  int m = b[0].size();
  int l = a[0].size();
  matrix c(n, line(m, 0));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      for (int k = 0; k < l; k++) {
        c[i][j] = (int) (c[i][j] + ((long long) a[i][k] * b[k][j]) % mod) % mod;
      }
    }
  }
  return c;
}

matrix pow(matrix a, long long n) {
  if (n == 1) {
    return a;
  }
  if (n % 2 == 0) {
    matrix b = pow(a, n / 2);
    return mul(b, b);
  } else {
    return mul(a, pow(a, n - 1));
  }
}

int main() {
  long long n;
  while (fin >> n) {
    if (n == (long long) 0) {
      return 0;
    }
    if (n == 1) {
      fout << 5 << "\n";
      continue;
    }
    matrix a = {
        {1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1},
        {1, 1, 0, 1, 0},
        {1, 1, 0, 1, 0}
    };
    matrix b = {
        {1},
        {1},
        {1},
        {1},
        {1}
    };
    b = mul(pow(a, n - 1), b);
    int answer = 0;
    for (int i = 0; i < 5; i++) {
      answer = (answer + b[i][0]) % ((int) mod);
    }
    fout << answer << "\n";
  }
  return 0;
}
