#!/usr/bin/env python3

import sys
import random

from splay_operation import Tree

class BenchmarkingTree(Tree):
    """ A modified Splay tree for benchmarking.

    We inherit the implementation of operations from the Tree class
    and extend it by keeping statistics on the number of splay operations
    and the total number of rotations. Also, if naive is turned on,
    splay uses only single rotations.
    """

    def __init__(self, naive=False):
        Tree.__init__(self)
        self.do_naive = naive
        self.reset()

    def reset(self):
        """Reset statistics."""
        self.num_rotations = 0;
        self.num_operations = 0;

    def rotate(self, node):
        self.num_rotations += 1
        Tree.rotate(self, node)

    def splay(self, node):
        self.num_operations += 1
        if self.do_naive:
            while node.parent is not None:
                self.rotate(node)
        else:
            Tree.splay(self, node)

    def rot_per_op(self):
        """Return the average number of rotations per operation."""
        if self.num_operations > 0:
            return self.num_rotations / self.num_operations
        else:
            return 0

def test_sequential():
    for n in range(100, 3001, 100):
        tree = BenchmarkingTree(naive)
        for elem in range(n):
            tree.insert(elem)

        for _ in range(5):
            for elem in range(n):
                tree.lookup(elem)

        print(n, tree.rot_per_op())

def test_random():
    for exp in range(32, 64):
        n = int(2**(exp/4))
        tree = BenchmarkingTree(naive)

        for elem in random.sample(range(n), n):
            tree.insert(elem)

        for _ in range(5*n):
            tree.lookup(random.randrange(n))

        print(n, tree.rot_per_op())

def make_progression(seq, A, B, s, inc):
    """An auxiliary function for constructing arithmetic progressions.

    The array seq will be modified to contain an arithmetic progression
    of elements in interval [A,B] starting from position s with step inc.
    """
    for i in range(len(seq)):
        while seq[i] >= A and seq[i] <= B and s + inc*(seq[i]-A) != i:
            pos = s + inc*(seq[i]-A)
            seq[i], seq[pos] = seq[pos], seq[i]

def test_subset():
    for sub in [10, 100, 1000]:
        for exp in range(32,64):
            n = int(2**(exp/4))
            if n < sub:
                continue

            # We will insert elements in order, which contain several
            # arithmetic progressions interspersed with random elements.
            seq = random.sample(range(n), n)
            make_progression(seq, n//4, n//4 + n//20, n//10, 1)
            make_progression(seq, n//2, n//2 + n//20, n//10, -1)
            make_progression(seq, 3*n//4, 3*n//4 + n//20, n//2, -4)
            make_progression(seq, 17*n//20, 17*n//20 + n//20, 2*n//5, 5)

            tree = BenchmarkingTree(naive)
            for elem in seq:
                tree.insert(elem)
            tree.reset()

            for _ in range(10000):
                tree.lookup(seq[random.randrange(sub)])

            print(sub, n, tree.rot_per_op())

tests = {
    "sequential": test_sequential,
    "random": test_random,
    "subset": test_subset,
}

if len(sys.argv) == 4:
    test, student_id = sys.argv[1], sys.argv[2]
    if sys.argv[3] == "std":
        naive = False
    elif sys.argv[3] == "naive":
        naive = True
    else:
        raise ValueError("Last argument must be either 'std' or 'naive'")
    random.seed(student_id)
    if test in tests:
        tests[test]()
    else:
        raise ValueError("Unknown test {}".format(test))
else:
    raise ValueError("Usage: {} <test> <student-id> (std|naive)".format(sys.argv[0]))
