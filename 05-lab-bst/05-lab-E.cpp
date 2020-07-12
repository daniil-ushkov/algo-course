#include <iostream>
#include <vector>
#include <algorithm>
#include <deque>

using namespace std;

int n;

struct Node {
  int x, y;
  int index;
  Node *leftChild;
  Node *rightChild;
  Node *parent;

  Node(int x, int y, int index) {
    this->x = x;
    this->y = y;
    this->index = index;
    leftChild = nullptr;
    rightChild = nullptr;
    parent = nullptr;
  }

  void setLeftChild(Node *leftChild) {
    this->leftChild = leftChild;
    if (this->leftChild != nullptr) {
      this->leftChild->parent = this;
    }
  }

  void setRightChild(Node *rightChild) {
    this->rightChild = rightChild;
    if (this->rightChild != nullptr) {
      this->rightChild->parent = this;
    }
  }
};

Node *merge(Node *tree1, Node *tree2) {
  if (tree1 == nullptr) {
    return tree2;
  }
  if (tree2 == nullptr) {
    return tree1;
  }
  if (tree1->y < tree2->y) {
    tree1->setRightChild(merge(tree1->rightChild, tree2));
    return tree1;
  } else {
    tree2->setLeftChild(merge(tree1, tree2->leftChild));
    return tree2;
  }
}

int main() {
  cin >> n;
  vector<Node *> inputList;
  vector<Node *> list;
  for (int i = 0; i < n; i++) {
    int a, b;
    cin >> a >> b;
    inputList.push_back(new Node(a, b, i));
    list.push_back(inputList.back());
  }
  sort(list.begin(), list.end(),
       [](Node *n1, Node *n2) {
         if (n1->x == n2->x) {
           return n1->y < n2->y;
         }
         return n1->x < n2->x;
       });
  deque<Node *> deque1;
  for (Node *node : list) {
    deque1.push_back(node);
  }
  while (deque1.size() > 1) {
    int size = deque1.size();
    bool flag = size % 2 == 1;
    for (int i = 0; i < size / 2; i++) {
      Node *first = deque1.front();
      deque1.pop_front();
      Node *second = deque1.front();
      deque1.pop_front();
      deque1.push_back(merge(first, second));
    }
    if (flag) {
      deque1.push_back(deque1.front());
      deque1.pop_front();
    }
  }
  cout << "YES" << "\n";
  for (Node *node : inputList) {
    if (node->parent == nullptr) {
      cout << 0 << " ";
    } else {
      cout << node->parent->index + 1 << " ";
    }
    if (node->leftChild == nullptr) {
      cout << 0 << " ";
    } else {
      cout << node->leftChild->index + 1 << " ";
    }
    if (node->rightChild == nullptr) {
      cout << 0 << " ";
    } else {
      cout << node->rightChild->index + 1;
    }
    cout << "\n";
  }
  return 0;
}
