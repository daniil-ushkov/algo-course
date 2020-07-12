#include <fstream>
#include <vector>

using namespace std;

ifstream cin("meetings.in");
ofstream cout("meetings.out");

const int INF = 1e9;

int n, k;
vector<pair<int, int>> edge;
vector<int> d;
vector<vector<int>> dp;
int ans_mask = 0;
int mask_size = 0;
int last = INF;

int get_size(int mask) {
  int cnt = 0;
  for (; mask; mask >>= 1) {
    cnt += mask & 1;
  }
  return cnt;
}

void calc() {
  dp.resize(n, vector<int>(1 << n));
  for (int i = 0; i < n; i++) {
    dp[i][0] = INF;
    if (edge[i].first <= k && k <= edge[i].second) {
      dp[i][0] = k + d[i];
      last = i;
    }
  }
  for (int A = 1; A < (1 << n); A++) {
    for (int i = 0; i < n; i++) {
      dp[i][A] = INF;
      if ((A & (1 << i)) == 0) {
        for (int j = 0; j < n; j++) {
          if ((A & (1 << j)) != 0
              && dp[j][A ^ (1 << j)] != INF
              && edge[i].first <= dp[j][A ^ (1 << j)]
              && dp[j][A ^ (1 << j)] <= edge[i].second) {
            dp[i][A] = dp[j][A ^ (1 << j)] + d[i];
            if (get_size(A) > mask_size) {
              ans_mask = A;
              mask_size = get_size(A);
              last = i;
            }
            break;
          }
        }
      }
    }
  }
}

vector<int> answer;

void restore_answer() {
  answer.push_back(last);
  if (mask_size == 0) {
    return;
  }
  for (int j = 0; j < n; j++) {
    if ((ans_mask & (1 << j)) != 0
        && dp[j][ans_mask ^ (1 << j)] != INF
        && edge[last].first <= dp[j][ans_mask ^ (1 << j)]
        && dp[j][ans_mask ^ (1 << j)] <= edge[last].second) {
      ans_mask ^= 1 << j;
      mask_size--;
      last = j;
      restore_answer();
    }
  }
}

int main() {
  cin >> n >> k;
  for (int i = 0; i < n; i++) {
    int l, r, change;
    cin >> l >> r >> change;
    edge.push_back({l, r});
    d.push_back(change);
  }
  calc();
  if (last == INF) {
    cout << 0;
    return 0;
  }
  cout << mask_size + 1 << "\n";
  restore_answer();
  for (int i = answer.size() - 1; i >= 0; i--) {
    cout << answer[i] + 1 << " ";
  }
  return 0;
}
