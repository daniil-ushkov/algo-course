#include <iostream>
#include <vector>

using namespace std;

const int INF = 1e9;

class DSU {
 public:
  DSU(int number) {
    n = number;
    p = new int[n];
    r = new int[n];
    anotherPart = new int[n];
    for (int i = 0; i < n; i++) {
      p[i] = i;
      r[i] = 0;
      anotherPart[i] = -1;
    }
  }

  int find_leader(int x) {
    if (p[x] == x) {
      return x;
    }
    return p[x] = find_leader(p[x]);
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
    if (r[a] == r[b]) {
      r[b]++;
    }
  }

  void mergeComponents(int a, int b) {
    a = find_leader(a);
    b = find_leader(b);
    int c = anotherPart[a];
    int d = anotherPart[b];
    if (d != -1) {
      merge(a, d);
    }
    if (c != -1) {
      merge(b, c);
    }
    anotherPart[find_leader(a)] = find_leader(b);
    anotherPart[find_leader(b)] = find_leader(a);
  }

 private:
  int n;
  int *p;
  int *r;
  int *anotherPart;
};

int main() {
  int n, m;
  cin >> n >> m;
  int shift = 0;
  DSU *dsu = new DSU(n);
  while (m--) {
    int type, x, y;
    cin >> type >> x >> y;
    if (type == 0) {
      dsu->mergeComponents((x + shift) % n, (y + shift) % n);
    } else {
      if (dsu->find_leader((x + shift) % n) == dsu->find_leader((y + shift) % n)) {
        cout << "YES\n";
        shift = (shift + 1) % n;
      } else {
        cout << "NO\n";
      }
    }
  }
  return 0;
}
