#include <fstream>
#include <cstdint>
#include <ctime>
#include <cstdlib>
#include <list>
#include <algorithm>

using namespace std;

ifstream cin("map.in");
ofstream cout("map.out");

struct hash_map {
  static const size_t M = 1e3;
  static const uint32_t P = 1e9 + 7;
  uint32_t A;
  uint32_t B;
  typedef list<pair<uint32_t, string>> bucket;
  bucket *elements[M];

  hash_map() {
    srand(time(NULL));
    A = rand();
    B = rand();
    for (auto &element : elements) {
      element = new bucket;
    }
  }

  size_t hash(uint32_t x) {
    return (A * x + B) % P % M;
  }

  void put(uint32_t k, string v) {
    if (!exists(k)) {
      elements[hash(k)]->push_back(make_pair(k, v));
    } else {
      erase(k);
      put(k, v);
    }
  }

  string get(uint32_t k) {
    bucket *b = elements[hash(k)];
    auto it = find_if(b->begin(),
                      b->end(),
                      [=](const pair<uint32_t, string>& el) { return el.first == k; });
    if (it != b->end()) {
      return it->second;
    } else {
      return "none";
    }
  }

  void erase(uint32_t k) {
    bucket *b = elements[hash(k)];
    auto it = find_if(b->begin(),
                      b->end(),
                      [=](pair<uint32_t, string> el) { return el.first == k; });
    if (it != b->end()) {
      b->erase(it);
    }
  }

  bool exists(uint32_t k) {
    bucket *b = elements[hash(k)];
    auto it = find_if(b->begin(),
                      b->end(),
                      [=](pair<uint32_t, string> el) { return el.first == k; });
    return it != b->end();
  }
};

uint32_t hash_fn(string s) {
  uint32_t h = 0;
  for (char c : s) {
    h = 29 * h + (c - 'a');
  }
  return h;
}

int main() {
  hash_map map;
  string com;
  while (cin >> com) {
    string x;
    cin >> x;
    if (com == "put") {
      string y;
      cin >> y;
      map.put(hash_fn(x), y);
    } else if (com == "delete") {
      map.erase(hash_fn(x));
    } else if (com == "get") {
      cout << map.get(hash_fn(x)) << "\n";
    }
  }
  return 0;
}
