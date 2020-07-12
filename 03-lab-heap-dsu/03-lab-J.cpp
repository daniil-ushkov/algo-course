#include <iostream>
#include <vector>

using namespace std;

class DSU {
 public:
  DSU(int number) {
    n = number;
    p = new int[n];
    r = new int[n];
    readMes = new int[n];
    subTreeMes = new int[n];
    for (int i = 0; i < n; i++) {
      p[i] = i;
      r[i] = 0;
      readMes[i] = 0;
      subTreeMes[i] = 0;
    }
  }

  int find_leader(int x) {
    if (p[x] == x) {
      return x;
    }
    int oldParent = p[x];
    p[x] = find_leader(p[x]);
    if (oldParent != p[x]) {
      subTreeMes[x] += subTreeMes[oldParent];
    }
    return p[x];
  }

  int read(int x) {
    int leader = find_leader(x);
    int oldReadMes = readMes[x];
    if (x == leader) {
      readMes[x] = subTreeMes[x];
      return subTreeMes[x] - oldReadMes;
    } else {
      readMes[x] = subTreeMes[x] + subTreeMes[leader];
      return subTreeMes[x] + subTreeMes[leader] - oldReadMes;
    }
  }

  void merge(int a, int b) {
    a = find_leader(a);
    b = find_leader(b);
    if (a == b) {
      return;
    }
    if (r[a] > r[b]) {
      swap(a, b);
    }
    p[a] = b;
    subTreeMes[a] -= subTreeMes[b];
    if (r[a] == r[b]) {
      r[b]++;
    }
  }
  int* subTreeMes;

 private:
  int n;
  int* p;
  int* r;
  int* readMes;
};

int main() {
  int n, m;
  cin >> n >> m;
  DSU *dsu = new DSU(n);
  int zerg = 0;
  int p = 1000003;
  for (int i = 0; i < m; i++) {
    int type;
    cin >> type;
    if (type == 1) {
      int x;
      cin >> x;
      dsu->subTreeMes[dsu->find_leader((zerg + x) % n)] += 1;
      zerg = (30 * zerg + 239) % p;
    } else if (type == 2) {
      int x, y;
      cin >> x >> y;
      bool flag = (dsu->find_leader((zerg + x) % n) == dsu->find_leader((zerg + y) % n));
      dsu->merge((zerg + x) % n, (zerg + y) % n);
      if (!flag) {
        zerg = (13 * zerg + 11) % p;
      }
    } else if (type == 3) {
      int x;
      cin >> x;
      int ans = dsu->read((zerg + x) % n);
      cout << ans << "\n";
      long long l = ((long long) 100500 * zerg + ans);
      zerg = (int) (l % (long long) p);
    }
  }
  return 0;
}
