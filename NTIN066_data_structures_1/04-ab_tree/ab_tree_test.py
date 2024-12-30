#!/usr/bin/env python3
import math
import sys

from ab_tree import ABNode, ABTree

def show(tree):
    """Show a tree."""
    def show_node(node, indent):
        for i in reversed(range(len(node.children))):
            if i < len(node.keys):
                print("    " * indent, node.keys[i], sep="")
            if node.children[i]:
                show_node(node.children[i], indent + 1)

    show_node(tree.root, 0)
    print("=" * 70)

def audit(tree):
    """Invariant check for the given tree."""
    def audit_node(node, parent, key_min, key_max, depth, leaf_depth):
        if not node:
            # Check that all leaves are on the same level.
            if leaf_depth is None:
                leaf_depth = depth
            assert depth == leaf_depth, "Leaves are not on the same level"

        else:
            # Check consistency of parent pointers
            assert node.parent == parent, "Inconsistent parent pointers"

            # The number of children must be in the allowed range.
            assert depth == 0 or len(node.children) >= tree.a, "Too few children"
            assert len(node.children) <= tree.b, "Too many children"

            # We must have one more children than keys
            assert len(node.children) == len(node.keys) + 1, "Number of keys does not match number of children"

            # Check that keys are increasing and in (key_min, key_max) range.
            for i in range(len(node.keys)):
                assert node.keys[i] > key_min and node.keys[i] < key_max, "Wrong key order"
                assert i == 0 or node.keys[i - 1] < node.keys[i], "Wrong key order"

            # Check children recursively
            for i in range(len(node.children)):
                child_min = node.keys[i - 1] if i > 0 else key_min
                child_max = node.keys[i] if i < len(node.keys) else key_max
                leaf_depth = audit_node(node.children[i], node, child_min, child_max, depth + 1, leaf_depth)

        return leaf_depth

    assert tree.root, "Tree has no root"
    audit_node(tree.root, None, -math.inf, math.inf, 0, None)

def test_basic():
    """Insert a couple of keys and show how the tree evolves."""
    print("## Basic test")

    tree = ABTree(2, 3)
    keys = [3, 1, 4, 5, 9, 2, 6, 8, 7, 0]
    for key in keys:
        tree.insert(key)
        show(tree)
        audit(tree)
        assert tree.find(key), "Inserted key disappeared"

    for key in keys:
        assert tree.find(key), "Some keys are missing at the end"
    """Try insert all keys again (nothing should change)."""
    for key in keys:
        tree.insert(key)
        audit(tree)
        assert tree.find(key), f"Key {key} that was inserted again has disappeared"

def test_main(a, b, limit, num_items):
    print("## Test Main: a={} b={} range={} num_items={}".format(a, b, limit, num_items))

    tree = ABTree(a, b)

    # Insert keys
    step = int(limit * 1.618)
    key, audit_time = 1, 1
    for i in range(num_items):
        tree.insert(key)
        key = (key + step) % limit

        # Audit the tree occasionally
        if i == audit_time or i + 1 == num_items:
            audit(tree)
            audit_time = int(audit_time * 1.33) + 1

    # Check the content of the tree
    key = 1
    for i in range(limit):
        assert tree.find(key) == (i < num_items), "Tree contains wrong keys"
        key = (key + step) % limit

def test_min(a, b, num_items, repeat):
    print("## Test Min: a={} b={} num_items={} repeat={}".format(a, b, num_items, repeat))

    tree = ABTree(a, b)

    # Insert keys
    for i in range(num_items):
        tree.insert(3*i)
    audit(tree)

    # Delete and insert min
    for i in range(repeat):
        tree.delete_min()
        tree.insert(0)
        if i < 3:
            audit(tree)
    audit(tree)

tests = [
    ("basic", test_basic),
    ("small-2,3", lambda: test_main(2, 3, 997, 700)),
    ("small-2,4", lambda: test_main(2, 4, 997, 700)),
    ("big-2,3", lambda: test_main(2, 3, 99991, 70000)),
    ("big-2,4", lambda: test_main(2, 4, 99991, 70000)),
    ("big-10,20", lambda: test_main(10, 20, 99991, 70000)),
    ("big-100,200", lambda: test_main(100, 200, 99991, 70000)),
    ("min", lambda: test_min(1000,1999,10000,10000))
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
