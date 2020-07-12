#include <fstream>
#include <vector>

using namespace std;

ifstream fin("lis.in");
ofstream fout("lis.out");

const int INF = 2*1e9;

int binary_search(vector<int> &a, vector<int> &dp2, int k, int value) {
  int l = 0;
  int r = k + 1;
  while (r - l > 1) {
    int m = (l + r) / 2;
    if (a[dp2[m]] < value) {
      l = m;
    } else {
      r = m;
    }
  }
  return l;
}

int main() {
  int n;
  fin >> n;
  if (n == 0) {
    return 0;
  }
  vector<int> a(n);
  vector<int> dp1(n);
  vector<int> dp2(n + 1);
  vector<int> p(n);
  for (int i = 0; i < n; i++) {
    fin >> a[i];
    dp2[i] = INF;
  }
  int end = 0;
  int k = 0;
  for (int i = 0; i < n; i++) {
    int j = binary_search(a, dp2, k, a[i]);
    if (j == 0) {
      dp1[i] = 1;
      if (dp2[1] == INF || a[dp2[1]] > a[i]) {
        dp2[1] = i;
      }
      p[i] = -1;
      k = max(k, 1);
    } else {
      dp1[i] = j + 1;
      if (dp2[dp1[i]] == INF || a[dp2[dp1[i]]] > a[i]) {
        dp2[dp1[i]] = i;
      }
      p[i] = dp2[j];
      k = max(k, dp1[i]);
    }
    if (dp1[end] < dp1[i]) {
      end = i;
    }
  }
  vector<int> stack;
  stack.push_back(a[end]);
  while (p[end] != -1) {
    stack.push_back(a[end = p[end]]);
  }
  fout << stack.size() << "\n";
  for (int i = (int) stack.size() - 1; i >= 0; i--) {
    fout << stack[i] << " ";
  }
  return 0;
}
