#include <iostream>
#include <vector>
#include <algorithm>
#include <deque>

using namespace std;

struct Node {
  int value, rang, size;
  Node *leftChild;
  Node *rightChild;

  Node(int value) {
    this->value = value;
    this->rang = rand();
    this->size = 1;
    leftChild = nullptr;
    rightChild = nullptr;
  }

  void setLeftChild(Node *leftChild) {
    this->leftChild = leftChild;
  }

  void setRightChild(Node *rightChild) {
    this->rightChild = rightChild;
  }
};

int size(Node* tree) {
  if (tree == nullptr) {
    return 0;
  }
  return tree->size;
}

void correct(Node* tree) {
  if (tree != nullptr) {
    tree->size = 1 + size(tree->leftChild) + size(tree->rightChild);
  }
}

Node *merge(Node *tree1, Node *tree2) {
  if (tree1 == nullptr) {
    return tree2;
  }
  if (tree2 == nullptr) {
    return tree1;
  }
  if (tree1->rang > tree2->rang) {
    tree1->setRightChild(merge(tree1->rightChild, tree2));
    correct(tree1);
    return tree1;
  } else {
    tree2->setLeftChild(merge(tree1, tree2->leftChild));
    correct(tree2);
    return tree2;
  }
}

pair<Node*, Node*> split(Node* tree, int number) {
  if (tree == nullptr) {
    return make_pair(nullptr, nullptr);
  }
  if (size(tree->leftChild) < number) {
    auto pair = split(tree->rightChild, number - 1 - size(tree->leftChild));
    tree->rightChild = pair.first;
    correct(tree);
    pair.first = tree;
    return pair;
  } else {
    auto pair = split(tree->leftChild, number);
    tree->leftChild = pair.second;
    correct(tree);
    pair.second = tree;
    return pair;
  }
}

Node* implace(Node* tree, int index, int value) {
  auto pair1 = split(tree, index - 1);
  auto pair2 = split(pair1.second, 1);
  pair2.first->value = value;
  pair1.second = merge(pair2.first, pair2.second);
  return merge(pair1.first, pair1.second);
}
vector<int> stack;
void print(Node* tree) {
  if (tree != nullptr) {
    print(tree->leftChild);
    stack.push_back(tree->value);
    print(tree->rightChild);
  }
}

class DSU {
 public:
  DSU(int number) {
    n = number;
    p = new int[n];
    for (int i = 0; i < n; i++) {
      p[i] = i;
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
    p[a] = b;
  }

 private:
  int n;
  int *p;
};

int main() {
  int n, m;
  cin >> n >> m;
  DSU dsu(2*max(n, m) + 10);
  Node* root = nullptr;
  for (int i = 0; i < 2*max(n, m) + 10; i++) {
    root = merge(root, new Node(0));
  }
  vector<bool> used(2*max(n, m) + 10, 0);
  for (int i = 1; i <= n; i++) {
    int l;
    cin >> l;
    if (!used[l]) {
      root = implace(root, l, i);
      used[l] = 1;
      if (used[l - 1]) {
        dsu.merge(l - 1, l);
      }
      if (used[l + 1]) {
        dsu.merge(l, l + 1);
      }
    } else {
      int next = dsu.find_leader(l) + 1;
      used[next] = 1;
      dsu.merge(l, next);
      if (used[next + 1]) {
        dsu.merge(next, next + 1);
      }
      auto pair1 = split(root, l - 1);
      auto pair2 = split(pair1.second, next - l);
      auto pair3 = split(pair2.second, 1);
      Node* node = pair3.first;
      node->value = i;
      pair2.second = merge(pair2.first, pair3.second);
      pair1.second = merge(node, pair2.second);
      root = merge(pair1.first, pair1.second);
    }
  }
  print(root);
  int cnt = 2*max(n, m) + 9;
  while (stack[cnt] == 0) {
    cnt--;
  }
  cout << cnt + 1 << "\n";
  for (int i = 0; i <= cnt; i++) {
    cout << stack[i] << " ";
  }
  return 0;
}
