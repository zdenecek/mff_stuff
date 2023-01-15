package com.mypackage.task3;

import java.util.Iterator;
public class BinaryTree<T extends Comparable> {

    private BinaryTreeNode<T> root;
    private int size;

    public BinaryTree() {
        root = null;
        size = 0;
    }

    public void add(T value) {

        BinaryTreeNode<T> current = root;

        if (root == null) {
            root = new BinaryTreeNode<T>(value);
            size++;
            return;
        }


        while (true) {
            if (current.value.compareTo(value) > 0) {
                if (current.left == null) {
                    current.left = new BinaryTreeNode<T>(value, current);
                    size++;
                    return;
                }
                current = current.left;
            } else {
                if (current.right == null) {
                    current.right = new BinaryTreeNode<T>(value, current);
                    size++;
                    return;
                }
                current = current.right;
            }
        }
    }

    public int size() {
        return size;
    }

    public Iterator<T> iterator() {
        return new BinaryTreeIterator<T>(root);
    }

    private class BinaryTreeIterator<T> implements Iterator {

        BinaryTreeNode<T> lastCompletedNode;
        BinaryTreeNode<T> current;

        BinaryTreeIterator(BinaryTreeNode<T> root) {
            current = root;
            while (current.left != null) current = current.left;

        }

        @Override
        public boolean hasNext() {
            return current != null;
        }

        @Override
        public T next() {
            lastCompletedNode = current;
            current = getNext();
            return lastCompletedNode.value;
        }

        private BinaryTreeNode<T> getNext() {

            if (current == null) return null;
            else if (lastCompletedNode == current.right) {
                return current.parent;
            } else if (current.right != null) {
                var local = current.right;
                while (local.left != null) local = local.left;
                return local;
            } else if (current.parent.left == current){
                return current.parent;
            } else {
                var local = current.parent;
                while (local.parent != null && local.parent.right == local) local = local.parent;
                return local.parent;
            }

        }
    }


    private class BinaryTreeNode<T> {

        T value;
        BinaryTreeNode<T> left;
        BinaryTreeNode<T> right;
        BinaryTreeNode<T> parent;

        BinaryTreeNode(T payload) {
            this.value = payload;
            left = null;
            right = null;
            parent = null;
        }

        BinaryTreeNode(T payload, BinaryTreeNode<T> parent) {
            this(payload);
            this.parent = parent;
        }
    }
}
