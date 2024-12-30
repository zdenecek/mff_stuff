#!/usr/bin/env python3

class ABNode:
    """Single node in an ABTree.

    Each node contains keys and children
    (with one more children than there are keys).
    We also store a pointer to node's parent (None for root).
    """
    def __init__(self, keys = None, children = None, parent = None):
        self.keys = keys if keys is not None else []
        self.children = children if children is not None else []
        self.parent = parent

    def find_branch(self, key):
        """ Try finding given key in this node.

        If this node contains the given key, returns (True, key_position).
        If not, returns (False, first_position_with_key_greater_than_the_given).
        """
        i = 0
        while (i < len(self.keys) and self.keys[i] < key):
            i += 1

        return (i < len(self.keys) and self.keys[i] == key, i)

    def insert_branch(self, i, key, child):
        """ Insert a new key and a given child between keys i and i+1."""
        self.keys.insert(i, key)
        self.children.insert(i + 1, child)

class ABTree:
    """A class representing the whole ABTree."""
    def __init__(self, a, b):
        assert a >= 2 and b >= 2 * a - 1, "Invalid values of a, b: {}, {}".format(a, b)
        self.a = a
        self.b = b
        self.root = ABNode(children=[None])

    def find(self, key):
        """Find a key in the tree.

        Returns True if the key is present, False otherwise.
        """
        node = self.root
        while node:
            found, i = node.find_branch(key)
            if found: return True
            node = node.children[i]
        return False

    def delete_min(self):
        """ Delete the smallest element. """
        node = self.root
        while node.children[0]:
            node = node.children[0]

        node.children.pop(0)
        node.keys.pop(0)

        while len(node.children) < self.a and node.parent:
            node = node.parent
            first = node.children[0]
            second = node.children[1]

            # Merge the second to the first
            if len(second.children) == self.a:
                if second.children[0]:
                    for c in second.children:
                        c.parent = first
                first.children.extend(second.children)
                first.keys.append(node.keys.pop(0))
                first.keys.extend(second.keys)
                node.children.pop(1)

            # Move the leftest child of the second to the first
            else:
                second.children[0].parent = first
                first.children.append(second.children.pop(0))
                first.keys.append(node.keys[0])
                node.keys[0] = second.keys.pop(0)

        if len(node.children) == 1:
            assert node == self.root
            node.parent = None
            self.root = node.children[0]

    def split_node(self, node, size):
        """Helper function for insert.

        Split node into two nodes. The middle key is promoted to the parent.
        Returns (left_node, right_node, median_key)
        """
        n = len(node.keys)
        m = size
        if m < self.a - 1:
            m = self.a - 1
        elif n - m - 1 < self.a - 1:
            m = n - self.a

        median_key = node.keys[m]
        left_keys = node.keys[:m]
        right_keys = node.keys[m + 1:]
        left_children = node.children[:m + 1]
        right_children = node.children[m + 1:]

        left_node = ABNode(left_keys, left_children, node.parent)
        right_node = ABNode(right_keys, right_children, node.parent)

        for child in filter(lambda x: x, left_children):
            child.parent = left_node
        for child in filter(lambda x: x, right_children):
            child.parent = right_node

        return left_node, right_node, median_key

    def insert(self, key):
        """Add a given key to the tree, unless already present."""
        node = self.root
        while node.children[0] is not None:
            found, i = node.find_branch(key)
            if found:
                return  
            node = node.children[i]

        found, i = node.find_branch(key)
        if found:
            return  
        node.keys.insert(i, key)
        node.children.insert(i + 1, None)

        while node is not None and len(node.keys) >= self.b:
            size = (len(node.keys) - 1) // 2
            left_node, right_node, median_key = self.split_node(node, size)
            if node.parent is None:
                new_root = ABNode(keys=[median_key], children=[left_node, right_node], parent=None)
                left_node.parent = new_root
                right_node.parent = new_root
                self.root = new_root
                node = None  
            else:
                parent = node.parent
                index = parent.children.index(node)
                parent.children[index] = left_node
                parent.children.insert(index + 1, right_node)
                parent.keys.insert(index, median_key)
                left_node.parent = parent
                right_node.parent = parent
                node = parent
