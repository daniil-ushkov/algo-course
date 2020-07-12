#include <iostream>
#include <vector>

using namespace std;

const int mod = 1e9;

int main() {
  int n;
  cin >> n;
  vector<int> a(n);
  for (int i = 0; i < n; i++) {
    cin >> a[i];
  }
  vector<vector<int>> dp(n, vector<int>(n));
  for (int i = n - 1; i >= 0; i--) {
    for (int j = 0; j < n; j++) {
      if (i == j) {
        dp[i][j] = 1;
        continue;
      }
      if (i > j) {
        dp[i][j] = 0;
        continue;
      }
      if (a[i] == a[j]) {
        dp[i][j] = (dp[i][j - 1] + dp[i + 1][j] + 1) % mod;
      } else {
        dp[i][j] = ((dp[i][j - 1] + dp[i + 1][j]) % mod - dp[i + 1][j - 1] + mod) % mod;
      }
    }
  }
  cout << dp[0][n - 1];
  return 0;
}
