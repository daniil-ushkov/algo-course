#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

string s, t;

bool contains(int x, int y1, int y2) {
  for (int i = y1; i <= y2; i++) {
    if (s[x] == t[i]) {
      return true;
    }
  }
  return false;
}

vector<int> dp1;
void lcs(int x1, int x2, int y1, int y2) {
  int x_size = x2 - x1 + 1;
  int y_size = y2 - y1 + 1;
  for (int i = 0; i <= y_size; i++) {
    dp1[i] = 0;
  }
  for (int i = 1; i <= x_size; i++) {
    int prev = 0;
    for (int j = 1; j <= y_size; j++) {
      if (s[x1 + i - 1] == t[y1 + j - 1]) {
        swap(dp1[j], prev);
        dp1[j]++;
      } else {
        prev = dp1[j];
        dp1[j] = max(dp1[j], dp1[j - 1]);
      }
    }
  }
}

vector<int> dp2;
void lcsr(int x1, int x2, int y1, int y2) {
  int x_size = x2 - x1 + 1;
  int y_size = y2 - y1 + 1;
  for (int i = 0; i <= y_size; i++) {
    dp2[i] = 0;
  }
  for (int i = 1; i <= x_size; i++) {
    int prev = 0;
    for (int j = 1; j <= y_size; j++) {
      if (s[x2 - i + 1] == t[y2 - j + 1]) {
        swap(dp2[j], prev);
        dp2[j]++;
      } else {
        prev = dp2[j];
        dp2[j] = max(dp2[j], dp2[j - 1]);
      }
    }
  }
}

vector<char> ans;
void hirchberg(int x1, int x2, int y1, int y2) {
  int y_size = y2 - y1 + 1;
  if (x1 == x2) {
    if (contains(x1, y1, y2)) {
      ans.push_back(s[x1]);
    }
    return;
  }
  if (y2 < y1) {
    return;
  }
  int mid = (x1 + x2) / 2;
  lcs(x1, mid, y1, y2);
  lcsr(mid + 1, x2, y1, y2);
  int max_value = -1;
  int i = -1;
  for (int j = 0; j <= y_size; j++) {
    if (dp1[j] + dp2[y_size - j] > max_value) {
      i = y1 + j - 1;
      max_value = dp1[j] + dp2[y_size - j];
    }
  }
  hirchberg(x1, mid, y1, i);
  hirchberg(mid + 1, x2, i + 1, y2);
}

int main() {
  cin >> s >> t;
  dp1.resize(t.size() + 1);
  dp2.resize(t.size() + 1);
  hirchberg(0, s.size() - 1, 0, t.size() - 1);
  for (char el : ans) {
    cout << el;
  }
  return 0;
}
