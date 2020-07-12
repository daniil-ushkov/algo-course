import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        try (Scanner scnanner = new Scanner(System.in)) {
            BST bst = new BST();
            while (scnanner.hasNext()) {
                String command = scnanner.next();
                if (command.equals("insert")) {
                    bst.insert(scnanner.nextInt());
                }
                if (command.equals("delete")) {
                    bst.delete(scnanner.nextInt());
                }
                if (command.equals("exists")) {
                    System.out.println(bst.exists(scnanner.nextInt()));
                }
                if (command.equals("next")) {
                    Node node = bst.next(scnanner.nextInt());
                    if (node == null) {
                        System.out.println("none");
                    } else {
                        System.out.println(node.value);
                    }
                }
                if (command.equals("prev")) {
                    Node node = bst.prev(scnanner.nextInt());
                    if (node == null) {
                        System.out.println("none");
                    } else {
                        System.out.println(node.value);
                    }
                }
            }
        }
    }
}

class BST {
    Node root;
    int size;

    public BST() {
        root = null;
        size = 0;
    }

    public void insert(int value) {
        if (this.exists(value)) {
            return;
        }
        if (root == null) {
            root = new Node(value);
        } else {
            root = root.insert(value);
        }
        size++;
    }

    public void delete(int value) {
        if (root != null) {
            root = root.delete(value);
        }
        size--;
    }

    public boolean exists(int value) {
        if (root == null) {
            return false;
        }
        return root.find(value) != null;
    }

    public Node next(int value) {
        if (root == null) {
            return null;
        }
        Node node = root.find(value);
        if (node == null) {
            this.insert(value);
            node = this.next(value);
            this.delete(value);
            return node;
        }
        return node.next();
    }

    public Node prev(int value) {
        if (root == null) {
            return null;
        }
        Node node = root.find(value);
        if (node == null) {
            this.insert(value);
            node = this.prev(value);
            this.delete(value);
            return node;
        }
        return node.prev();
    }
}

class Node {
    int value;
    int height;
    Node parent;
    Node leftChild, rightChild;
    static Node lastDeleted;

    public Node(int value) {
        this.value = value;
        this.height = 0;
        leftChild = null;
        rightChild = null;
        lastDeleted = null;
    }

    public void setLeftChild(Node leftChild) {
        this.leftChild = leftChild;
        if (leftChild != null) {
            leftChild.parent = this;
        }
    }

    public void setRightChild(Node rightChild) {
        this.rightChild = rightChild;
        if (rightChild != null) {
            rightChild.parent = this;
        }
    }

    public boolean isItRoot() {
        return parent == null;
    }

    public boolean isItLeftChild() {
        if (parent == null) {
            return false;
        }
        return this.parent.leftChild == this;
    }

    public Node min() {
        Node currentNode = this;
        while (currentNode.leftChild != null) {
            currentNode = currentNode.leftChild;
        }
        return currentNode;
    }

    public Node max() {
        Node currentNode = this;
        while (currentNode.rightChild != null) {
            currentNode = currentNode.rightChild;
        }
        return currentNode;
    }

    public Node insert(int value) {
        if (value < this.value) {
            if (this.leftChild != null) {
                return this.leftChild.insert(value);
            } else {
                this.setLeftChild(new Node(value));
                return this.correct();
            }
        } else {
            if (this.rightChild != null) {
                return this.rightChild.insert(value);
            } else {
                this.setRightChild(new Node(value));
                return this.correct();
            }
        }
    }

    public Node find(int value) {
        Node currentNode = this;
        while (currentNode.value != value) {
            if (value < currentNode.value) {
                currentNode = currentNode.leftChild;
            } else {
                currentNode = currentNode.rightChild;
            }
            if (currentNode == null) {
                return null;
            }
        }
        return currentNode;
    }

    public Node next() {
        if (this.rightChild != null) {
            return this.rightChild.min();
        }
        Node currentNode = this;
        while (!currentNode.isItLeftChild() && !currentNode.isItRoot()) {
            currentNode = currentNode.parent;
        }
        return currentNode.parent;
    }

