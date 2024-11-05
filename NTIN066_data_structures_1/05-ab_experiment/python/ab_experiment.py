#!/usr/bin/env python3

import sys
import random

from ab_tree import ABTree

class BenchmarkingABTree(ABTree):
    """A modified ABTree for benchmarking.

    We inherit the implementation of operations from the ABTree class
    and extend it by delete operation and by keeping statistics on the number
    of operations and the total number of structural changes.
    """
    def __init__(self, a, b):
        ABTree.__init__(self, a, b)
        self.reset()

    def reset(self):
        """ Reset statistics """
        self.num_operations = 0
        self.num_struct_changes = 0

    def struct_changes_per_op(self):
        """Return the average number of struct. changes per operation."""
        if self.num_operations > 0:
            return self.num_struct_changes / self.num_operations
        else:
            return 0

    def insert(self, key):
        self.num_operations += 1
        ABTree.insert(self, key)

    def split_node(self, node, size):
        self.num_struct_changes += 1
        return ABTree.split_node(self, node, size)

    def remove(self, key):
        """ Delete key from the tree. Does nothing if the key is not in the tree. """
        self.num_operations += 1

        # Find the key to be deleted
        node = self.root
        found, i = node.find_branch(key)
        while not found:
            node = node.children[i]
            if not node: return     # Key is not in the tree
            found, i = node.find_branch(key)

        # If node is not a leaf, we need to swap the key with its successor
        if node.children[0] is not None:        # Only leaves have None as children
            # Successor is leftmost key in the right subtree of key
            succ = self._min(node.children[i+1])
            node.keys[i], succ.keys[0] = succ.keys[0], node.keys[i]
            node = succ

        # Now run the main part of the delete
        self._remove_leaf(key, node)

    def _remove_leaf(self, key, node):
        """ Main part of the delete.
        """
        assert node is not None, "Trying to delete key from None"
        assert node.children[0] is None, "Leaf's child must be None"

        while True:
            # Find the key in the node
            found, key_position = node.find_branch(key)
            assert found, "Trying to delete key that is not in the node."

            # Start with the deleting itself
            del node.keys[key_position]
            del node.children[key_position + 1]

            # No underflow means we are done
            if len(node.children) >= self.a: return

            # Root may underflow, but cannot have just one child (unless tree is empty)
            if node == self.root:
                if (len(node.children) == 1) and (self.root.children[0] is not None):
                    self.root = self.root.children[0]
                    self.root.parent = None
                return

            brother, separating_key_pos, _ = self._get_brother(node)
            separating_key = node.parent.keys[separating_key_pos]

            # First check whether we can steal brother's child
            if len(brother.children) > self.a:
                self._steal_child(node)
                return

            # If the brother is too small, we merge with him and propagate the delete
            node = self.merge_node(node)
            node, key, key_position = node.parent, separating_key, separating_key_pos

    def _min(self, node):
        """ Return the leftmost node of a subtree rooted at node."""
        assert node is not None
        while node.children[0] is not None:
            node = node.children[0]
        return node

    def _get_brother(self, node):
        """ Return the left brother if it exists, otherwise return right brother.
            returns tuple (brother, key_position, is_left_brother), where
            key_position is a position of the key that separates node and brother in their parent.
        """
        parent = node.parent
        assert parent is not None, "Node without parent has no brother"

        # Find node in parent's child list
        i = 0
        for c in parent.children:
            if c is node: break
            else: i += 1
        assert i < len(parent.children), "Node is not inside its parent"

        if i == 0:
            return parent.children[1], 0, False
        else:
            return parent.children[i - 1], i - 1, True

    def _steal_child(self, node):
        """ Transfer one child from node's left brother to the node.
            If node has no left brother, use right brother instead.
        """
        brother, separating_key_pos, is_left_brother = self._get_brother(node)
        separating_key = node.parent.keys[separating_key_pos]

        assert len(brother.children) > self.a, "Stealing child causes underflow in brother!"
        assert len(node.children) < self.b, "Stealing child causes overflow in the node!"

        # We steal either from front or back
        if is_left_brother:
            steal_position = len(brother.children)-1
            target_position = 0
        else:
            steal_position = 0
            target_position = len(node.children)
        # Steal the child
        stolen_child = brother.children[steal_position]
        if stolen_child is not None:
            stolen_child.parent = node
        node.children.insert(target_position, stolen_child)
        del brother.children[steal_position]

        # List of keys is shorter than list of children
        if is_left_brother:
            steal_position -= 1
        else:
            target_position -= 1
        # Update keys
        node.keys.insert(target_position, separating_key)
        node.parent.keys[separating_key_pos] = brother.keys[steal_position]
        del brother.keys[steal_position]

    def merge_node(self, node):
        """ Merge node with its left brother and destroy the node. Must not cause overflow!

        Returns result of the merge.
        If node has no left brother, use right brother instead.
        """
        self.num_struct_changes += 1

        brother, separating_key_pos, is_left_brother = self._get_brother(node)
        separating_key = node.parent.keys[separating_key_pos]

        # We swap brother and node if necessary so that the node is always on the right
        if not is_left_brother:
            brother, node = node, brother

        brother.children.extend(node.children)
        brother.keys.append(separating_key)
        brother.keys.extend(node.keys)

        assert len(brother.children) <= self.b, "Merge caused overflow!"

        # Update parent pointers in non-leaf
        if brother.children[0] is not None:
            for c in brother.children:
                c.parent = brother
        return brother

def test_insert():
    for exp in range(32, 64):
        n = int(2**(exp/4))
        tree = BenchmarkingABTree(a, b)

        for elem in random.sample(range(n), n):
            tree.insert(elem)

        print(n, tree.struct_changes_per_op())

def test_random():
    for exp in range(32, 64):
        n = int(2**(exp/4))
        tree = BenchmarkingABTree(a, b)

        for elem in range(0, 2*n, 2):
            tree.insert(elem)

        # We keep track of elements present and not present in the tree
        elems = list(range(0, n, 2))
        anti_elems = list(range(-1, 2*n+1, 2))

        for _ in range(n):
            # Delete random element
            elem = random.choice(elems)
            tree.remove(elem)
            elems.remove(elem)
            anti_elems.append(elem)

            # Insert random "anti-element"
            elem = random.choice(anti_elems)
            tree.insert(elem)
            elems.append(elem)
            anti_elems.remove(elem)

        print(n, tree.struct_changes_per_op())

def test_min():
    for exp in range(32, 64):
        n = int(2 ** (exp / 4))
        tree = BenchmarkingABTree(a, b)

        for i in range(n):
            tree.insert(i)

        for _ in range(n):
            tree.remove(0)
            tree.insert(0)

        print(n, tree.struct_changes_per_op())

tests = {
    "min": test_min,
    "insert": test_insert,
    "random": test_random,
}

if __name__ == '__main__':
    if len(sys.argv) == 4:
        test, student_id = sys.argv[1], sys.argv[2]
        a = 2
        if sys.argv[3] == "2-3":
            b = 3
        elif sys.argv[3] == "2-4":
            b = 4
        else:
            raise ValueError("Last argument must be either '2-3' or '2-4'")
        random.seed(student_id)
        if test in tests:
            tests[test]()
        else:
            raise ValueError("Unknown test {}".format(test))
    else:
        raise ValueError("Usage: {} <test> <student-id> (2-3|2-4)".format(sys.argv[0]))
