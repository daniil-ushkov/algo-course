#include <fstream>
#include <vector>

using namespace std;

ifstream fin("matrix.in");
ofstream fout("matrix.out");

const int INF = 1e9;

int n;
vector<pair<int, int>> m;
vector<vector<int>> dp;
vector<vector<int>> sep;

string form(int i, int j) {
  if (i == j) {
    return "A";
  } else {
    return "(" + form(i, sep[i][j]) + form(sep[i][j] + 1, j) + ")";
  }
}

int main() {
  fin >> n;
  m.resize(n);
  dp.resize(n, vector<int>(n));
  sep.resize(n, vector<int>(n));
  for (int i = 0; i < n; i++) {
    fin >> m[i].first >> m[i].second;
  }

  for (int i = n - 1; i >= 0; i--) {
    for (int j = 0; j < n; j++) {
      if (i == j) {
        dp[i][j] = 0;
        continue;
      }
      dp[i][j] = INF;
      for (int k = i; k < j; k++) {
        if (dp[i][j] > dp[i][k] + dp[k + 1][j] + m[i].first*m[k].second*m[j].second) {
          dp[i][j] = dp[i][k] + dp[k + 1][j] + m[i].first*m[k].second*m[j].second;
          sep[i][j] = k;
        }
      }
    }
  }
  fout << form(0, n - 1);
  return 0;
}
