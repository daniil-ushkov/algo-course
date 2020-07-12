#include <iostream>
#include <vector>

using namespace std;

const int INF = 1e9;

class Heap {
 public:
  Heap() {
    for (int i = 0; i < 1000000; i++) {
      heap[i] = INF;
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
    heapSize++;
    siftUp(heapSize - 1);
  }

  int extractMin() {
    if (heapSize == 0) {
      return INF;
    }
    int m = heap[0];
    swapVertex(0, heapSize - 1);
    heap[heapSize - 1] = INF;
    heapSize--;
    siftDown(0);
    return m;
  }
  int heapSize = 0;

 private:
  void swapVertex(int i, int j) {
    swap(heap[i], heap[j]);
  }
  int heap[1000000];
};

int main() {
  int n;
  cin >> n;
  vector<vector<int>> p(n);
  int* ans = new int[n];

  int place;
  int start;
  cin >> place;
  p[place - 1].push_back(0);
  start = place - 1;
  for (int i = 1; i < n; i++) {
    cin >> place;
    p[place - 1].push_back(i);
  }

  Heap h;
  for (int i = start; i < start + 2 * n; i++) {
    for (int car : p[i % n]) {
      h.push(car);
    }
    if (h.heapSize != 0) {
      ans[h.extractMin()] = i % n;
    }
  }
  for (int i = 0; i < n; i++) {
    cout << ans[i] + 1 << " ";
  }
  return 0;
}
