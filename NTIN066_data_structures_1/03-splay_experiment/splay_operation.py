#!/usr/bin/env python3

class Node:
    """Node in a binary tree `Tree`"""

    def __init__(self, key, left=None, right=None, parent=None):
        self.key = key
        self.parent = parent
        self.left = left
        self.right = right
        if left is not None: left.parent = self
        if right is not None: right.parent = self

class Tree:
    """A simple binary search tree"""

    def __init__(self, root=None):
        self.root = root

    def rotate(self, node):
        """ Rotate the given `node` up.

        Performs a single rotation of the edge between the given node
        and its parent, choosing left or right rotation appropriately.
        """
        parent = node.parent
        grandparent = parent.parent
        if parent.left == node:
            # Right rotation
            parent.left = node.right
            if node.right is not None:
                node.right.parent = parent
            node.right = parent
        else:
            # Left rotation
            parent.right = node.left
            if node.left is not None:
                node.left.parent = parent
            node.left = parent
        parent.parent = node
        node.parent = grandparent
        if grandparent is not None:
            if grandparent.left == parent:
                grandparent.left = node
            else:
                grandparent.right = node
        else:
            self.root = node

    def splay(self, node):
        """Splay the given node."""
        while node.parent is not None:
            parent = node.parent
            grandparent = parent.parent
            if grandparent is None:
                self.rotate(node)
            elif (grandparent.left == parent and parent.left == node):
                self.rotate(parent)
                self.rotate(node)
            elif (grandparent.right == parent and parent.right == node):
                self.rotate(parent)
                self.rotate(node)
            else:
                self.rotate(node)
                self.rotate(node)

    def lookup(self, key):
        """Look up the given key in the tree."""
        node = self.root
        last = None 
        while node is not None:
            last = node
            if key == node.key:
                self.splay(node)
                return node
            elif key < node.key:
                node = node.left
            else:
                node = node.right
        if last is not None:
            self.splay(last)
        return None

    def insert(self, key):
        """Insert key into the tree."""
        if self.root is None:
            self.root = Node(key)
            return

        node = self.root
        while True:
            if key == node.key:
                self.splay(node)
                return
            elif key < node.key:
                if node.left is None:
                    node.left = Node(key, parent=node)
                    self.splay(node.left)
                    return
                node = node.left
            else:
                if node.right is None:
                    node.right = Node(key, parent=node)
                    self.splay(node.right)
                    return
                node = node.right

    def remove(self, key):
        """Remove given key from the tree."""
        node = self.lookup(key)
        if node is None:
            return

        self.splay(node)

        left_subtree = node.left
        right_subtree = node.right

        if left_subtree is not None:
            left_subtree.parent = None
        if right_subtree is not None:
            right_subtree.parent = None

        if left_subtree is not None:
            max_node = left_subtree
            while max_node.right is not None:
                max_node = max_node.right
            self.splay(max_node)
            max_node.right = right_subtree
            if right_subtree is not None:
                right_subtree.parent = max_node
            self.root = max_node
        else:
            self.root = right_subtree

        node.left = node.right = node.parent = None