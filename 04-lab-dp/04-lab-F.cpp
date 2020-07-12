#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int INF = 1e9;

int dist(string reg, string s) {
  int n = reg.length();
  int m = s.length();
  vector<int> dp(m + 1);
  for (int i = 0; i <= m; i++) {
    dp[i] = i;
  }
  int stars = 0;
  for (int i = 1; i <= n; i++) {
    if (reg[i - 1] == '*') {
      stars++;
    }
    int prev = dp[0];
    dp[0] = i - stars;
    for (int j = 1; j <= m; j++) {
      if (reg[i - 1] == s[j - 1] || reg[i - 1] == '?') {
        swap(prev, dp[j]);
      } else {
        int old_prev = prev;
        prev = dp[j];
        dp[j] = min(min(dp[j], dp[j - 1]), old_prev) + 1;
        if (reg[i - 1] == '*') {
          dp[j] = min(dp[j], min(dp[j - 1], prev));
        }
      }
    }
  }
  return dp[m];
}

int main() {
  string reg, s;
  cin >> reg >> s;
  if (dist(reg, s) == 0) {
    cout << "YES";
  } else {
    cout << "NO";
  }
  return 0;
}
