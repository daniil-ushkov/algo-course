#include <iostream>
#include <vector>

using namespace std;

const int INF = 1e9;

class Heap {
 public:
  Heap() {
    for (int i = 0; i < 100000; i++) {
      heap[i] = INF;
      pushIndex[i] = INF;
      pos[i] = INF;
    }
  }

  void siftUp(int i) {
    if (i == 0) {
      return;
    }
    if (heap[(i - 1) / 2] > heap[i]) {
      swapVertex((i - 1) / 2, i);
      siftUp((i - 1) / 2);
    }
  }

  void siftDown(int i) {
    if (heap[i] <= heap[2 * i + 1] && heap[i] <= heap[2 * i + 2]) {
      return;
    }
    if (heap[2 * i + 1] <= heap[2 * i + 2]) {
      swapVertex(2 * i + 1, i);
      siftDown(2 * i + 1);
    } else {
      swapVertex(2 * i + 2, i);
      siftDown(2 * i + 2);
    }
  }

  void push(int value) {
    heap[heapSize] = value;
    pushIndex[heapSize] = pushNumber;
    pos[pushNumber++] = heapSize;
    heapSize++;
    siftUp(heapSize - 1);
  }

  int extractMin() {
    pushNumber++;
    if (heapSize == 0) {
      return INF;
    }
    int m = heap[0];
    swapVertex(0, heapSize - 1);
    pushNumberOfExtracted = pushIndex[heapSize - 1];
    heap[heapSize - 1] = INF;
    pos[pushIndex[heapSize - 1]] = INF;
    pushIndex[heapSize - 1] = INF;
    heapSize--;
    siftDown(0);
    return m;
  }

  void decrease_key(int i, int value) {
    pushNumber++;
    if (pos[i] == INF) {
      return;
    }
    heap[pos[i]] = min(heap[pos[i]], value);
    siftUp(pos[i]);
  }

  int pushNumberOfExtracted = INF;

 private:
  void swapVertex(int i, int j) {
    swap(heap[i], heap[j]);
    swap(pushIndex[i], pushIndex[j]);
    swap(pos[pushIndex[i]], pos[pushIndex[j]]);
  }

  int heap[100000];
  int pushIndex[100000];
  int pos[100000];
  int heapSize = 0;
  int pushNumber = 0;
};

int main() {
  Heap h;
  string command;
  while (cin >> command) {
    if (command == "push") {
      int value;
      cin >> value;
      h.push(value);
    }
    if (command == "extract-min") {
      int m = h.extractMin();
      if (m == INF) {
        cout << "*" << "\n";
      } else {
        cout << m << " " << h.pushNumberOfExtracted + 1 << "\n";
      }
    }
    if (command == "decrease-key") {
      int x, v;
      cin >> x >> v;
      h.decrease_key(x - 1, v);
    }
  }
  return 0;
}
