#include <iostream>
#include <vector>

using namespace std;

const int INF = 1e9;
const long long INF_L = 1e18;
const int SIZE = 700000;

class Heap {
 public:
  Heap() {
    for (int i = 0; i < SIZE; i++) {
      heap[i] = -INF_L;
      pushIndex[i] = INF;
      position[i] = INF;
    }
  }

  void siftUp(int i) {
    if (i == 0) {
      return;
    }
    if (heap[(i - 1) / 2] < heap[i]) {
      swapElements((i - 1) / 2, i);
      siftUp((i - 1) / 2);
    }
  }

  void siftDown(int i) {
    if (heap[i] >= heap[2 * i + 1] && heap[i] >= heap[2 * i + 2]) {
      return;
    }
    if (heap[2 * i + 1] >= heap[2 * i + 2]) {
      swapElements(2 * i + 1, i);
      siftDown(2 * i + 1);
    } else {
      swapElements(2 * i + 2, i);
      siftDown(2 * i + 2);
    }
  }

  void push(long long value) {
    heap[heapSize] = value;
    pushIndex[heapSize] = pushNumber;
    position[pushNumber++] = heapSize;
    heapSize++;
    siftUp(heapSize - 1);
  }

  long long extractMax() {
    if (heapSize == 0) {
      return -INF_L;
    }
    long long m = heap[0];
    swapElements(0, heapSize - 1);
    heap[heapSize - 1] = -INF_L;
    position[pushIndex[heapSize - 1]] = INF;
    pushIndex[heapSize - 1] = INF;
    heapSize--;
    siftDown(0);
    return m;
  }

  void decreaseKey(int i, long long value) {
    if (position[i] == INF) {
      return;
    }
    heap[position[i]] = min(heap[position[i]], value);
    siftDown(position[i]);
  }

  long long top() {
    return heap[0];
  }
  int pushNumber = 0;

 private:
  void swapElements(int i, int j) {
    swap(heap[i], heap[j]);
    swap(pushIndex[i], pushIndex[j]);
    swap(position[pushIndex[i]], position[pushIndex[j]]);
  }

  long long heap[SIZE];
  int pushIndex[SIZE];
  int position[SIZE];
  int heapSize = 0;
};

int main() {
  int n, m;
  cin >> n >> m;
  vector<vector<pair<int, int>>> begin(n);
  vector<vector<int>> end(n);
  for (int i = 0; i < m; i++) {
    int l, r, value;
    cin >> l >> r >> value;
    begin[l - 1].push_back({value, r - 1});
  }
  Heap h;
  long long *answer = new long long[n];
  for (int i = 0; i < n; i++) {
    for (auto edge : begin[i]) {
      end[edge.second].push_back(h.pushNumber);
      h.push(edge.first);
    }
    answer[i] = h.top();
    if (answer[i] == -INF_L) {
      answer[i] = 1;
    }
    for (int edge : end[i]) {
      h.decreaseKey(edge, -INF_L);
    }
  }
  for (int i = 0; i < n; i++) {
    cout << answer[i] << " ";
  }
  return 0;
}
