#include <fstream>
#include <vector>

using namespace std;

ifstream cin("bridge.in");
ofstream cout("bridge.out");

int x, a, y, b, l;

vector<vector<int>> dp;
bool check(int w) {
  for (int i = 0; i <= x; i++) {
    for (int j = 0; j <= y; j++) {
      dp[i][j] = 0;
      for (int k = 0; a * (k - 1) <= w; k++) {
        int s = k;
        int t = (max(0, w - a * k + b - 1)) / b;
        if (s <= i && t <= j) {
          dp[i][j] = max(dp[i][j], dp[i - s][j - t] + 1);
        }
      }
    }
  }
  return dp[x][y] >= l;
}


int main() {
  cin >> x >> a >> y >> b >> l;
  dp.resize(x + 1, vector<int>(y + 1));
  int left = 0, right = x*a + y*b + 1;
  while (right - left > 1) {
    int mid = (left + right) / 2;
    if (check(mid)) {
      left = mid;
    } else {
      right = mid;
    }
  }
  cout << left;
  return 0;
}
