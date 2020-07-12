#include <fstream>
#include <cstdint>
#include <ctime>
#include <cstdlib>
#include <list>
#include <algorithm>

using namespace std;

ifstream cin("multimap.in");
ofstream cout("multimap.out");

uint32_t str_to_num(string s) {
  uint32_t h = 0;
  for (char c : s) {
    h = 29 * h + (c - 'a');
  }
  return h;
}

struct hash_set {
  struct entry {
    string value;
    entry *prev;
    entry *next;
    explicit entry(const string &k) {
      value = k;
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

  static const size_t M = 100;
  static const uint32_t P = 1e9 + 7;
  uint32_t A;
  uint32_t B;

  typedef list<entry *> bucket;
  bucket *elements[M];
  size_t size = 0;
  entry* last = nullptr;

  hash_set() {
    srand(time(NULL));
    A = rand();
    B = rand();
    for (size_t i = 0; i < M; i++) {
      elements[i] = new bucket;
    }
  }

  size_t hash(const string& x) {
    uint32_t h = str_to_num(x);
    return (A * h + B) % P % M;
  }
  bucket *find_bucket(const string &k) {
    return elements[hash(k)];
  }
  static _List_iterator<entry *> find_it(bucket *b, const string &k) {
    _List_iterator<entry *> it = find_if(b->begin(),
                                         b->end(),
                                         [=](const entry *el) { return el->value == k; });
    return it;
  }

  void insert(const string& x) {
    if (!exists(x)) {
      auto new_entry = new entry(x);
      entry::link(last, new_entry);
      last = new_entry;
      elements[hash(x)]->push_back(new_entry);
      size++;
    }
  }

  void erase(const string& x) {
    if (exists(x)) {
      bucket *b = find_bucket(x);
      auto it = find_it(b, x);
      entry::link((*it)->prev, (*it)->next);
      if (*it == last) {
        last = (*it)->prev;
      }
      b->erase(it);
      size--;
    }
  }

  bool exists(const string& x) {
    bucket *b = find_bucket(x);
    return b->end() != find_it(b, x);
  }
};

struct hash_multimap {
  struct entry {
    string key;
    hash_set *value;

    entry(const string &k) {
      key = k;
      value = new hash_set;
    }
  };

  typedef list<entry *> bucket;
  static const size_t M = 1e6;
  static const uint32_t P = 1e9 + 7;
  uint32_t A;
  uint32_t B;
  bucket *elements[M];

  hash_multimap() {
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

  void put(const string& k, const string& v) {
    bucket *b = find_bucket(k);
    auto it = find_it(b, k);
    if (it != b->end()) {
      (*it)->value->insert(v);
    } else {
      auto new_entry = new entry(k);
      new_entry->value->insert(v);
      b->push_back(new_entry);
    }
  }

  hash_set *get(const string& k) {
    bucket *b = find_bucket(k);
    auto it = find_it(b, k);
    if (it != b->end()) {
      return (*it)->value;
    } else {
      return nullptr;
    }
  }

  void erase(const string& k, const string& v) {
    bucket *b = find_bucket(k);
    auto it = find_it(b, k);
    if (it != b->end()) {
      (*it)->value->erase(v);
    }
  }

  void eraseAll(const string& k) {
    bucket *b = find_bucket(k);
    auto it = find_it(b, k);
    if (it != b->end()) {
      b->erase(it);
    }
  }
};

int main() {
  hash_multimap map;
  string com;
  while (cin >> com) {
    string x;
    cin >> x;
    if (com == "put") {
      string y;
      cin >> y;
      map.put(x, y);
    } else if (com == "delete") {
      string y;
      cin >> y;
      map.erase(x, y);
    } else if (com == "deleteall") {
      map.eraseAll(x);
    } else if (com == "get") {
      hash_set *s = map.get(x);
      if (s != nullptr) {
        cout << s->size << " ";
        hash_set::entry *curr = s->last;
        while (curr != nullptr) {
          cout << curr->value << " ";
          curr = curr->prev;
        }
        cout << "\n";
      } else {
        cout << 0 << "\n";
      }
    }
  }
  return 0;
}
