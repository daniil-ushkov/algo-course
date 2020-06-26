#include <iostream>
#include <cstdint>
#include <bitset>

using namespace std;

int main() {
  uint32_t k;
  cin >> k;
  string w1 = "Aa";
  string w2 = "BB";
  for (size_t i = 0; i < k; i++) {
    for (size_t j = 0; j < 10; j++) {
      if ((i >> j) % 2 == 1) {
        cout << w1;
      } else {
        cout << w2;
      }
    }
    cout << "\n";
  }
}
