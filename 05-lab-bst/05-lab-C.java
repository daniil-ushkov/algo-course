import java.util.Scanner;
import java.util.TreeSet;

public class Main {
    public static void main(String[] args) {
        try (Scanner scanner = new Scanner(System.in)) {
            int n = scanner.nextInt();
            int m = scanner.nextInt();
            Node root = null;
            for (int i = 1; i <= n; i++) {
                root = Node.pushBack(root, i);
            }
            for (int i = 0; i < m; i++) {
                int l = scanner.nextInt() - 1;
                int r = scanner.nextInt() - 1;
                Pair pair1 = Node.split(root, l);
                Pair pair2 = Node.split(pair1.right, r - l + 1);
                root = Node.merge(pair2.left, pair1.left);
                root = Node.merge(root, pair2.right);
            }
            Node.print(root);
        }
    }

}

class Pair {
    Node left;
    Node right;

    public Pair(Node left, Node right) {
        this.left = left;
        this.right = right;
    }
}

class Node {
    int value;
    double rang;
    int size;
    Node leftChild;
    Node rightChild;

    public Node(int value) {
        this.value = value;
        this.rang = Math.random();
        size = 1;
        leftChild = null;
        rightChild = null;
    }

    public static void print(Node tree) {
        if (tree != null) {
            print(tree.leftChild);
            System.out.print(" ");
            System.out.print(tree.value);
            System.out.print(" ");
            print(tree.rightChild);
        }
    }

    public static int size(Node tree) {
        if (tree != null) {
            return tree.size;
        }
        return 0;
    }

    public static void correct(Node tree) {
        if (tree != null) {
            tree.size = 1 + size(tree.leftChild) + size(tree.rightChild);
        }
    }

    public static Node merge(Node tree1, Node tree2) {
        if (tree1 == null) {
            return tree2;
        }
        if (tree2 == null) {
            return tree1;
        }
        if (tree1.rang > tree2.rang) {
            tree1.rightChild = merge(tree1.rightChild, tree2);
            correct(tree1);
            return tree1;
        } else {
            tree2.leftChild = merge(tree1, tree2.leftChild);
            correct(tree2);
            return tree2;
        }
    }

    public static Pair split(Node tree, int number) {
        if (tree == null) {
            return new Pair(null, null);
        }
        if (size(tree.leftChild) < number) {
            Pair pair = split(tree.rightChild, number - 1 - size(tree.leftChild));
            tree.rightChild = pair.left;
            correct(tree);
            pair.left = tree;
            return pair;
        } else {
            Pair pair = split(tree.leftChild, number);
            tree.leftChild = pair.right;
            correct(tree);
            pair.right = tree;
            return pair;
        }
    }

    public static Node pushBack(Node tree, int value) {
        tree = merge(tree, new Node(value));
        return tree;
    }
}