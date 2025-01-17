#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

ifstream fin("levenshtein.in");
ofstream fout("levenshtein.out");

const int INF = 1e9;

int dist(string s, string t) {
  int n = s.length();
  int m = t.length();
  vector<vector<int>> dp(n + 1, vector<int>(m + 1));
  for (int i = 0; i <= n; i++) {
    dp[i][0] = i;
  }
  for (int i = 0; i <= m; i++) {
    dp[0][i] = i;
  }
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= m; j++) {
      if (s[i - 1] == t[j - 1]) {
        dp[i][j] = dp[i - 1][j - 1];
      } else {
        dp[i][j] = min(min(dp[i - 1][j], dp[i][j - 1]), dp[i - 1][j - 1]) + 1;
      }
    }
  }
  return dp[n][m];
}

int main() {
  string s, t;
  fin >> s >> t;
  fout << dist(s, t);
  return 0;
}
