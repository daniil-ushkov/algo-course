#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

ifstream fin("salesman.in");
ofstream fout("salesman.out");

const long long INF = 200000000000;

int main() {
  int n, m;
  fin >> n >> m;
  vector<vector<long long>> w(n, vector<long long>(n));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      w[i][j] = INF;
    }
  }
  for (int i = 0; i < m; i++) {
    int from, to;
    long long weight;
    fin >> from >> to >> weight;
    w[from - 1][to - 1] = weight;
    w[to - 1][from - 1] = weight;
  }
  vector<vector<long long>> dp(n, vector<long long>(1 << n));
  for (int i = 0; i < n; i++) {
    dp[i][0] = 0;
  }
  for (int A = 1; A < (1 << n); A++) {
    for (int i = 0; i < n; i++) {
      dp[i][A] = INF;
      if ((A & (1 << i)) == 0) {
        for (int j = 0; j < n; j++) {
          if ((A & (1 << j)) != 0) {
            dp[i][A] = min(dp[i][A], dp[j][A ^ (1 << j)] + w[j][i]);
          }
        }
      }
    }
  }
  long long min_weight = INF;
  for (int i = 0; i < n; i++) {
    min_weight = min(min_weight, dp[i][((1 << n) - 1) ^ (1 << i)]);
  }
  if (min_weight == INF) {
    fout << -1;
    return 0;
  }
  fout << min_weight;
  return 0;
}
