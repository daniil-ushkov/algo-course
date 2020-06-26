#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <cmath>

#pragma GCC optimize("O3")
#define fastio ios_base::sync_with_stdio(false), cin.tie(0), cout.tie(0)

#define fin cin
#define fout cout

using namespace std;

const int INF = (int) 1e6;

int n, m, k;
vector<int> a;

struct range {
  int l, r;
  long long ans;
  vector<int> cnt;

  range() {
    l = 1;
    r = 1;
    ans = INF;
    cnt.resize((int) 1e6 + 1, 0);
  }
};

range ran;

void addl() {
  int val = a[--ran.l];

  ran.ans -= (long long) ran.cnt[val] * ran.cnt[val] * val;

  ran.cnt[val]++;

  ran.ans += (long long) ran.cnt[val] * ran.cnt[val] * val;
}

void addr() {
  int val = a[++ran.r];

  ran.ans -= (long long) ran.cnt[val] * ran.cnt[val] * val;
  ran.cnt[val]++;
  ran.ans += (long long) ran.cnt[val] * ran.cnt[val] * val;
}

void dell() {
  int val = a[ran.l++];

  ran.ans -= (long long) ran.cnt[val] * ran.cnt[val] * val;
  ran.cnt[val]--;
  ran.ans += (long long) ran.cnt[val] * ran.cnt[val] * val;
}

void delr() {
  int val = a[ran.r--];

  ran.ans -= (long long) ran.cnt[val] * ran.cnt[val] * val;
  ran.cnt[val]--;
  ran.ans += (long long) ran.cnt[val] * ran.cnt[val] * val;
}

int main() {
  fastio;
  fin >> n >> k;
  m = (int) sqrt(n);

  a.resize(n + 1);
  for (int i = 1; i <= n; i++)
    fin >> a[i];

  ran.cnt[a[1]] = 1;
  ran.ans = a[1];

  int l, r;
  vector<pair<pair<int, int>, int>> query;

  int i = 0;
  while (fin >> l >> r)
    query.push_back({{l, r}, i++});

  sort(query.begin(), query.end(),
       [](auto p1, auto p2) {
         return make_pair(p1.first.first / m, p1.first.second)
             < make_pair(p2.first.first / m, p2.first.second);
       });
  vector<pair<int, long long>> ans;
  for (auto q : query) {
    while (q.first.first < ran.l)
      addl();
    while (q.first.first > ran.l)
      dell();
    while (q.first.second > ran.r)
      addr();
    while (q.first.second < ran.r)
      delr();
    ans.emplace_back(q.second, ran.ans);
  }

  sort(ans.begin(), ans.end());
  for (auto el : ans)
    fout << el.second << endl;
  return 0;
}
