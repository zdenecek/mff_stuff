#!/usr/bin/env python3
import itertools
import math
import sys

from splay_operation import Tree, Node

def test_failed_insert():
    elements = 100000
    tree = Tree()

    for elem in range(elements):
        tree.insert(elem)

    for _ in range(2*elements):
        tree.insert(0)

tests = [
    ("failed_insert", test_failed_insert),
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
