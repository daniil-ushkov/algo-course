#include <fstream>
#include <cstdint>
#include <ctime>
#include <cstdlib>
#include <list>
#include <algorithm>

using namespace std;

ifstream cin("linkedmap.in");
ofstream cout("linkedmap.out");

uint32_t str_to_num(string s) {
  uint32_t h = 0;
  for (char c : s) {
    h = 29 * h + (c - 'a');
  }
  return h;
}

struct hash_map {
  struct entry {
    string key;
    string value;
    entry *prev;
    entry *next;

    entry(const string &k, const string &v) {
      key = k;
      value = v;
      prev = nullptr;
      next = nullptr;
    }

    static void link(entry *en1, entry *en2) {
      if (en1 != nullptr) {
        en1->next = en2;
      }
      if (en2 != nullptr) {
        en2->prev = en1;
      }
    }
  };

  entry *last = nullptr;
  static const size_t M = 1e6;
  static const uint32_t P = 1e9 + 7;
  uint32_t A;
  uint32_t B;
  typedef list<entry *> bucket;
  bucket *elements[M];

  hash_map() {
    srand(time(NULL));
    A = rand();
    B = rand();
    for (auto &element : elements) {
      element = new bucket;
    }
  }
  size_t hash(const string &x) {
    uint32_t h = str_to_num(x);
    return (A * h + B) % P % M;
  }
  bucket *find_bucket(const string &k) {
    return elements[hash(k)];
  }
  static _List_iterator<entry *> find_it(bucket *b, const string &k) {
    _List_iterator<entry *> it = find_if(b->begin(),
                                         b->end(),
                                         [=](const entry *el) { return el->key == k; });
    return it;
  }

  void put(const string &k, const string &v) {
    bucket *b = find_bucket(k);
    auto it = find_it(b, k);
    if (it != b->end()) {
      (*it)->value = v;
    } else {
      auto new_entry = new entry(k, v);
      entry::link(last, new_entry);
      last = new_entry;
      b->push_back(new_entry);
    }
  }

  string get(const string &k) {
    bucket *b = find_bucket(k);
    auto it = find_it(b, k);
    if (it != b->end()) {
      return (*it)->value;
    } else {
      return "none";
    }
  }

  void erase(const string &k) {
    bucket *b = find_bucket(k);
    auto it = find_it(b, k);
    if (it != b->end()) {
      entry::link((*it)->prev, (*it)->next);
      if (*it == last) {
        last = (*it)->prev;
      }
      b->erase(it);
    }
  }

  string next(const string &k) {
    bucket *b = find_bucket(k);
    auto it = find_it(b, k);
    if (it != b->end() && (*it)->next != nullptr) {
      return (*it)->next->value;
    } else {
      return "none";
    }
  }

  string prev(const string &k) {
    bucket *b = find_bucket(k);
    auto it = find_it(b, k);
    if (it != b->end() && (*it)->prev != nullptr) {
      return (*it)->prev->value;
    } else {
      return "none";
    }
  }
};

int main() {
  hash_map map;
  string com;
  while (cin >> com) {
    string x;
    cin >> x;
    if (com == "put") {
      string y;
      cin >> y;
      map.put(x, y);
    } else if (com == "delete") {
      map.erase(x);
    } else if (com == "get") {
      cout << map.get(x) << "\n";
    } else if (com == "next") {
      cout << map.next(x) << "\n";
    } else if (com == "prev") {
      cout << map.prev(x) << "\n";
    }
  }
  return 0;
}
