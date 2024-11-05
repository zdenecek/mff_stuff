#!/usr/bin/env python3
import itertools
import math
import sys

from splay_operation import Tree, Node

def flatten(tree):
    """Flatten given tree in ascending order."""
    L, R, F = 0, 1, 2

    node, stack, flattened = tree.root, [L], []
    while node is not None:
        if stack[-1] == L:
            stack[-1] = R
            if node.left is not None:
                node = node.left
                stack.append(L)
        elif stack[-1] == R:
            flattened.append(node.key)
            stack[-1] = F
            if node.right is not None:
                node = node.right
                stack.append(L)
        else:
            node = node.parent
            stack.pop()

    return flattened

def test_splay():
    def deserialize_tree(string):
        def deserialize_node(i):
            assert string[i] == "("
            i += 1
            if string[i] == ")":
                return i + 1, None
            else:
                comma = string.find(",", i)
                comma2, left = deserialize_node(comma + 1)
                rparen, right = deserialize_node(comma2 + 1)
                assert string[rparen] == ")"
                return rparen + 1, Node(int(string[i : comma]), left=left, right=right)

        index, root = deserialize_node(0)
        assert index == len(string)
        return Tree(root)

    def compare(system, gold):
        if system is None and gold is not None:
            return "expected node with key {}, found None".format(gold.key)
        elif system is not None and gold is None:
            return "expected None, found node with key {}".format(system.key)
        elif system is not None and gold is not None:
            if system.key != gold.key:
                return "expected node with key {}, found {}".format(gold.key, system.key)
            return compare(system.left, gold.left) or compare(system.right, gold.right)

    with open("splay_tests.txt", "r") as splay_tests_file:
        for line in splay_tests_file:
            original_serialized, target_serialized, splayed_serialized = line.rstrip("\n").split()
            original = deserialize_tree(original_serialized)
            splayed = deserialize_tree(splayed_serialized)
            target = int(target_serialized)

            node = original.root
            while node is not None and node.key != target:
                if target < node.key: node = node.left
                else: node = node.right
            assert node is not None

            original.splay(node)
            error = compare(original.root, splayed.root)
            assert not error, "Error running splay on key {} of {}: {}".format(node.key, original_serialized, error)

def test_lookup():
    tree = Tree()
    for elem in range(0, 100000, 2):
        tree.insert(elem)

    # Find non-existing
    for elem in range(1, 100000, 2):
        for _ in range(10):
            assert tree.lookup(elem) is None, "Non-existing element was found"

    # Find existing
    for elem in range(0, 100000, 2):
        for _ in range(10):
            assert tree.lookup(elem) is not None, "Existing element was not found"

def test_insert():
    # Test validity first
    tree = Tree()
    sequence = [pow(997, i, 1999) for i in range(1, 1999)]
    for elem in sequence:
        tree.insert(elem)
    assert flatten(tree) == sorted(sequence), "Incorrect tree after a sequence of inserts"

    # Test speed
    elements = 200000 # Must be even!
    tree = Tree()
    for elem in range(elements):
        for _ in range(10):
            tree.insert(elem)

    tree = Tree()
    for elem in reversed(range(elements)):
        tree.insert(elem)
    for elem in range(elements):
        tree.insert(elements)

def test_remove():
    # Test validity first
    tree = Tree()
    for elem in range(2, 1999 * 2):
        tree.insert(elem)

    sequence = [2 * pow(997, i, 1999) for i in range(1, 1999)]
    for elem in sequence:
        tree.remove(elem + 1)
    assert flatten(tree) == sorted(sequence), "Incorrect tree after a sequence of removes"

    # Test speed
    elements = 200000
    tree = Tree()
    for elem in range(0, elements, 2):
        tree.insert(elem)

    # Non-existing elements
    for elem in range(1, elements, 2):
        for _ in range(10):
            tree.remove(elem)

    # Existing elements
    for elem in range(2, elements, 2):
        tree.remove(elem)

    tree = Tree()
    for elem in range(1, elements):
        tree.insert(elem)
    for elem in range(elements):
        tree.remove(0)

    left_subtree = right_subtree = None
    for i in reversed(range(0, elements//2)):
        left_subtree = Node(i, right=left_subtree)
        right_subtree = Node(elements-i, left=right_subtree)
    node = Node(elements//2, left=left_subtree, right=right_subtree)
    tree = Tree(node)
    while tree.root is not None:
        tree.remove(tree.root.key)

tests = [
    ("splay", test_splay),
    ("lookup", test_lookup),
    ("insert", test_insert),
    ("remove", test_remove),
]

if __name__ == "__main__":
    for required_test in sys.argv[1:] or [name for name, _ in tests]:
        for name, test in tests:
            if name == required_test:
                print("Running test {}".format(name), file=sys.stderr)
                test()
                break
        else:
            raise ValueError("Unknown test {}".format(name))
