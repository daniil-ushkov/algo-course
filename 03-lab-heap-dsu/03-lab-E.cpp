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
    theRightest = new int[n];
    theLeftest = new int[n];
    for (int i = 0; i < n; i++) {
      p[i] = i;
      r[i] = 0;
      theRightest[i] = i;
      theLeftest[i] = i;
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
    theRightest[b] = max(theRightest[b], theRightest[a]);
    theLeftest[b] = min(theLeftest[b], theLeftest[a]);
    if (r[a] == r[b]) {
      r[b]++;
    }
  }
  int *theRightest;
  int *theLeftest;

 private:
  int n;
  int *p;
  int *r;
};

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  int n, q;
  cin >> n >> q;
  DSU *dsu = new DSU(n);
  DSU *ranges = new DSU(n);
  while (q--) {
    int type, x, y;
    cin >> type >> x >> y;
    if (type == 1) {
      dsu->merge(x - 1, y - 1);
      if (ranges->theRightest[ranges->find_leader(x - 1)] + 1
        == ranges->theLeftest[ranges->find_leader(y - 1)]) {
        ranges->merge(x - 1, y - 1);
      }
    } else if (type == 2) {
      while (ranges->theRightest[ranges->find_leader(x - 1)] < y - 1) {
        dsu->merge(x - 1, ranges->theRightest[ranges->find_leader(x - 1)] + 1);
        ranges->merge(x - 1, ranges->theRightest[ranges->find_leader(x - 1)] + 1);
      }
    } else if (type == 3) {
      if (dsu->find_leader(x - 1) == dsu->find_leader(y - 1)) {
        cout << "YES\n";
      } else {
        cout << "NO\n";
      }
    }
  }
}
