#include <fstream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

ifstream fin("perm.in");
ofstream fout("perm.out");

int n, k;
long long m;
vector<vector<bool>> g;

vector<vector<long long>> dp;

vector<int> prefix;
int p = 0;
int mask = 0;

map<int, int> index_by_object;
map<int, int> object_by_index;

void analyse() {
  dp.resize(n, vector<long long>(1 << n));
  for (int i = 0; i < n; i++) {
    dp[i][0] = 1;
  }
  for (int A = 1; A < (1 << n); A++) {
    for (int i = 0; i < n; i++) {
      dp[i][A] = 0;
      if ((A & (1 << i)) == 0) {
        for (int j = 0; j < n; j++) {
          if ((A & (1 << j)) != 0 && g[j][i]) {
            dp[i][A] += dp[j][A ^ (1 << j)];
          }
        }
      }
    }
  }
}

int gcd(int a, int b) {
  if (a == 0) {
    return b;
  }
  return gcd(b % a, a);
}

void find() {
  if (p == n) {
    if (m == 0) {
      for (int i = 0; i < p; i++) {
        fout << prefix[i] << " ";
      }
    }
    m--;
  }
  if (m < 0) {
    return;
  }
  for (int i = 0; i < n; i++) {
    if (m < 0) {
      break;
    }
    if ((mask & (1 << i)) == 0 && (p == 0 || gcd(object_by_index[i], prefix[p - 1]) >= k)) {
      prefix[p++] = object_by_index[i];
      if (m > dp[i][((1 << n) - 1 - mask) ^ (1 << i)]) {
        m -= dp[i][((1 << n) - 1 - mask) ^ (1 << i)];
      } else {
        mask = mask ^ (1 << i);
        find();
        mask = mask ^ (1 << i);
      }
      p--;
    }
  }
}

int main() {
  fin >> n >> m >> k;
  m--;
  int object;
  vector<int> objects;
  for (int i = 0; i < n; i++) {
    fin >> object;
    objects.push_back(object);
  }
  sort(objects.begin(), objects.end());
  for (int i = 0; i < n; i++) {
    index_by_object.insert({objects[i], i});
    object_by_index.insert({i, objects[i]});
  }
  g.resize(n, vector<bool>(n));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (gcd(object_by_index[i], object_by_index[j]) >= k) {
        g[i][j] = true;
        g[j][i] = true;
      }
    }
  }
  analyse();
  prefix.resize(n);
  long long sum = 0;
  for (int i = 0; i < n; i++) {
    sum += dp[i][((1 << n) - 1) ^ (1 << i)];
  }
  if (m + 1 > sum) {
    fout << -1;
    return 0;
  }
  find();
  return 0;
}