    public Node prev() {
        if (this.leftChild != null) {
            return this.leftChild.max();
        }
        Node currentNode = this;
        while (currentNode.isItLeftChild() && !currentNode.isItRoot()) {
            currentNode = currentNode.parent;
        }
        return currentNode.parent;
    }

    public Node delete(int value) {
        Node node = this.find(value);
        if (node == null) {
            return this;
        }
        if (node.isItRoot()) {
            Node root = node.delete();
            if (lastDeleted.parent != null) {
                root = lastDeleted.parent.correct();
            }
            return root;
        }
        if (node.isItLeftChild()) {
            node.parent.setLeftChild(node.delete());
        } else {
            node.parent.setRightChild(node.delete());
        }
        if (lastDeleted.parent != null) {
            return lastDeleted.parent.correct();
        }
        return this;
    }

    public Node delete() {
        if (this.leftChild == null && this.rightChild == null) {
            lastDeleted = this;
            return null;
        }
        if (this.leftChild != null && this.rightChild == null) {
            this.leftChild.parent = null;
            lastDeleted = this;
            return this.leftChild;
        }
        if (this.leftChild == null && this.rightChild != null) {
            this.rightChild.parent = null;
            lastDeleted = this;
            return this.rightChild;
        }
        Node next = this.rightChild.min();
        this.value = next.value;
        if (next.isItLeftChild()) {
            next.parent.setLeftChild(next.delete());
        } else {
            next.parent.setRightChild(next.delete());
        }
        return this;
    }

    public static void correctHeight(Node node) {
        if (node == null) {
            return;
        }
        node.height = 1 + Math.max(getHeight(node.leftChild), getHeight(node.rightChild));
    }

    public static int getHeight(Node node) {
        if (node == null) {
            return -1;
        }
        return node.height;
    }

    public void rotate() {
        if (this.isItRoot()) {
            return;
        }
        if (this.isItLeftChild()) {
            Node grandParent = this.parent.parent;
            Node parent = this.parent;
            Node child = this.rightChild;
            if (grandParent == null) {
                this.parent = null;
            } else {
                if (parent.isItLeftChild()) {
                    grandParent.setLeftChild(this);
                } else {
                    grandParent.setRightChild(this);
                }
            }
            this.setRightChild(parent);
            parent.setLeftChild(child);
            correctHeight(parent);
            correctHeight(this);
            correctHeight(grandParent);
        } else {
            Node grandParent = this.parent.parent;
            Node parent = this.parent;
            Node child = this.leftChild;
            if (grandParent == null) {
                this.parent = null;
            } else {
                if (parent.isItLeftChild()) {
                    grandParent.setLeftChild(this);
                } else {
                    grandParent.setRightChild(this);
                }
            }
            this.setLeftChild(parent);
            parent.setRightChild(child);
            correctHeight(parent);
            correctHeight(this);
            correctHeight(grandParent);
        }


    }

    public Node correct() {
        correctHeight(this);
        Node grandParent = this.parent;
        Node leftChild = this.leftChild;
        Node rightChild = this.rightChild;
        Node root = this;
        if (Math.abs(getHeight(leftChild) - getHeight(rightChild)) > 1) {
            int height = getHeight(this);
            if (getHeight(leftChild) > getHeight(rightChild)) {
                if (getHeight(leftChild.leftChild) == height - 2) {
                    leftChild.rotate();
                    root = leftChild;
                } else {
                    Node rightChildOfLeftChild = leftChild.rightChild;
                    rightChildOfLeftChild.rotate();
                    rightChildOfLeftChild.rotate();
                    root = rightChildOfLeftChild;
                }
            } else {
                if (getHeight(rightChild.rightChild) == height - 2) {
                    rightChild.rotate();
                    root = rightChild;
                } else {
                    Node leftChildOfRightChild = rightChild.leftChild;
                    leftChildOfRightChild.rotate();
                    leftChildOfRightChild.rotate();
                    root = leftChildOfRightChild;
                }
            }
        }
        if (grandParent != null) {
            return grandParent.correct();
        } else {
            return root;
        }
    }
}