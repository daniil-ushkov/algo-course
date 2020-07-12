#include <fstream>
#include <vector>

using namespace std;

ifstream fin("knapsack.in");
ofstream fout("knapsack.out");

int main() {
  int n, m;
  fin >> n >> m;
  int* c = new int[n];
  int* w = new int[n];
  for (int i = 0; i < n; i++) {
    fin >> w[i];
  }
  for (int i = 0; i < n; i++) {
    fin >> c[i];
  }
  vector<vector<int>> dp(n + 1, vector<int>(m + 1));
  for (int i = 0; i <= m; i++)  {
    dp[0][i] = 0;
  }
  for (int i = 1; i <= n; i++) {
    for (int j = m; j >= 0; j--) {
      dp[i][j] = dp[i - 1][j];
      if (j - w[i - 1] >= 0 && dp[i - 1][j - w[i - 1]] + c[i - 1] > dp[i][j]) {
        dp[i][j] = dp[i - 1][j - w[i - 1]] + c[i - 1];
      }
    }
  }
  vector<int> stack;
  int curr_m = m;
  for (int i = n - 1; i >= 0; i--) {
    if (curr_m - w[i] >= 0 && dp[i + 1][curr_m] == dp[i][curr_m - w[i]] + c[i]) {
      curr_m -= w[i];
      stack.push_back(i + 1);
    }
  }
  fout << stack.size() << "\n";
  for (int i = (int) stack.size() - 1; i >= 0; i--) {
    fout << stack[i] << " ";
  }
  return 0;
}
