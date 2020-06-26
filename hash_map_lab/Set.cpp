#include <fstream>
#include <cstdint>
#include <ctime>
#include <cstdlib>
#include <list>
#include <algorithm>

using namespace std;

ifstream cin("set.in");
ofstream cout("set.out");

struct hash_set {
  static const size_t M = 1e6;
  static const uint32_t P = 1e9 + 7;
  uint32_t A;
  uint32_t B;
  list<uint32_t> *elements[M];

  hash_set() {
    srand(time(NULL));
    A = rand();
    B = rand();
    for (size_t i = 0; i < M; i++) {
      elements[i] = new list<uint32_t>;
    }
  }

  size_t hash(uint32_t x) {
    return (A * x + B) % P % M;
  }

  void insert(uint32_t x) {
    if (!exists(x)) {
      elements[hash(x)]->push_back(x);
    }
  }

  void erase(uint32_t x) {
    if (exists(x)) {
      list<uint32_t> *list = elements[hash(x)];
      list->erase(find(list->begin(), list->end(), x));
    }
  }

  bool exists(uint32_t x) {
    list<uint32_t> *list = elements[hash(x)];
    return list->end() != find(list->begin(), list->end(), x);
  }
};

int main() {
  hash_set set;
  string com;
  while (cin >> com) {
    uint32_t x;
    cin >> x;
    if (com == "insert") {
      set.insert(x);
    } else if (com == "delete") {
      set.erase(x);
    } else if (com == "exists") {
      cout << (set.exists(x) ? "true" : "false") << "\n";
    }
  }
  return 0;
}
